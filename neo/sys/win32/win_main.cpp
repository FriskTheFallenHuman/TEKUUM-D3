/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include <errno.h>
#include <float.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <shellapi.h>
#include <shlobj.h>

#ifndef __MRC__
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

#include "../sys_local.h"
#include "win_local.h"
#include "rc/CreateResourceIDs.h"
#include "../../renderer/RenderCommon.h"

idCVar Win32Vars_t::sys_cpustring( "sys_cpustring", "detect", CVAR_SYSTEM | CVAR_INIT, "" );
idCVar Win32Vars_t::in_mouse( "in_mouse", "1", CVAR_SYSTEM | CVAR_BOOL, "enable mouse input" );
idCVar Win32Vars_t::win_allowAltTab( "win_allowAltTab", "0", CVAR_SYSTEM | CVAR_BOOL, "allow Alt-Tab when fullscreen" );
idCVar Win32Vars_t::win_notaskkeys( "win_notaskkeys", "0", CVAR_SYSTEM | CVAR_INTEGER, "disable windows task keys" );
idCVar Win32Vars_t::win_outputDebugString( "win_outputDebugString", "1", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar Win32Vars_t::win_outputEditString( "win_outputEditString", "1", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar Win32Vars_t::win_viewlog( "win_viewlog", "0", CVAR_SYSTEM | CVAR_INTEGER, "" );
idCVar Win32Vars_t::win_timerUpdate( "win_timerUpdate", "0", CVAR_SYSTEM | CVAR_BOOL, "allows the game to be updated while dragging the window" );

Win32Vars_t	win32;

static char		sys_cmdline[MAX_STRING_CHARS];

// not a hard limit, just what we keep track of for debugging
xthreadInfo* g_threads[MAX_THREADS];

int g_thread_count = 0;

static	xthreadInfo	threadInfo;
static	HANDLE		hTimer;

/*
==================
Sys_Createthread
==================
*/
void Sys_CreateThread( xthread_t function, void* parms, xthreadPriority priority, xthreadInfo& info, const char* name, xthreadInfo* threads[MAX_THREADS], int* thread_count )
{
	DWORD id;
	HANDLE temp = CreateThread(	NULL,	// LPSECURITY_ATTRIBUTES lpsa,
								0,		// DWORD cbStack,
								( LPTHREAD_START_ROUTINE )function,	// LPTHREAD_START_ROUTINE lpStartAddr,
								parms,	// LPVOID lpvThreadParm,
								0,		//   DWORD fdwCreate,
								&id );

	info.threadId = id;
	info.threadHandle = ( intptr_t ) temp;
	if( priority == THREAD_HIGHEST )
	{
		SetThreadPriority( ( HANDLE )info.threadHandle, THREAD_PRIORITY_HIGHEST );		//  we better sleep enough to do this
	}
	else if( priority == THREAD_ABOVE_NORMAL )
	{
		SetThreadPriority( ( HANDLE )info.threadHandle, THREAD_PRIORITY_ABOVE_NORMAL );
	}
	info.name = name;
	if( *thread_count < MAX_THREADS )
	{
		threads[( *thread_count )++] = &info;
	}
	else
	{
		common->DPrintf( "WARNING: MAX_THREADS reached\n" );
	}
}

/*
==================
Sys_DestroyThread
==================
*/
void Sys_DestroyThread( xthreadInfo& info )
{
	WaitForSingleObject( ( HANDLE )info.threadHandle, INFINITE );
	CloseHandle( ( HANDLE )info.threadHandle );
	info.threadHandle = 0;
}

/*
==================
Sys_GetThreadName
==================
*/
const char* Sys_GetThreadName( int* index )
{
	int id = GetCurrentThreadId();
	for( int i = 0; i < g_thread_count; i++ )
	{
		if( id == g_threads[i]->threadId )
		{
			if( index )
			{
				*index = i;
			}
			return g_threads[i]->name;
		}
	}
	if( index )
	{
		*index = -1;
	}
	return "main";
}

// RB begin
void Sys_InitCriticalSections()
{
	for( int i = 0; i < MAX_CRITICAL_SECTIONS; i++ )
	{
		InitializeCriticalSection( &win32.criticalSections[i] );
	}
}
// RB end

/*
==================
Sys_EnterCriticalSection
==================
*/
void Sys_EnterCriticalSection( int index )
{
	assert( index >= 0 && index < MAX_CRITICAL_SECTIONS );
	if( TryEnterCriticalSection( &win32.criticalSections[index] ) == 0 )
	{
		EnterCriticalSection( &win32.criticalSections[index] );
//		Sys_DebugPrintf( "busy lock '%s' in thread '%s'\n", lock->name, Sys_GetThreadName() );
	}
}

/*
==================
Sys_LeaveCriticalSection
==================
*/
void Sys_LeaveCriticalSection( int index )
{
	assert( index >= 0 && index < MAX_CRITICAL_SECTIONS );
	LeaveCriticalSection( &win32.criticalSections[index] );
}

/*
==================
Sys_WaitForEvent
==================
*/
void Sys_WaitForEvent( int index )
{
	assert( index == 0 );
	if( !win32.backgroundDownloadSemaphore )
	{
		win32.backgroundDownloadSemaphore = CreateEvent( NULL, TRUE, FALSE, NULL );
	}
	WaitForSingleObject( win32.backgroundDownloadSemaphore, INFINITE );
	ResetEvent( win32.backgroundDownloadSemaphore );
}

/*
==================
Sys_TriggerEvent
==================
*/
void Sys_TriggerEvent( int index )
{
	assert( index == 0 );
	SetEvent( win32.backgroundDownloadSemaphore );
}

/*
=============
Sys_Error

Show the early console as an error dialog
=============
*/
void Sys_Error( const char* error, ... )
{
	va_list		argptr;
	char		text[4096];
	MSG        msg;

	va_start( argptr, error );
	vsprintf( text, error, argptr );
	va_end( argptr );

	Conbuf_AppendText( text );
	Conbuf_AppendText( "\n" );

	Win_SetErrorText( text );
	Sys_ShowConsole( 1, true );

	timeEndPeriod( 1 );

	Sys_ShutdownInput();

	GLimp_Shutdown();

	// wait for the user to quit
	while( 1 )
	{
		if( !GetMessage( &msg, NULL, 0, 0 ) )
		{
			common->Quit();
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	Sys_DestroyConsole();

	exit( 1 );
}

/*
==============
Sys_Quit
==============
*/
void Sys_Quit()
{
	timeEndPeriod( 1 );
	Sys_ShutdownInput();

	// RB begin
	Sys_DestroyConsole();

#ifdef ID_ALLOW_TOOLS
	if( com_editors )
	{
		if( com_editors & EDITOR_RADIANT )
		{
			// Level Editor
			RadiantShutdown();
		}
	}
#endif
	// RB end
	ExitProcess( 0 );
}


/*
==============
Sys_Printf
==============
*/
#define MAXPRINTMSG 4096
void Sys_Printf( const char* fmt, ... )
{
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, MAXPRINTMSG - 1, fmt, argptr );
	va_end( argptr );
	msg[sizeof( msg ) - 1] = '\0';

	// RB: added thread check
	if( win32.win_outputDebugString.GetBool() && idLib::IsMainThread() )
	{
		OutputDebugString( msg );
	}

	if( win32.win_outputEditString.GetBool() && idLib::IsMainThread() )
	{
		Conbuf_AppendText( msg );
	}
}

/*
==============
Sys_DebugPrintf
==============
*/
#define MAXPRINTMSG 4096
void Sys_DebugPrintf( const char* fmt, ... )
{
	char msg[MAXPRINTMSG];

	va_list argptr;
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, MAXPRINTMSG - 1, fmt, argptr );
	msg[ sizeof( msg ) - 1 ] = '\0';
	va_end( argptr );

	OutputDebugString( msg );
}

/*
==============
Sys_DebugVPrintf
==============
*/
void Sys_DebugVPrintf( const char* fmt, va_list arg )
{
	char msg[MAXPRINTMSG];

	idStr::vsnPrintf( msg, MAXPRINTMSG - 1, fmt, arg );
	msg[ sizeof( msg ) - 1 ] = '\0';

	OutputDebugString( msg );
}

/*
==============
Sys_Sleep
==============
*/
void Sys_Sleep( int msec )
{
	Sleep( msec );
}

/*
==============
Sys_ShowWindow
==============
*/
void Sys_ShowWindow( bool show )
{
	::ShowWindow( win32.hWnd, show ? SW_SHOW : SW_HIDE );
}

/*
==============
Sys_IsWindowVisible
==============
*/
bool Sys_IsWindowVisible()
{
	return ( ::IsWindowVisible( win32.hWnd ) != 0 );
}

/*
==============
Sys_Mkdir
==============
*/
void Sys_Mkdir( const char* path )
{
	_mkdir( path );
}

/*
=================
Sys_FileTimeStamp
=================
*/
ID_TIME_T Sys_FileTimeStamp( idFileHandle fp )
{
	FILETIME writeTime;
	GetFileTime( fp, NULL, NULL, &writeTime );

	/*
		FILETIME = number of 100-nanosecond ticks since midnight
		1 Jan 1601 UTC. time_t = number of 1-second ticks since
		midnight 1 Jan 1970 UTC. To translate, we subtract a
		FILETIME representation of midnight, 1 Jan 1970 from the
		time in question and divide by the number of 100-ns ticks
		in one second.
	*/

	SYSTEMTIME base_st =
	{
		1970,   // wYear
		1,      // wMonth
		0,      // wDayOfWeek
		1,      // wDay
		0,      // wHour
		0,      // wMinute
		0,      // wSecond
		0       // wMilliseconds
	};

	FILETIME base_ft;
	SystemTimeToFileTime( &base_st, &base_ft );

	LARGE_INTEGER itime;
	itime.QuadPart = reinterpret_cast<LARGE_INTEGER&>( writeTime ).QuadPart;
	itime.QuadPart -= reinterpret_cast<LARGE_INTEGER&>( base_ft ).QuadPart;
	itime.QuadPart /= 10000000LL;
	return itime.QuadPart;
}

/*
==============
Sys_Cwd
==============
*/
const char* Sys_Cwd()
{
	static char cwd[MAX_OSPATH];

	_getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH - 1] = 0;

	return cwd;
}

static int WPath2A( char* dst, size_t size, const WCHAR* src )
{
	int len;
	BOOL default_char = FALSE;

	// test if we can convert lossless
	len = WideCharToMultiByte( CP_ACP, 0, src, -1, dst, size, NULL, &default_char );

	if( default_char )
	{
		/* The following lines implement a horrible
		   hack to connect the UTF-16 WinAPI to the
		   ASCII doom3 strings. While this should work in
		   most cases, it'll fail if the "Windows to
		   DOS filename translation" is switched off.
		   In that case the function will return NULL
		   and no homedir is used. */
		WCHAR w[MAX_OSPATH];
		len = GetShortPathNameW( src, w, sizeof( w ) );

		if( len == 0 )
		{
			return 0;
		}

		/* Since the DOS path contains no UTF-16 characters, convert it to the system's default code page */
		len = WideCharToMultiByte( CP_ACP, 0, w, len, dst, size - 1, NULL, NULL );
	}

	if( len == 0 )
	{
		return 0;
	}

	dst[len] = 0;

	/* Replace backslashes by slashes */
	for( int i = 0; i < len; ++i )
		if( dst[i] == '\\' )
		{
			dst[i] = '/';
		}

	// cut trailing slash
	if( dst[len - 1] == '/' )
	{
		dst[len - 1] = 0;
		len--;
	}

	return len;
}

/*
==============
Returns "My Documents"/My Games/rbdoom3 directory (or equivalent - "CSIDL_PERSONAL").
To be used with Sys_GetPath(PATH_SAVE), so savegames, screenshots etc will be
saved to the users files instead of systemwide.

Based on (with kind permission) Yamagi Quake II's Sys_GetHomeDir()

Returns the number of characters written to dst
==============
 */
extern "C" { // DG: I need this in SDL_win32_main.c
	int Win_GetHomeDir( char* dst, size_t size )
	{
		int len;
		WCHAR profile[MAX_OSPATH];

		/* Get the path to "My Documents" directory */
		SHGetFolderPathW( NULL, CSIDL_PERSONAL, NULL, 0, profile );

		len = WPath2A( dst, size, profile );
		if( len == 0 )
		{
			return 0;
		}

		idStr::Append( dst, size, "/My Games/rbdoom3" );

		return len;
	}
}

static int GetRegistryPath( char* dst, size_t size, const WCHAR* subkey, const WCHAR* name )
{
	WCHAR w[MAX_OSPATH];
	DWORD len = sizeof( w );
	HKEY res;
	DWORD sam = KEY_QUERY_VALUE
#ifdef _WIN64
				| KEY_WOW64_32KEY
#endif
				;
	DWORD type;

	if( RegOpenKeyExW( HKEY_LOCAL_MACHINE, subkey, 0, sam, &res ) != ERROR_SUCCESS )
	{
		return 0;
	}

	if( RegQueryValueExW( res, name, NULL, &type, ( LPBYTE )w, &len ) != ERROR_SUCCESS )
	{
		RegCloseKey( res );
		return 0;
	}

	RegCloseKey( res );

	if( type != REG_SZ )
	{
		return 0;
	}

	return WPath2A( dst, size, w );
}

bool Sys_GetPath( sysPath_t type, idStr& path )
{
	char buf[MAX_OSPATH];
	struct _stat st;
	idStr s;

	switch( type )
	{
		case PATH_BASE:
			// try <path to exe>/base first
			if( Sys_GetPath( PATH_EXE, path ) )
			{
				path.StripFilename();

				s = path;
				s.AppendPath( BASE_GAMEDIR );
				if( _stat( s.c_str(), &st ) != -1 && ( st.st_mode & _S_IFDIR ) )
				{
					common->Warning( "using path of executable: %s", path.c_str() );
					return true;
				}
				else
				{
					s = path + "/demo/demo00.pk4";
					if( _stat( s.c_str(), &st ) != -1 && ( st.st_mode & _S_IFREG ) )
					{
						common->Warning( "using path of executable (seems to contain demo game data): %s ", path.c_str() );
						return true;
					}
				}

				common->Warning( "base path '%s' does not exist", s.c_str() );
			}

			// Note: apparently there is no registry entry for the Doom 3 Demo

			// fallback to vanilla doom3 cd install
			if( GetRegistryPath( buf, sizeof( buf ), L"SOFTWARE\\id\\Doom 3", L"InstallPath" ) > 0 )
			{
				path = buf;
				return true;
			}

			// fallback to steam doom3 install
			if( GetRegistryPath( buf, sizeof( buf ), L"SOFTWARE\\Valve\\Steam", L"InstallPath" ) > 0 )
			{
				path = buf;
				path.AppendPath( "steamapps\\common\\doom 3" );

				if( _stat( path.c_str(), &st ) != -1 && st.st_mode & _S_IFDIR )
				{
					return true;
				}
			}

			common->Warning( "vanilla doom3 path not found either" );

			return false;

		case PATH_CONFIG:
		case PATH_SAVE:
			if( Win_GetHomeDir( buf, sizeof( buf ) ) < 1 )
			{
				Sys_Error( "ERROR: Couldn't get dir to home path" );
				return false;
			}

			path = buf;
			return true;

		case PATH_EXE:
			GetModuleFileName( NULL, buf, sizeof( buf ) - 1 );
			path = buf;
			path.BackSlashesToSlashes();
			return true;
	}

	return false;
}

/*
==============
Sys_ListFiles
==============
*/
int Sys_ListFiles( const char* directory, const char* extension, idStrList& list )
{
	idStr		search;
	struct _finddata_t findinfo;
	intptr_t	findhandle;
	int			flag;

	if( !extension )
	{
		extension = "";
	}

	// passing a slash as extension will find directories
	if( extension[0] == '/' && extension[1] == 0 )
	{
		extension = "";
		flag = 0;
	}
	else
	{
		flag = _A_SUBDIR;
	}

	sprintf( search, "%s\\*%s", directory, extension );

	// search
	list.Clear();

	findhandle = _findfirst( search, &findinfo );
	if( findhandle == -1 )
	{
		return -1;
	}

	do
	{
		if( flag ^ ( findinfo.attrib & _A_SUBDIR ) )
		{
			list.Append( findinfo.name );
		}
	}
	while( _findnext( findhandle, &findinfo ) != -1 );

	_findclose( findhandle );

	return list.Num();
}


/*
================
Sys_GetClipboardData
================
*/
char* Sys_GetClipboardData()
{
	char* data = NULL;
	char* cliptext;

	if( OpenClipboard( NULL ) != 0 )
	{
		HANDLE hClipboardData;

		if( ( hClipboardData = GetClipboardData( CF_TEXT ) ) != 0 )
		{
			if( ( cliptext = ( char* )GlobalLock( hClipboardData ) ) != 0 )
			{
				data = ( char* )Mem_Alloc( GlobalSize( hClipboardData ) + 1 );
				strcpy( data, cliptext );
				GlobalUnlock( hClipboardData );

				strtok( data, "\n\r\b" );
			}
		}
		CloseClipboard();
	}
	return data;
}

/*
================
Sys_SetClipboardData
================
*/
void Sys_SetClipboardData( const char* string )
{
	HGLOBAL HMem;
	char* PMem;

	// allocate memory block
	HMem = ( char* )::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, strlen( string ) + 1 );
	if( HMem == NULL )
	{
		return;
	}
	// lock allocated memory and obtain a pointer
	PMem = ( char* )::GlobalLock( HMem );
	if( PMem == NULL )
	{
		return;
	}
	// copy text into allocated memory block
	lstrcpy( PMem, string );
	// unlock allocated memory
	::GlobalUnlock( HMem );
	// open Clipboard
	if( !OpenClipboard( 0 ) )
	{
		::GlobalFree( HMem );
		return;
	}
	// remove current Clipboard contents
	EmptyClipboard();
	// supply the memory handle to the Clipboard
	SetClipboardData( CF_TEXT, HMem );
	HMem = 0;
	// close Clipboard
	CloseClipboard();
}

/*
========================================================================

DLL Loading

========================================================================
*/

/*
=====================
Sys_DLL_Load
=====================
*/
uintptr_t Sys_DLL_Load( const char* dllName )
{
	HINSTANCE	libHandle;
	libHandle = LoadLibrary( dllName );
	if( libHandle )
	{
		// since we can't have LoadLibrary load only from the specified path, check it did the right thing
		char loadedPath[ MAX_OSPATH ];
		GetModuleFileName( libHandle, loadedPath, sizeof( loadedPath ) - 1 );
		if( idStr::IcmpPath( dllName, loadedPath ) )
		{
			Sys_Printf( "ERROR: LoadLibrary '%s' wants to load '%s'\n", dllName, loadedPath );
			Sys_DLL_Unload( ( uintptr_t )libHandle );
			return 0;
		}
	}
	else
	{
		DWORD e = GetLastError();
		LPVOID msgBuf = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			e,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			( LPTSTR )&msgBuf,
			0, NULL );

		idStr errorStr = va( "[%i (0x%X)]\t%s", e, e, msgBuf );

		// common, skipped.
		if( e == 0x7E )  // [126 (0x7E)] The specified module could not be found.
		{
			errorStr = "";
		}
		// probably going to be common. Lets try to be less cryptic.
		else if( e == 0xC1 )  // [193 (0xC1)] is not a valid Win32 application.
		{
			errorStr = va( "[%i (0x%X)]\t%s", e, e, "probably the DLL is of the wrong architecture, like x64 instead of x86" );
		}

		if( errorStr.Length() )
		{
			common->Warning( "LoadLibrary(%s) Failed ! %s", dllName, errorStr.c_str() );
		}

		::LocalFree( msgBuf );
	}
	return ( uintptr_t )libHandle;
}

/*
=====================
Sys_DLL_GetProcAddress
=====================
*/
void* Sys_DLL_GetProcAddress( uintptr_t dllHandle, const char* procName )
{
	void* adr = ( void* )GetProcAddress( ( HINSTANCE )dllHandle, procName );
	if( !adr )
	{
		DWORD e = GetLastError();
		LPVOID msgBuf = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			e,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			( LPTSTR )&msgBuf,
			0, NULL );

		idStr errorStr = va( "[%i (0x%X)]\t%s", e, e, msgBuf );

		if( errorStr.Length() )
		{
			common->Warning( "GetProcAddress( %i %s) Failed ! %s", dllHandle, procName, errorStr.c_str() );
		}

		::LocalFree( msgBuf );
	}
	return adr;
}

/*
=====================
Sys_DLL_Unload
=====================
*/
void Sys_DLL_Unload( uintptr_t dllHandle )
{
	if( !dllHandle )
	{
		return;
	}
	if( FreeLibrary( ( HINSTANCE )dllHandle ) == 0 )
	{
		int lastError = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			lastError,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			( LPTSTR ) &lpMsgBuf,
			0,
			NULL
		);
		Sys_Error( "Sys_DLL_Unload: FreeLibrary failed - %s (%d)", lpMsgBuf, lastError );
	}
}

/*
========================================================================

EVENT LOOP

========================================================================
*/

#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t	eventQue[MAX_QUED_EVENTS];
int			eventHead = 0;
int			eventTail = 0;

/*
================
Sys_QueEvent

Ptr should either be null, or point to a block of data that can
be freed by the game later.
================
*/
void Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void* ptr, int inputDeviceNum )
{
	sysEvent_t* ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

	if( eventHead - eventTail >= MAX_QUED_EVENTS )
	{
		common->Printf( "Sys_QueEvent: overflow\n" );
		// we are discarding an event, but don't leak memory
		if( ev->evPtr )
		{
			Mem_Free( ev->evPtr );
		}
		eventTail++;
	}

	eventHead++;

	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
	ev->inputDevice = inputDeviceNum;
}

/*
=============
Sys_PumpEvents

This allows windows to be moved during renderbump
=============
*/
void Sys_PumpEvents()
{
	MSG msg;

	// pump the message loop
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( !GetMessage( &msg, NULL, 0, 0 ) )
		{
			common->Quit();
		}

		// save the msg time, because wndprocs don't have access to the timestamp
		if( win32.sysMsgTime && win32.sysMsgTime > ( int )msg.time )
		{
			// don't ever let the event times run backwards
//			common->Printf( "Sys_PumpEvents: win32.sysMsgTime (%i) > msg.time (%i)\n", win32.sysMsgTime, msg.time );
		}
		else
		{
			win32.sysMsgTime = msg.time;
		}

#ifdef ID_ALLOW_TOOLS
		if( GUIEditorHandleMessage( &msg ) )
		{
			continue;
		}
#endif

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

/*
================
Sys_GenerateEvents
================
*/
void Sys_GenerateEvents()
{
	static int entered = false;
	char* s;

	if( entered )
	{
		return;
	}
	entered = true;

	// pump the message loop
	Sys_PumpEvents();

	// grab or release the mouse cursor if necessary
	IN_Frame();

	// check for console commands
	s = Sys_ConsoleInput();
	if( s )
	{
		char*	b;
		int		len;

		len = strlen( s ) + 1;
		b = ( char* )Mem_Alloc( len );
		strcpy( b, s );
		Sys_QueEvent( SE_CONSOLE, 0, 0, len, b, 0 );
	}

	entered = false;
}

/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents()
{
	eventHead = eventTail = 0;
}

/*
================
Sys_GetEvent
================
*/
sysEvent_t Sys_GetEvent()
{
	sysEvent_t	ev;

	// return if we have data
	if( eventHead > eventTail )
	{
		eventTail++;
		return eventQue[( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// return the empty event
	memset( &ev, 0, sizeof( ev ) );

	return ev;
}

//================================================================

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( const idCmdArgs& args )
{
	Sys_ShutdownInput();
	Sys_InitInput();
}


/*
==================
Sys_AsyncThread
==================
*/
static void Sys_AsyncThread( void* parm )
{
	int		wakeNumber;
	int		startTime;

	startTime = Sys_Milliseconds();
	wakeNumber = 0;

	while( 1 )
	{
#ifdef WIN32
		// this will trigger 60 times a second
		int r = WaitForSingleObject( hTimer, 100 );
		if( r != WAIT_OBJECT_0 )
		{
			OutputDebugString( "idPacketServer::PacketServerInterrupt: bad wait return" );
		}
#endif

#if 0
		wakeNumber++;
		int		msec = Sys_Milliseconds();
		int		deltaTime = msec - startTime;
		startTime = msec;

		char	str[1024];
		sprintf( str, "%i ", deltaTime );
		OutputDebugString( str );
#endif


		common->Async();
	}
}

/*
==============
Sys_StartAsyncThread

Start the thread that will call idCommon::Async()
==============
*/
void Sys_StartAsyncThread()
{
	// create an auto-reset event that happens 60 times a second
	hTimer = CreateWaitableTimer( NULL, false, NULL );
	if( !hTimer )
	{
		common->Error( "idPacketServer::Spawn: CreateWaitableTimer failed" );
	}

	LARGE_INTEGER	t;
	t.HighPart = t.LowPart = 0;
	SetWaitableTimer( hTimer, &t, USERCMD_MSEC, NULL, NULL, TRUE );

	Sys_CreateThread( ( xthread_t )Sys_AsyncThread, NULL, THREAD_ABOVE_NORMAL, threadInfo, "Async", g_threads,  &g_thread_count );

#ifdef SET_THREAD_AFFINITY
	// give the async thread an affinity for the second cpu
	SetThreadAffinityMask( ( HANDLE )threadInfo.threadHandle, 2 );
#endif

	if( !threadInfo.threadHandle )
	{
		common->Error( "Sys_StartAsyncThread: failed" );
	}
}

/*
================
Sys_Init

The cvar system must already be setup
================
*/
void Sys_Init()
{

	CoInitialize( NULL );

	// make sure the timer is high precision, otherwise
	// NT gets 18ms resolution
	timeBeginPeriod( 1 );

	// get WM_TIMER messages pumped every millisecond
//	SetTimer( NULL, 0, 100, NULL );

	cmdSystem->AddCommand( "in_restart", Sys_In_Restart_f, CMD_FL_SYSTEM, "restarts the input system" );
#ifdef DEBUG
	cmdSystem->AddCommand( "createResourceIDs", CreateResourceIDs_f, CMD_FL_TOOL, "assigns resource IDs in _resouce.h files" );
#endif
#if 0
	cmdSystem->AddCommand( "setAsyncSound", Sys_SetAsyncSound_f, CMD_FL_SYSTEM, "set the async sound option" );
#endif

	//
	// Windows version
	//
	win32.osversion.dwOSVersionInfoSize = sizeof( win32.osversion );

	if( !GetVersionEx( ( LPOSVERSIONINFO )&win32.osversion ) )
	{
		Sys_Error( "Couldn't get OS info" );
	}

	if( win32.osversion.dwMajorVersion < 4 )
	{
		Sys_Error( GAME_NAME " requires Windows version 4 (NT) or greater" );
	}
	if( win32.osversion.dwPlatformId == VER_PLATFORM_WIN32s )
	{
		Sys_Error( GAME_NAME " doesn't run on Win32s" );
	}

	//
	// CPU type
	//
	if( !idStr::Icmp( win32.sys_cpustring.GetString(), "detect" ) )
	{
		idStr string;

		common->Printf( "%1.0f MHz ", Sys_ClockTicksPerSecond() / 1000000.0f );

		win32.cpuid = Sys_GetCPUId();

		string.Clear();

		if( win32.cpuid & CPUID_AMD )
		{
			string += "AMD CPU";
		}
		else if( win32.cpuid & CPUID_INTEL )
		{
			string += "Intel CPU";
		}
		else if( win32.cpuid & CPUID_UNSUPPORTED )
		{
			string += "unsupported CPU";
		}
		else
		{
			string += "generic CPU";
		}

		string += " with ";
		if( win32.cpuid & CPUID_MMX )
		{
			string += "MMX & ";
		}
		if( win32.cpuid & CPUID_3DNOW )
		{
			string += "3DNow! & ";
		}
		if( win32.cpuid & CPUID_SSE )
		{
			string += "SSE & ";
		}
		if( win32.cpuid & CPUID_SSE2 )
		{
			string += "SSE2 & ";
		}
		if( win32.cpuid & CPUID_SSE3 )
		{
			string += "SSE3 & ";
		}
		if( win32.cpuid & CPUID_HTT )
		{
			string += "HTT & ";
		}
		string.StripTrailing( " & " );
		string.StripTrailing( " with " );
		win32.sys_cpustring.SetString( string );
	}
	else
	{
		common->Printf( "forcing CPU type to " );
		idLexer src( win32.sys_cpustring.GetString(), idStr::Length( win32.sys_cpustring.GetString() ), "sys_cpustring" );
		idToken token;

		int id = CPUID_NONE;
		while( src.ReadToken( &token ) )
		{
			if( token.Icmp( "generic" ) == 0 )
			{
				id |= CPUID_GENERIC;
			}
			else if( token.Icmp( "intel" ) == 0 )
			{
				id |= CPUID_INTEL;
			}
			else if( token.Icmp( "amd" ) == 0 )
			{
				id |= CPUID_AMD;
			}
			else if( token.Icmp( "mmx" ) == 0 )
			{
				id |= CPUID_MMX;
			}
			else if( token.Icmp( "3dnow" ) == 0 )
			{
				id |= CPUID_3DNOW;
			}
			else if( token.Icmp( "sse" ) == 0 )
			{
				id |= CPUID_SSE;
			}
			else if( token.Icmp( "sse2" ) == 0 )
			{
				id |= CPUID_SSE2;
			}
			else if( token.Icmp( "sse3" ) == 0 )
			{
				id |= CPUID_SSE3;
			}
			else if( token.Icmp( "htt" ) == 0 )
			{
				id |= CPUID_HTT;
			}
		}
		if( id == CPUID_NONE )
		{
			common->Printf( "WARNING: unknown sys_cpustring '%s'\n", win32.sys_cpustring.GetString() );
			id = CPUID_GENERIC;
		}
		win32.cpuid = ( cpuid_t ) id;
	}

	common->Printf( "%s\n", win32.sys_cpustring.GetString() );
	common->Printf( "%d MB System Memory\n", Sys_GetSystemRam() );
	if( ( win32.cpuid & CPUID_SSE2 ) == 0 )
	{
		common->Error( "SSE2 not supported!" );
	}

	win32.g_Joystick.Init();
}

/*
================
Sys_Shutdown
================
*/
void Sys_Shutdown()
{
	CoUninitialize();
}

/*
================
Sys_GetProcessorId
================
*/
cpuid_t Sys_GetProcessorId()
{
	return win32.cpuid;
}

/*
================
Sys_GetProcessorString
================
*/
const char* Sys_GetProcessorString()
{
	return win32.sys_cpustring.GetString();
}

//=======================================================================

//#define SET_THREAD_AFFINITY


/*
====================
Win_Frame
====================
*/
void Win_Frame()
{
	// if "viewlog" has been modified, show or hide the log console
	if( win32.win_viewlog.IsModified() )
	{
		if( !com_skipRenderer.GetBool() && idAsyncNetwork::serverDedicated.GetInteger() != 1 )
		{
			Sys_ShowConsole( win32.win_viewlog.GetInteger(), false );
		}
		win32.win_viewlog.ClearModified();
	}
}

// code that tells windows we're High DPI aware so it doesn't scale our windows
// taken from Yamagi Quake II

typedef enum D3_PROCESS_DPI_AWARENESS
{
	D3_PROCESS_DPI_UNAWARE = 0,
	D3_PROCESS_SYSTEM_DPI_AWARE = 1,
	D3_PROCESS_PER_MONITOR_DPI_AWARE = 2
} YQ2_PROCESS_DPI_AWARENESS;

static void setHighDPIMode( void )
{
	/* For Vista, Win7 and Win8 */
	BOOL( WINAPI * SetProcessDPIAware )( void ) = NULL;

	/* Win8.1 and later */
	HRESULT( WINAPI * SetProcessDpiAwareness )( D3_PROCESS_DPI_AWARENESS dpiAwareness ) = NULL;


	HINSTANCE userDLL = LoadLibrary( "USER32.DLL" );

	if( userDLL )
	{
		SetProcessDPIAware = ( BOOL( WINAPI* )( void ) ) GetProcAddress( userDLL, "SetProcessDPIAware" );
	}


	HINSTANCE shcoreDLL = LoadLibrary( "SHCORE.DLL" );

	if( shcoreDLL )
	{
		SetProcessDpiAwareness = ( HRESULT( WINAPI* )( YQ2_PROCESS_DPI_AWARENESS ) )
								 GetProcAddress( shcoreDLL, "SetProcessDpiAwareness" );
	}


	if( SetProcessDpiAwareness )
	{
		SetProcessDpiAwareness( D3_PROCESS_PER_MONITOR_DPI_AWARE );
	}
	else if( SetProcessDPIAware )
	{
		SetProcessDPIAware();
	}
}

/*
==================
WinMain
==================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

#ifdef ID_DEDICATED
	MSG msg;
#else
	// tell windows we're high dpi aware, otherwise display scaling screws up the game
	setHighDPIMode();
#endif

	Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );

	win32.hInstance = hInstance;
	idStr::Copynz( sys_cmdline, lpCmdLine, sizeof( sys_cmdline ) );

	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();

	// no abort/retry/fail errors
	SetErrorMode( SEM_FAILCRITICALERRORS );

// RB begin
	Sys_InitCriticalSections();
// RB end
	// make sure the timer is high precision, otherwise
	// NT gets 18ms resolution
	timeBeginPeriod( 1 );

	// get the initial time base
	Sys_Milliseconds();

#ifdef DEBUG
	// disable the painfully slow MS heap check every 1024 allocs
	_CrtSetDbgFlag( 0 );
#endif

	common->Init( 0, NULL );

#ifndef	ID_DEDICATED
	if( win32.win_notaskkeys.GetInteger() )
	{
		DisableTaskKeys( TRUE, FALSE, /*( win32.win_notaskkeys.GetInteger() == 2 )*/ FALSE );
	}
#endif

	Sys_StartAsyncThread();

	// hide or show the early console as necessary
	if( win32.win_viewlog.GetInteger() || com_skipRenderer.GetBool() || idAsyncNetwork::serverDedicated.GetInteger() )
	{
		Sys_ShowConsole( 1, true );
	}
	else
	{
		Sys_ShowConsole( 0, false );
	}

#ifdef SET_THREAD_AFFINITY
	// give the main thread an affinity for the first cpu
	SetThreadAffinityMask( GetCurrentThread(), 1 );
#endif

	::SetCursor( hcurSave );

	// Launch the script debugger
	if( strstr( lpCmdLine, "+debugger" ) )
	{

#ifdef ID_ALLOW_TOOLS
		DebuggerClientInit( lpCmdLine );
#endif
		return 0;
	}

	::SetFocus( win32.hWnd );

	// main game loop
	while( 1 )
	{
#if ID_DEDICATED
		// Since this is a Dedicated Server, process all Windowing Messages
		// Now.
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// Give the OS a little time to recuperate.
		Sleep( 10 );
#endif

		Win_Frame();

#ifdef ID_ALLOW_TOOLS
		if( com_editors )
		{
			if( com_editors & EDITOR_GUI )
			{
				// GUI editor
				GUIEditorRun();
			}
			else if( com_editors & EDITOR_RADIANT )
			{
				// Level Editor
				RadiantRun();
			}
			else if( com_editors & EDITOR_MATERIAL )
			{
				//BSM Nerve: Add support for the material editor
				MaterialEditorRun();
			}
			else
			{
				if( com_editors & EDITOR_LIGHT )
				{
					// in-game Light Editor
					LightEditorRun();
				}
				if( com_editors & EDITOR_SOUND )
				{
					// in-game Sound Editor
					SoundEditorRun();
				}
				if( com_editors & EDITOR_DECL )
				{
					// in-game Declaration Browser
					DeclBrowserRun();
				}
				if( com_editors & EDITOR_AF )
				{
					// in-game Articulated Figure Editor
					AFEditorRun();
				}
				if( com_editors & EDITOR_PARTICLE )
				{
					// in-game Particle Editor
					ParticleEditorRun();
				}
				if( com_editors & EDITOR_SCRIPT )
				{
					// in-game Script Editor
					ScriptEditorRun();
				}
				if( com_editors & EDITOR_PDA )
				{
					// in-game PDA Editor
					PDAEditorRun();
				}
			}
		}
#endif
		// run the game
		common->Frame();
	}

	// never gets here
	return 0;
}

/*
==================
idSysLocal::OpenURL
==================
*/
void idSysLocal::OpenURL( const char* url, bool doexit )
{
	static bool doexit_spamguard = false;
	HWND wnd;

	if( doexit_spamguard )
	{
		common->DPrintf( "OpenURL: already in an exit sequence, ignoring %s\n", url );
		return;
	}

	common->Printf( "Open URL: %s\n", url );

	if( !ShellExecute( NULL, "open", url, NULL, NULL, SW_RESTORE ) )
	{
		common->Error( "Could not open url: '%s' ", url );
		return;
	}

	wnd = GetForegroundWindow();
	if( wnd )
	{
		ShowWindow( wnd, SW_MAXIMIZE );
	}

	if( doexit )
	{
		doexit_spamguard = true;
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
	}
}

/*
==================
idSysLocal::StartProcess
==================
*/
void idSysLocal::StartProcess( const char* exePath, bool doexit )
{
	TCHAR				szPathOrig[_MAX_PATH];
	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;

	ZeroMemory( &si, sizeof( si ) );
	si.cb = sizeof( si );

	strncpy( szPathOrig, exePath, _MAX_PATH );
	szPathOrig[_MAX_PATH - 1] = 0;

	if( !CreateProcess( NULL, szPathOrig, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
	{
		common->Error( "Could not start process: '%s' ", szPathOrig );
		return;
	}

	if( doexit )
	{
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
	}
}
