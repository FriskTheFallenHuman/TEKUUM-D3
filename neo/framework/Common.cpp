/*
===========================================================================

KROOM 3 GPL Source Code

This file is part of the KROOM 3 Source Code, originally based
on the Doom 3 with bits and pieces from Doom 3 BFG edition GPL Source Codes both published in 2011 and 2012.

KROOM 3 Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Extra attributions can be found on the CREDITS.txt file

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "Common_local.h"

#include "../renderer/Image.h"
#include "ConsoleHistory.h"

#if defined( _DEBUG )
	#define BUILD_DEBUG "-debug"
#else
	#define BUILD_DEBUG ""
#endif

struct version_s
{
	version_s()
	{
		sprintf( string, "%s.%d%s %s-%s %s %s", ENGINE_VERSION, BUILD_NUMBER, BUILD_DEBUG, OSTYPE, CPUSTRING, ID__DATE__, ID__TIME__ );
	}
	char	string[256];
} version;

idCVar com_version( "si_version", version.string, CVAR_SYSTEM | CVAR_ROM | CVAR_SERVERINFO, "engine version" );
idCVar com_skipRenderer( "com_skipRenderer", "0", CVAR_BOOL | CVAR_SYSTEM, "skip the renderer completely" );
idCVar com_purgeAll( "com_purgeAll", "0", CVAR_BOOL | CVAR_ARCHIVE | CVAR_SYSTEM, "purge everything between level loads" );
idCVar com_memoryMarker( "com_memoryMarker", "-1", CVAR_INTEGER | CVAR_SYSTEM | CVAR_INIT, "used as a marker for memory stats" );
idCVar com_preciseTic( "com_preciseTic", "1", CVAR_BOOL | CVAR_SYSTEM, "run one game tick every async thread update" );
idCVar com_asyncInput( "com_asyncInput", "0", CVAR_BOOL | CVAR_SYSTEM, "sample input from the async thread" );
idCVar com_forceGenericSIMD( "com_forceGenericSIMD", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "force generic platform independent SIMD" );
idCVar com_developer( "developer", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "developer mode" );
idCVar com_allowConsole( "com_allowConsole", "1", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "allow toggling console with the tilde key" );
idCVar com_speeds( "com_speeds", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "show engine timings" );
idCVar com_showFPS( "com_showFPS", "0", CVAR_INTEGER | CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_NOCHEAT, "show frames rendered per second. 0: off 1: default bfg values, 2: only show FPS (classic view)" );
idCVar com_showMemoryUsage( "com_showMemoryUsage", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "show total and per frame memory usage" );
idCVar com_showAsyncStats( "com_showAsyncStats", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "show async network stats" );
idCVar com_showSoundDecoders( "com_showSoundDecoders", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "show sound decoders" );
idCVar com_timescale( "timescale", "1", CVAR_SYSTEM | CVAR_FLOAT, "scales the time", 0.1f, 10.0f );
idCVar com_makingBuild( "com_makingBuild", "0", CVAR_BOOL | CVAR_SYSTEM, "1 when making a build" );
idCVar com_updateLoadSize( "com_updateLoadSize", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "update the load size after loading a map" );
idCVar com_enableDebuggerServer( "com_enableDebuggerServer", "0", CVAR_BOOL | CVAR_SYSTEM, "toggle debugger server and try to connect to com_dbgClientAdr" );
idCVar com_dbgClientAdr( "com_dbgClientAdr", "localhost", CVAR_SYSTEM | CVAR_ARCHIVE, "debuggerApp client address" );
idCVar com_dbgServerAdr( "com_dbgServerAdr", "localhost", CVAR_SYSTEM | CVAR_ARCHIVE, "debugger server address" );
idCVar com_productionMode( "com_productionMode", "0", CVAR_SYSTEM | CVAR_BOOL, "0 - no special behavior, 1 - building a production build, 2 - running a production build" );

// com_speeds times
int				com_frameMsec;

int				time_gameFrame;
int				time_gameDraw;

int				time_lastGameFrame;
int				time_lastGameDraw;

uint64_t			time_frontend;			// renderSystem frontend time
uint64_t			time_backend;			// renderSystem backend time
uint64_t			time_shadows;			// renderer backend waiting for shadow volumes to be created
uint64_t			time_gpu;				// total gpu time, at least for PC in microseconds

int				com_frameTime;			// time for the current frame in milliseconds
int				com_frameNumber;		// variable frame number
volatile int	com_ticNumber;			// 60 hz tics
int				com_editors;			// currently opened editor(s)
bool			com_editorActive;		//  true if an editor has focus

#ifdef __DOOM_DLL__
	idGame* 		game = NULL;
	idGameEdit* 	gameEdit = NULL;
#endif

// writes si_version to the config file - in a kinda obfuscated way
//#define ID_WRITE_VERSION

idCommonLocal	commonLocal;
idCommon* 		common = &commonLocal;


/*
==================
idCommonLocal::idCommonLocal
==================
*/
idCommonLocal::idCommonLocal()
{
	com_fullyInitialized = false;
	com_refreshOnPrint = false;
	com_errorEntered = 0;
	com_shuttingDown = false;

	logFile = NULL;

	strcpy( errorMessage, "" );

	rd_buffer = NULL;
	rd_buffersize = 0;
	rd_flush = NULL;

	gameDLL = 0;

#ifdef ID_WRITE_VERSION
	config_compressor = NULL;
#endif
}

/*
==================
idCommonLocal::Quit
==================
*/
void idCommonLocal::Quit()
{

#ifdef ID_ALLOW_TOOLS
	if( com_editors & EDITOR_RADIANT )
	{
		RadiantInit();
		return;
	}
#endif

	// don't try to shutdown if we are in a recursive error
	if( !com_errorEntered )
	{
		Shutdown();
	}

	Sys_Quit();
}


/*
============================================================================

COMMAND LINE FUNCTIONS

+ characters separate the commandLine string into multiple console
command lines.

All of these are valid:

doom +set test blah +map test
doom set test blah+map test
doom set test blah + map test

============================================================================
*/

#define		MAX_CONSOLE_LINES	32
int			com_numConsoleLines;
idCmdArgs	com_consoleLines[MAX_CONSOLE_LINES];

/*
==================
idCommonLocal::ParseCommandLine
==================
*/
void idCommonLocal::ParseCommandLine( int argc, char** argv )
{
	int i;

	com_numConsoleLines = 0;
	// API says no program path
	for( i = 0; i < argc; i++ )
	{
		if( argv[ i ][ 0 ] == '+' )
		{
			com_numConsoleLines++;
			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] + 1 );
		}
		else
		{
			if( !com_numConsoleLines )
			{
				com_numConsoleLines++;
			}
			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] );
		}
	}
}

/*
==================
idCommonLocal::ClearCommandLine
==================
*/
void idCommonLocal::ClearCommandLine()
{
	com_numConsoleLines = 0;
}

/*
==================
idCommonLocal::SafeMode

Check for "safe" on the command line, which will
skip loading of config file (DoomConfig.cfg)
==================
*/
bool idCommonLocal::SafeMode()
{
	int			i;

	for( i = 0 ; i < com_numConsoleLines ; i++ )
	{
		if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "safe" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "cvar_restart" ) )
		{
			com_consoleLines[ i ].Clear();
			return true;
		}
	}
	return false;
}

/*
==================
idCommonLocal::CheckToolMode

Check for "renderbump", "dmap", or "editor" on the command line,
and force fullscreen off in those cases
==================
*/
void idCommonLocal::CheckToolMode()
{
	int			i;

	for( i = 0 ; i < com_numConsoleLines ; i++ )
	{
		if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "guieditor" ) )
		{
			com_editors |= EDITOR_GUI;
		}
		else if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "debugger" ) )
		{
			com_editors |= EDITOR_DEBUGGER;
		}
		else if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "editor" ) )
		{
			com_editors |= EDITOR_RADIANT;
		}
		// Nerve: Add support for the material editor
		else if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "materialEditor" ) )
		{
			com_editors |= EDITOR_MATERIAL;
		}

		if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "renderbump" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "editor" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "guieditor" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "debugger" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "dmap" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "materialEditor" )
		  )
		{
			cvarSystem->SetCVarBool( "r_fullscreen", false );
			return;
		}
	}
}

/*
==================
idCommonLocal::StartupVariable

Searches for command line parameters that are set commands.
If match is not NULL, only that cvar will be looked for.
That is necessary because cddir and basedir need to be set
before the filesystem is started, but all other sets should
be after execing the config and default.
==================
*/
void idCommonLocal::StartupVariable( const char* match, bool once )
{
	int i = 0;
	while(	i < com_numConsoleLines )
	{
		if( strcmp( com_consoleLines[ i ].Argv( 0 ), "set" ) )
		{
			i++;
			continue;
		}
		const char* s = com_consoleLines[ i ].Argv( 1 );

		if( !match || !idStr::Icmp( s, match ) )
		{
			cvarSystem->SetCVarString( s, com_consoleLines[ i ].Argv( 2 ) );
			if( once )
			{
				// kill the line
				int j = i + 1;
				while( j < com_numConsoleLines )
				{
					com_consoleLines[ j - 1 ] = com_consoleLines[ j ];
					j++;
				}
				com_numConsoleLines--;
				continue;
			}
		}
		i++;
	}
}

/*
==================
idCommonLocal::AddStartupCommands

Adds command line parameters as script statements
Commands are separated by + signs

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
==================
*/
bool idCommonLocal::AddStartupCommands()
{
	int		i;
	bool	added;

	added = false;
	// quote every token, so args with semicolons can work
	for( i = 0; i < com_numConsoleLines; i++ )
	{
		if( !com_consoleLines[i].Argc() )
		{
			continue;
		}

		// set commands won't override menu startup
		if( idStr::Icmpn( com_consoleLines[i].Argv( 0 ), "set", 3 ) )
		{
			added = true;
		}
		// directly as tokenized so nothing gets screwed
		cmdSystem->BufferCommandArgs( CMD_EXEC_APPEND, com_consoleLines[i] );
	}

	return added;
}

/*
=================
idCommonLocal::InitTool
=================
*/
void idCommonLocal::InitTool( const toolFlag_t tool, const idDict* dict )
{
#ifdef ID_ALLOW_TOOLS
	if( tool & EDITOR_SOUND )
	{
		SoundEditorInit( dict );
	}
	else if( tool & EDITOR_LIGHT )
	{
		LightEditorInit( dict );
	}
	else if( tool & EDITOR_PARTICLE )
	{
		ParticleEditorInit( dict );
	}
	else if( tool & EDITOR_AF )
	{
		AFEditorInit( dict );
	}
#endif
}

/*
==================
idCommonLocal::ActivateTool

Activates or Deactivates a tool
==================
*/
void idCommonLocal::ActivateTool( bool active )
{
	com_editorActive = active;
	Sys_GrabMouseCursor( !active );
}

/*
==================
idCommonLocal::WriteFlaggedCVarsToFile
==================
*/
void idCommonLocal::WriteFlaggedCVarsToFile( const char* filename, int flags, const char* setCmd )
{
	idFile* f;

	f = fileSystem->OpenFileWrite( filename, "fs_configpath" );
	if( !f )
	{
		Printf( "Couldn't write %s.\n", filename );
		return;
	}
	cvarSystem->WriteFlaggedVariables( flags, setCmd, f );
	fileSystem->CloseFile( f );
}

/*
==================
idCommonLocal::WriteConfigToFile
==================
*/
void idCommonLocal::WriteConfigToFile( const char* filename )
{
	idFile* f;
#ifdef ID_WRITE_VERSION
	ID_TIME_T t;
	char* curtime;
	idStr runtag;
	idFile_Memory compressed( "compressed" );
	idBase64 out;
#endif

	f = fileSystem->OpenFileWrite( filename, "fs_configpath" );
	if( !f )
	{
		Printf( "Couldn't write %s.\n", filename );
		return;
	}

#ifdef ID_WRITE_VERSION
	assert( config_compressor );
	t = time( NULL );
	curtime = ctime( &t );
	sprintf( runtag, "%s - %s", cvarSystem->GetCVarString( "si_version" ), curtime );
	config_compressor->Init( &compressed, true, 8 );
	config_compressor->Write( runtag.c_str(), runtag.Length() );
	config_compressor->FinishCompress( );
	out.Encode( ( const byte* )compressed.GetDataPtr(), compressed.Length() );
	f->Printf( "// %s\n", out.c_str() );
#endif

	idKeyInput::WriteBindings( f );
	cvarSystem->WriteFlaggedVariables( CVAR_ARCHIVE, "seta", f );
	fileSystem->CloseFile( f );
}

/*
===============
idCommonLocal::WriteConfiguration

Writes key bindings and archived cvars to config file if modified
===============
*/
void idCommonLocal::WriteConfiguration()
{
	// if we are quiting without fully initializing, make sure
	// we don't write out anything
	if( !com_fullyInitialized )
	{
		return;
	}

	if( !( cvarSystem->GetModifiedFlags() & CVAR_ARCHIVE ) )
	{
		return;
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );

	// disable printing out the "Writing to:" message
	bool developer = com_developer.GetBool();
	com_developer.SetBool( false );

	WriteConfigToFile( CONFIG_FILE );
	session->WriteCDKey( );

	// restore the developer cvar
	com_developer.SetBool( developer );
}

/*
===============
KeysFromBinding()
Returns the key bound to the command
===============
*/
const char* idCommonLocal::KeysFromBinding( const char* bind )
{
	return idKeyInput::KeysFromBinding( bind );
}

/*
===============
BindingFromKey()
Returns the binding bound to key
===============
*/
const char* idCommonLocal::BindingFromKey( const char* key )
{
	return idKeyInput::BindingFromKey( key );
}

/*
===============
ButtonState()
Returns the state of the button
===============
*/
int	idCommonLocal::ButtonState( int key )
{
	return usercmdGen->ButtonState( key );
}

/*
===============
ButtonState()
Returns the state of the key
===============
*/
int	idCommonLocal::KeyState( int key )
{
	return usercmdGen->KeyState( key );
}

//============================================================================

#ifdef ID_ALLOW_TOOLS
/*
==================
Com_Editor_f

  we can start the editor dynamically, but we won't ever get back
==================
*/
static void Com_Editor_f( const idCmdArgs& args )
{
	RadiantInit();
}

/*
=============
Com_ScriptDebugger_f
=============
*/
static void Com_ScriptDebugger_f( const idCmdArgs& args )
{
	// Make sure it wasnt on the command line
	if( !( com_editors & EDITOR_DEBUGGER ) )
	{
		DebuggerClientLaunch();
	}
}

/*
=============
Com_EditGUIs_f
=============
*/
static void Com_EditGUIs_f( const idCmdArgs& args )
{
	GUIEditorInit();
}

/*
=============
Com_MaterialEditor_f
=============
*/
static void Com_MaterialEditor_f( const idCmdArgs& args )
{
	// Turn off sounds
	soundSystem->SetMute( true );
	MaterialEditorInit();
}
#endif // ID_ALLOW_TOOLS

CONSOLE_COMMAND( testFormattingSizes, "test printf format security", 0 )
{
#ifdef _MSC_VER
	common->Printf( " sizeof( int32_t ): %Iu bytes\n", sizeof( int32_t ) );
	common->Printf( " sizeof( int64_t ): %Iu bytes\n", sizeof( int64_t ) );
#else
	common->Printf( " sizeof( int32_t ): %zu bytes\n", sizeof( int32_t ) );
	common->Printf( " sizeof( int64_t ): %zu bytes\n", sizeof( int64_t ) );
#endif
}

// RB end

/*
============
idCmdSystemLocal::PrintMemInfo_f

This prints out memory debugging data
============
*/
static void PrintMemInfo_f( const idCmdArgs& args )
{
	MemInfo_t mi;

	memset( &mi, 0, sizeof( mi ) );
	mi.filebase = session->GetCurrentMapName();

	renderSystem->PrintMemInfo( &mi );			// textures and models
	soundSystem->PrintMemInfo( &mi );			// sounds

	common->Printf( " Used image memory: %s bytes\n", idStr::FormatNumber( mi.imageAssetsTotal ).c_str() );
	mi.assetTotals += mi.imageAssetsTotal;

	common->Printf( " Used model memory: %s bytes\n", idStr::FormatNumber( mi.modelAssetsTotal ).c_str() );
	mi.assetTotals += mi.modelAssetsTotal;

	common->Printf( " Used sound memory: %s bytes\n", idStr::FormatNumber( mi.soundAssetsTotal ).c_str() );
	mi.assetTotals += mi.soundAssetsTotal;

	common->Printf( " Used asset memory: %s bytes\n", idStr::FormatNumber( mi.assetTotals ).c_str() );

	// write overview file
	idFile* f;

	f = fileSystem->OpenFileAppend( "maps/printmeminfo.txt" );
	if( !f )
	{
		return;
	}

	f->Printf( "total(%s ) image(%s ) model(%s ) sound(%s ): %s\n", idStr::FormatNumber( mi.assetTotals ).c_str(), idStr::FormatNumber( mi.imageAssetsTotal ).c_str(),
			   idStr::FormatNumber( mi.modelAssetsTotal ).c_str(), idStr::FormatNumber( mi.soundAssetsTotal ).c_str(), mi.filebase.c_str() );

	fileSystem->CloseFile( f );
}

#ifdef ID_ALLOW_TOOLS
/*
==================
Com_EditLights_f
==================
*/
static void Com_EditLights_f( const idCmdArgs& args )
{
	LightEditorInit( NULL );
	cvarSystem->SetCVarInteger( "g_editEntityMode", 1 );
}

/*
==================
Com_EditSounds_f
==================
*/
static void Com_EditSounds_f( const idCmdArgs& args )
{
	SoundEditorInit( NULL );
	cvarSystem->SetCVarInteger( "g_editEntityMode", 2 );
}

/*
==================
Com_EditDecls_f
==================
*/
static void Com_EditDecls_f( const idCmdArgs& args )
{
	DeclBrowserInit( NULL );
}

/*
==================
Com_EditAFs_f
==================
*/
static void Com_EditAFs_f( const idCmdArgs& args )
{
	AFEditorInit( NULL );
}

/*
==================
Com_EditParticles_f
==================
*/
static void Com_EditParticles_f( const idCmdArgs& args )
{
	ParticleEditorInit( NULL );
}

/*
==================
Com_EditScripts_f
==================
*/
static void Com_EditScripts_f( const idCmdArgs& args )
{
	ScriptEditorInit( NULL );
}

/*
==================
Com_EditPDAs_f
==================
*/
static void Com_EditPDAs_f( const idCmdArgs& args )
{
	PDAEditorInit( NULL );
}
#endif // ID_ALLOW_TOOLS

/*
==================
Com_Error_f

Just throw a fatal error to test error shutdown procedures.
==================
*/
CONSOLE_COMMAND( error, "causes an error", NULL )
{
	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "error may only be used in developer mode\n" );
		return;
	}

	if( args.Argc() > 1 )
	{
		commonLocal.FatalError( "Testing fatal error" );
	}
	else
	{
		commonLocal.Error( "Testing drop error" );
	}
}

/*
==================
Com_Freeze_f

Just freeze in place for a given number of seconds to test error recovery.
==================
*/
CONSOLE_COMMAND( freeze, "freezes the game for a number of seconds", NULL )
{
	float	s;
	int		start, now;

	if( args.Argc() != 2 )
	{
		commonLocal.Printf( "freeze <seconds>\n" );
		return;
	}

	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "freeze may only be used in developer mode\n" );
		return;
	}

	s = atof( args.Argv( 1 ) );

	start = eventLoop->Milliseconds();

	while( 1 )
	{
		now = eventLoop->Milliseconds();
		if( ( now - start ) * 0.001f > s )
		{
			break;
		}
	}
}

/*
=================
Com_Crash_f

A way to force a bus error for development reasons
=================
*/
CONSOLE_COMMAND( crash, "causes a crash", NULL )
{
	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "crash may only be used in developer mode\n" );
		return;
	}
#ifdef __GNUC__
	__builtin_trap();
#else
	* ( int* ) 0 = 0x12345678;
#endif
}

/*
=================
Com_Quit_f
=================
*/
CONSOLE_COMMAND_SHIP( quit, "quits the game", NULL )
{
	commonLocal.Quit();
}
CONSOLE_COMMAND_SHIP( exit, "exits the game", NULL )
{
	commonLocal.Quit();
}

/*
===============
Com_WriteConfig_f

Write the config file to a specific name
===============
*/
CONSOLE_COMMAND( writeConfig, "writes a config file", NULL )
{
	idStr	filename;

	if( args.Argc() != 2 )
	{
		commonLocal.Printf( "Usage: writeconfig <filename>\n" );
		return;
	}

	filename = args.Argv( 1 );
	filename.DefaultFileExtension( ".cfg" );
	commonLocal.Printf( "Writing %s.\n", filename.c_str() );
	commonLocal.WriteConfigToFile( filename );
}

/*
=================
Com_ReloadEngine_f
=================
*/
void Com_ReloadEngine_f( const idCmdArgs& args )
{
	bool menu = false;

	if( !commonLocal.IsInitialized() )
	{
		return;
	}

	if( args.Argc() > 1 && idStr::Icmp( args.Argv( 1 ), "menu" ) == 0 )
	{
		menu = true;
	}

	common->Printf( "============= ReloadEngine start =============\n" );
	if( !menu )
	{
		Sys_ShowConsole( 1, false );
	}
	commonLocal.ShutdownGame( true );
	commonLocal.InitGame();
	if( !menu && !idAsyncNetwork::serverDedicated.GetBool() )
	{
		Sys_ShowConsole( 0, false );
	}
	common->Printf( "============= ReloadEngine end ===============\n" );

	if( !cmdSystem->PostReloadEngine() )
	{
		if( menu )
		{
			session->StartMenu( );
		}
	}
}

// RB: development tool
static void Com_GenerateSinCosTables_f( const idCmdArgs& args )
{
	int i;
	float angle;
	int numSteps = 180;

	idFile* file = fileSystem->OpenFileByMode( "materials/generated.mtr", FS_WRITE );

	file->Printf( "table sinTable { {\n" );
	for( i = 0, angle = 0; i < numSteps; i++, angle += ( ( 2 * idMath::PI ) / ( float ) numSteps ) )
	{
		if( i % 10 == 0 )
		{
			file->Printf( "\n" );
		}

		file->Printf( "%f", idMath::Sin( angle ) );

		if( i < numSteps - 1 )
		{
			file->Printf( "," );
		}
	}
	file->Printf( " } }\n" );

	file->Printf( "table cosTable { {\n" );
	for( i = 0, angle = 0; i < numSteps; i++, angle += ( ( 2 * idMath::PI ) / ( float ) numSteps ) )
	{
		if( i % 10 == 0 )
		{
			file->Printf( "\n" );
		}

		file->Printf( "%f", idMath::Cos( angle ) );

		if( i < numSteps - 1 )
		{
			file->Printf( "," );
		}
	}
	file->Printf( " } }\n" );

	fileSystem->CloseFile( file );
}

/*
===============
idCommonLocal::GetLanguageDict
===============
*/
const idLangDict* idCommonLocal::GetLanguageDict()
{
	return &languageDict;
}

/*
===============
idCommonLocal::FilterLangList
===============
*/
void idCommonLocal::FilterLangList( idStrList* list, idStr lang )
{

	idStr temp;
	for( int i = 0; i < list->Num(); i++ )
	{
		temp = ( *list )[i];
		temp = temp.Right( temp.Length() - strlen( "strings/" ) );
		temp = temp.Left( lang.Length() );
		if( idStr::Icmp( temp, lang ) != 0 )
		{
			list->RemoveIndex( i );
			i--;
		}
	}
}

/*
===============
idCommonLocal::InitLanguageDict
===============
*/
void idCommonLocal::InitLanguageDict()
{
	idStr fileName;
	languageDict.Clear();

	//D3XP: Instead of just loading a single lang file for each language
	//we are going to load all files that begin with the language name
	//similar to the way pak files work. So you can place english001.lang
	//to add new strings to the english language dictionary
	idFileList*	langFiles;
	langFiles =  fileSystem->ListFilesTree( "strings", ".lang", true );

	idStrList langList = langFiles->GetList();

	StartupVariable( "sys_lang", false );	// let it be set on the command line - this is needed because this init happens very early
	idStr langName = cvarSystem->GetCVarString( "sys_lang" );

	//Loop through the list and filter
	idStrList currentLangList = langList;
	FilterLangList( &currentLangList, langName );

	if( currentLangList.Num() == 0 )
	{
		// reset cvar to default and try to load again
		cmdSystem->BufferCommandText( CMD_EXEC_NOW, "reset sys_lang" );
		langName = cvarSystem->GetCVarString( "sys_lang" );
		currentLangList = langList;
		FilterLangList( &currentLangList, langName );
	}

	for( int i = 0; i < currentLangList.Num(); i++ )
	{
		//common->Printf("%s\n", currentLangList[i].c_str());
		languageDict.Load( currentLangList[i], false );
	}

	fileSystem->FreeFileList( langFiles );
}

/*
=================
ReloadLanguage_f
=================
*/
CONSOLE_COMMAND( reloadLanguage, "reload language dict", NULL )
{
	commonLocal.InitLanguageDict();
}

/*
=================
Com_FinishBuild_f
=================
*/
CONSOLE_COMMAND( finishBuild, "finishes the build process", NULL )
{
	if( game )
	{
		game->CacheDictionaryMedia( NULL );
	}
}

/*
==============
Com_Help_f
==============
*/
void Com_Help_f( const idCmdArgs& args )
{
	common->Printf( "\nCommonly used commands:\n" );
	common->Printf( "  spawnServer      - start the server.\n" );
	common->Printf( "  disconnect       - shut down the server.\n" );
	common->Printf( "  listCmds         - list all console commands.\n" );
	common->Printf( "  listCVars        - list all console variables.\n" );
	common->Printf( "  kick             - kick a client by number.\n" );
	common->Printf( "  gameKick         - kick a client by name.\n" );
	common->Printf( "  serverNextMap    - immediately load next map.\n" );
	common->Printf( "  serverMapRestart - restart the current map.\n" );
	common->Printf( "  serverForceReady - force all players to ready status.\n" );
	common->Printf( "\nCommonly used variables:\n" );
	common->Printf( "  si_name          - server name (change requires a restart to see)\n" );
	common->Printf( "  si_gametype      - type of game.\n" );
	common->Printf( "  si_fragLimit     - max kills to win (or lives in Last Man Standing).\n" );
	common->Printf( "  si_timeLimit     - maximum time a game will last.\n" );
	common->Printf( "  si_warmup        - do pre-game warmup.\n" );
	common->Printf( "  si_pure          - pure server.\n" );
	common->Printf( "  g_mapCycle       - name of .scriptcfg file for cycling maps.\n" );
	common->Printf( "See mapcycle.scriptcfg for an example of a mapcyle script.\n\n" );
}

/*
=================
idCommonLocal::InitCommands
=================
*/
void idCommonLocal::InitCommands()
{
	cmdSystem->AddCommand( "reloadEngine", Com_ReloadEngine_f, CMD_FL_SYSTEM, "reloads the engine down to including the file system" );

#if	!defined(ID_DEDICATED) && defined(ID_ALLOW_CMD_TOOLS)
	// compilers
	cmdSystem->AddCommand( "dmap", Dmap_f, CMD_FL_TOOL, "compiles a map", idCmdSystem::ArgCompletion_MapName );
//	cmdSystem->AddCommand( "renderbump", RenderBump_f, CMD_FL_TOOL, "renders a bump map", idCmdSystem::ArgCompletion_ModelName );
//	cmdSystem->AddCommand( "renderbumpFlat", RenderBumpFlat_f, CMD_FL_TOOL, "renders a flat bump map", idCmdSystem::ArgCompletion_ModelName );
	cmdSystem->AddCommand( "runAAS", RunAAS_f, CMD_FL_TOOL, "compiles an AAS file for a map", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "runAASDir", RunAASDir_f, CMD_FL_TOOL, "compiles AAS files for all maps in a folder", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "runReach", RunReach_f, CMD_FL_TOOL, "calculates reachability for an AAS file", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "roq", RoQFileEncode_f, CMD_FL_TOOL, "encodes a roq file" );
#endif

#ifdef ID_ALLOW_TOOLS
	// editors
	cmdSystem->AddCommand( "editor", Com_Editor_f, CMD_FL_TOOL, "launches the level editor Radiant" );
	cmdSystem->AddCommand( "editLights", Com_EditLights_f, CMD_FL_TOOL, "launches the in-game Light Editor" );
	cmdSystem->AddCommand( "editSounds", Com_EditSounds_f, CMD_FL_TOOL, "launches the in-game Sound Editor" );
	cmdSystem->AddCommand( "editDecls", Com_EditDecls_f, CMD_FL_TOOL, "launches the in-game Declaration Editor" );
	cmdSystem->AddCommand( "editAFs", Com_EditAFs_f, CMD_FL_TOOL, "launches the in-game Articulated Figure Editor" );
	cmdSystem->AddCommand( "editParticles", Com_EditParticles_f, CMD_FL_TOOL, "launches the in-game Particle Editor" );
	cmdSystem->AddCommand( "editScripts", Com_EditScripts_f, CMD_FL_TOOL, "launches the in-game Script Editor" );
	cmdSystem->AddCommand( "editGUIs", Com_EditGUIs_f, CMD_FL_TOOL, "launches the GUI Editor" );
	cmdSystem->AddCommand( "editPDAs", Com_EditPDAs_f, CMD_FL_TOOL, "launches the in-game PDA Editor" );
	cmdSystem->AddCommand( "debugger", Com_ScriptDebugger_f, CMD_FL_TOOL, "launches the Script Debugger" );

	//BSM Nerve: Add support for the material editor
	cmdSystem->AddCommand( "materialEditor", Com_MaterialEditor_f, CMD_FL_TOOL, "launches the Material Editor" );
#endif

	cmdSystem->AddCommand( "generateMaterialTables", Com_GenerateSinCosTables_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "generates tables required by the engine to run" );

	cmdSystem->AddCommand( "printMemInfo", PrintMemInfo_f, CMD_FL_SYSTEM, "prints memory debugging data" );

	// idLib commands
//	cmdSystem->AddCommand( "memoryDump", Mem_Dump_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "creates a memory dump" );
//	cmdSystem->AddCommand( "memoryDumpCompressed", Mem_DumpCompressed_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "creates a compressed memory dump" );
	cmdSystem->AddCommand( "showStringMemory", idStr::ShowMemoryUsage_f, CMD_FL_SYSTEM, "shows memory used by strings" );
	cmdSystem->AddCommand( "showDictMemory", idDict::ShowMemoryUsage_f, CMD_FL_SYSTEM, "shows memory used by dictionaries" );
	cmdSystem->AddCommand( "listDictKeys", idDict::ListKeys_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "lists all keys used by dictionaries" );
	cmdSystem->AddCommand( "listDictValues", idDict::ListValues_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "lists all values used by dictionaries" );
	cmdSystem->AddCommand( "testSIMD", idSIMD::Test_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "test SIMD code" );

#ifdef ID_DEDICATED
	cmdSystem->AddCommand( "help", Com_Help_f, CMD_FL_SYSTEM, "shows help" );
#endif
}

/*
=================
idCommonLocal::InitRenderSystem
=================
*/
void idCommonLocal::InitRenderSystem()
{
	if( com_skipRenderer.GetBool() )
	{
		return;
	}

	renderSystem->InitOpenGL();

	// initialize the renderSystem data structures
	renderSystem->Init();

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04343" ) );
}

/*
=================
idCommonLocal::PrintLoadingMessage
=================
*/
void idCommonLocal::PrintLoadingMessage( const char* msg )
{
	if( !( msg && *msg ) )
	{
		return;
	}

	//const emptyCommand_t* renderCommands = NULL;

	// RB: this is the same as Doom 3 renderSystem->BeginFrame()
	//renderCommands = renderSystem->SwapCommandBuffers_FinishCommandBuffers();

	const idMaterial* whiteMaterial = declManager->FindMaterial( "_white" );
	const idMaterial* splashScreen = declManager->FindMaterial( "splashScreen" );

	const float sysWidth = renderSystem->GetWidth() * renderSystem->GetPixelAspect();
	const float sysHeight = renderSystem->GetHeight();
	const float sysAspect = sysWidth / sysHeight;
	const float splashAspect = 16.0f / 9.0f;
	const float adjustment = sysAspect / splashAspect;
	const float barHeight = ( adjustment >= 1.0f ) ? 0.0f : ( 1.0f - adjustment ) * ( float )SCREEN_HEIGHT * 0.25f;
	const float barWidth = ( adjustment <= 1.0f ) ? 0.0f : ( adjustment - 1.0f ) * ( float )SCREEN_WIDTH * 0.25f;
	if( barHeight > 0.0f )
	{
		renderSystem->SetColor( colorBlack );
		renderSystem->DrawStretchPic( 0, 0, SCREEN_WIDTH, barHeight, 0, 0, 1, 1, whiteMaterial );
		renderSystem->DrawStretchPic( 0, SCREEN_HEIGHT - barHeight, SCREEN_WIDTH, barHeight, 0, 0, 1, 1, whiteMaterial );
	}
	if( barWidth > 0.0f )
	{
		renderSystem->SetColor( colorBlack );
		renderSystem->DrawStretchPic( 0, 0, barWidth, SCREEN_HEIGHT, 0, 0, 1, 1, whiteMaterial );
		renderSystem->DrawStretchPic( SCREEN_WIDTH - barWidth, 0, barWidth, SCREEN_HEIGHT, 0, 0, 1, 1, whiteMaterial );
	}
	renderSystem->SetColor4( 1, 1, 1, 1 );
	renderSystem->DrawStretchPic( barWidth, barHeight, SCREEN_WIDTH - barWidth * 2.0f, SCREEN_HEIGHT - barHeight * 2.0f, 0, 0, 1, 1, splashScreen );
	int len = strlen( msg );
	renderSystem->DrawSmallStringExt( ( SCREEN_WIDTH - len * SMALLCHAR_WIDTH ) / 2, SCREEN_HEIGHT - 60, msg, idVec4( 0.0f, 0.81f, 0.94f, 1.0f ), true );

	const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers( &time_frontend, &time_backend, &time_shadows, &time_gpu, NULL, NULL );
	renderSystem->RenderCommandBuffers( cmd );

	// RB: this is the same as Doom 3 renderSystem->EndFrame()
	//renderSystem->SwapCommandBuffers_FinishRendering( &time_frontend, &time_backend, &time_shadows, &time_gpu );
}

/*
=================
idCommonLocal::InitSIMD
=================
*/
void idCommonLocal::InitSIMD()
{
	idSIMD::InitProcessor( "doom", com_forceGenericSIMD.GetBool() );
	com_forceGenericSIMD.ClearModified();
}

/*
=================
idCommonLocal::Frame
=================
*/
void idCommonLocal::Frame()
{
	try
	{

		// pump all the events
		Sys_GenerateEvents();

		// write config file if anything changed
		WriteConfiguration();

		// change SIMD implementation if required
		if( com_forceGenericSIMD.IsModified() )
		{
			InitSIMD();
		}

#ifdef ID_ALLOW_TOOLS
		if( com_enableDebuggerServer.IsModified() )
		{
			if( com_enableDebuggerServer.GetBool() )
			{
				DebuggerServerInit();
			}
			else
			{
				DebuggerServerShutdown();
			}
		}
#endif

		eventLoop->RunEventLoop();

		com_frameTime = Sys_Milliseconds();

		idAsyncNetwork::RunFrame();

		if( idAsyncNetwork::IsActive() )
		{
			if( idAsyncNetwork::serverDedicated.GetInteger() != 1 )
			{
				session->GuiFrameEvents();
				session->UpdateScreen( true, false, true );
			}
		}
		else
		{
			session->Frame();

			// normal, in-sequence screen update
			session->UpdateScreen( true, false, true );
		}

		// report timing information

		// RB: changed to have the last game frame time for the next dynamic screen resolution calculation
		//if( com_speeds.GetBool() || com_showFPS.GetInteger() == 1 )
		{
			static int	lastTime;
			int		nowTime = Sys_Milliseconds();
			com_frameMsec = nowTime - lastTime;
			lastTime = nowTime;

			if( com_speeds.GetBool() )
			{
				Printf( "frame:%i all:%3i gfr:%3i rf:%3" PRIuSIZE " bk:%3" PRIuSIZE "\n", com_frameNumber, com_frameMsec, time_gameFrame, time_frontend, time_backend );
			}

			time_lastGameFrame = time_gameFrame;
			time_lastGameDraw = time_gameDraw;

			time_gameFrame = 0;
			time_gameDraw = 0;
		}
		// RB end

		com_frameNumber++;

		// set idLib frame number for frame based memory dumps
		idLib::frameNumber = com_frameNumber;
	}

	catch( idException& )
	{
		return;			// an ERP_DROP was thrown
	}
}

/*
=================
idCommonLocal::GUIFrame
=================
*/
void idCommonLocal::GUIFrame( bool execCmd, bool network )
{
	Sys_GenerateEvents();
	eventLoop->RunEventLoop( execCmd );	// and execute any commands
	if( network )
	{
		idAsyncNetwork::RunFrame();
	}
	session->Frame();

	const bool captureToImage = false;
	session->UpdateScreen( captureToImage, false );
}

/*
=================
idCommonLocal::LoadGameDLL
=================
*/
void idCommonLocal::LoadGameDLL()
{
#ifdef __DOOM_DLL__
	char			dllPath[ MAX_OSPATH ];

	gameImport_t	gameImport;
	gameExport_t	gameExport;
	GetGameAPI_t	GetGameAPI;

	fileSystem->FindDLL( "game", dllPath, true );

	if( !dllPath[ 0 ] )
	{
		common->FatalError( "couldn't find game dynamic library" );
		return;
	}
	common->DPrintf( "Loading game DLL: '%s'\n", dllPath );
	gameDLL = sys->DLL_Load( dllPath );
	if( !gameDLL )
	{
		common->FatalError( "couldn't load game dynamic library" );
		return;
	}

	GetGameAPI = ( GetGameAPI_t ) Sys_DLL_GetProcAddress( gameDLL, "GetGameAPI" );
	if( !GetGameAPI )
	{
		Sys_DLL_Unload( gameDLL );
		gameDLL = 0;
		common->FatalError( "couldn't find game DLL API" );
		return;
	}

	gameImport.version					= GAME_API_VERSION;
	gameImport.sys						= ::sys;
	gameImport.common					= ::common;
	gameImport.cmdSystem				= ::cmdSystem;
	gameImport.cvarSystem				= ::cvarSystem;
	gameImport.fileSystem				= ::fileSystem;
	gameImport.networkSystem			= ::networkSystem;
	gameImport.renderSystem				= ::renderSystem;
	gameImport.soundSystem				= ::soundSystem;
	gameImport.renderModelManager		= ::renderModelManager;
	gameImport.uiManager				= ::uiManager;
	gameImport.declManager				= ::declManager;
	gameImport.AASFileManager			= ::AASFileManager;
	gameImport.collisionModelManager	= ::collisionModelManager;

	gameExport							= *GetGameAPI( &gameImport );

	if( gameExport.version != GAME_API_VERSION )
	{
		Sys_DLL_Unload( gameDLL );
		gameDLL = 0;
		common->FatalError( "wrong game DLL API version" );
		return;
	}

	game								= gameExport.game;
	gameEdit							= gameExport.gameEdit;

#endif

	// initialize the game object
	if( game != NULL )
	{
		game->Init();
	}
}

/*
=================
idCommonLocal::UnloadGameDLL
=================
*/
void idCommonLocal::UnloadGameDLL()
{

	// shut down the game object
	if( game != NULL )
	{
		game->Shutdown();
	}

#ifdef __DOOM_DLL__

	if( gameDLL )
	{
		Sys_DLL_Unload( gameDLL );
		gameDLL = 0;
	}
	game = NULL;
	gameEdit = NULL;

#endif
}

/*
=================
idCommonLocal::IsInitialized
=================
*/
bool idCommonLocal::IsInitialized() const
{
	return com_fullyInitialized;
}

/*
=================
idCommonLocal::Init
=================
*/
void idCommonLocal::Init( int argc, char** argv )
{

	try
	{

		// set interface pointers used by idLib
		idLib::sys			= sys;
		idLib::common		= common;
		idLib::cvarSystem	= cvarSystem;
		idLib::fileSystem	= fileSystem;

		// initialize idLib
		idLib::Init();

		// clear warning buffer
		ClearWarnings( GAME_NAME " initialization" );

		// parse command line options
		ParseCommandLine( argc, argv );

		// init console command system
		cmdSystem->Init();

		// init CVar system
		cvarSystem->Init();

		// start file logging right away, before early console or whatever
		StartupVariable( "win_outputDebugString", false );

		// register all static CVars
		idCVar::RegisterStaticVars();

		// print engine version
		Printf( "%s\n", version.string );

		// initialize key input/binding, done early so bind command exists
		idKeyInput::Init();

		// init the console so we can take prints
		console->Init();

		// get architecture info
		Sys_Init();

		// initialize networking
		Sys_InitNetworking();

		// override cvars from command line
		StartupVariable( NULL, false );

		// set fpu double extended precision
		Sys_FPU_SetPrecision();

		// initialize processor specific SIMD implementation
		InitSIMD();

		// init commands
		InitCommands();

#ifdef ID_WRITE_VERSION
		config_compressor = idCompressor::AllocArithmetic();
#endif

		// game specific initialization
		InitGame();

		// don't add startup commands if no CD key is present
#if ID_ENFORCE_KEY
		if( !session->CDKeysAreValid( false ) || !AddStartupCommands() )
		{
#else
		if( !AddStartupCommands() )
		{
#endif
			// if the user didn't give any commands, run default action
			session->StartMenu( true );
		}

		Printf( "--- Common Initialization Complete ---\n" );

		// print all warnings queued during initialization
		PrintWarnings();

#ifdef	ID_DEDICATED
		Printf( "\nType 'help' for dedicated server info.\n\n" );
#endif

		// remove any prints from the notify lines
		console->ClearNotifyLines();

		ClearCommandLine();

		com_fullyInitialized = true;
	}

	catch( idException& )
	{
		Sys_Error( "Error during initialization" );
	}
}


/*
=================
idCommonLocal::Shutdown
=================
*/
void idCommonLocal::Shutdown()
{

	if( com_shuttingDown )
	{
		return;
	}
	com_shuttingDown = true;

	idAsyncNetwork::server.Kill();
	idAsyncNetwork::client.Shutdown();

	//printf( "ImGuiHook::Destroy();\n" );
	//ImGuiHook::Destroy();

	// game specific shut down
	ShutdownGame( false );

	// shut down non-portable system services
	printf( "Sys_Shutdown();\n" );
	Sys_Shutdown();

	// shut down the console
	printf( "console->Shutdown();\n" );
	console->Shutdown();

	// shut down the key system
	printf( "idKeyInput::Shutdown();\n" );
	idKeyInput::Shutdown();

	// shut down the cvar system
	printf( "cvarSystem->Shutdown();\n" );
	cvarSystem->Shutdown();

	// shut down the console command system
	printf( "cmdSystem->Shutdown();\n" );
	cmdSystem->Shutdown();

#ifdef ID_WRITE_VERSION
	delete config_compressor;
	config_compressor = NULL;
#endif

	// free any buffered warning messages
	printf( "ClearWarnings( GAME_NAME \" shutdown\" );\n" );
	ClearWarnings( GAME_NAME " shutdown" );
	printf( "warningCaption.Clear();\n" );
	warningCaption.Clear();
	printf( "errorList.Clear();\n" );
	errorList.Clear();

	// free language dictionary
	languageDict.Clear();

	// enable leak test
//	Mem_EnableLeakTest( "doom" );

	// shutdown idLib
	printf( "idLib::ShutDown();\n" );
	idLib::ShutDown();
}

/*
=================
idCommonLocal::InitGame
=================
*/
void idCommonLocal::InitGame()
{
	// initialize the file system
	fileSystem->Init();

	// initialize the declaration manager
	declManager->Init();

	// force r_fullscreen 0 if running a tool
	CheckToolMode();

	// initialize string database right off so we can use it for loading messages
	InitLanguageDict();

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04344" ) );

	// init journalling, etc
	eventLoop->Init();

	// init the parallel job manager
	parallelJobManager->Init();

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04345" ) );

	// exec the startup scripts
	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec editor.cfg\n" );
	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec default.cfg\n" );

	// skip the config file if "safe" is on the command line
	if( !SafeMode() )
	{
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec " CONFIG_FILE "\n" );
	}
	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec autoexec.cfg\n" );

	// reload the language dictionary now that we've loaded config files
	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "reloadLanguage\n" );

	// run cfg execution
	cmdSystem->ExecuteCommandBuffer();

	// re-override anything from the config files with command line args
	StartupVariable( NULL, false );

	// if any archived cvars are modified after this, we will trigger a writing of the config file
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04346" ) );

	// start the sound system, but don't do any hardware operations yet
	soundSystem->Init();

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04347" ) );

	// init async network
	idAsyncNetwork::Init();

#ifdef	ID_DEDICATED
	idAsyncNetwork::server.InitPort();
	cvarSystem->SetCVarBool( "s_noSound", true );
#else
	if( idAsyncNetwork::serverDedicated.GetInteger() == 1 )
	{
		idAsyncNetwork::server.InitPort();
		cvarSystem->SetCVarBool( "s_noSound", true );
	}
	else
	{
		// init OpenGL, which will open a window and connect sound and input hardware
		PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04348" ) );
		InitRenderSystem();
	}
#endif

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04349" ) );

	// RB: moved down here to build rectangles for touch screen interfaces
	// init the user command input code
	usercmdGen->Init();

	// initialize the user interfaces
	uiManager->Init();

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04350" ) );

	// load the game dll
	LoadGameDLL();

#ifdef ID_ALLOW_TOOLS
	// startup the script debugger
	if( com_enableDebuggerServer.GetBool() )
	{
		DebuggerServerInit();
	}
#endif

	PrintLoadingMessage( common->GetLanguageDict()->GetString( "#str_04351" ) );

	// RB: load the console history file
	consoleHistory.LoadHistoryFile();
	// RB end

	// init the session
	session->Init();

	// RB: start sound hardware
	cmdSystem->BufferCommandText( CMD_EXEC_NOW, "s_restart\n" );
	cmdSystem->ExecuteCommandBuffer();
}

/*
=================
idCommonLocal::ShutdownGame
=================
*/
void idCommonLocal::ShutdownGame( bool reloading )
{

	// kill sound first
	idSoundWorld* sw = soundSystem->GetPlayingSoundWorld();
	if( sw )
	{
		sw->StopAllSounds();
	}

#ifdef ID_ALLOW_TOOLS
	// startup the script debugger
	if( com_enableDebuggerServer.GetBool() )
	{
		DebuggerServerInit();
	}
#endif

	idAsyncNetwork::client.Shutdown();

	// shut down the session
	printf( "session->Shutdown();\n" );
	session->Shutdown();

	// shut down the user interfaces
	printf( "uiManager->Shutdown();\n" );
	uiManager->Shutdown();

	// shut down the sound system
	printf( "soundSystem->Shutdown();\n" );
	soundSystem->Shutdown();

	// shut down async networking
	idAsyncNetwork::Shutdown();

	// shut down the user command input code
	printf( "usercmdGen->Shutdown();\n" );
	usercmdGen->Shutdown();

	// shut down the event loop
	printf( "eventLoop->Shutdown();\n" );
	eventLoop->Shutdown();

	// shut down the parallel jobs
	printf( "parallelJobManager->Shutdown();\n" );
	parallelJobManager->Shutdown();

	// shut down the renderSystem
	printf( "renderSystem->Shutdown();\n" );
	renderSystem->Shutdown();

	// shutdown the decl manager
	printf( "declManager->Shutdown();\n" );
	declManager->Shutdown();

	// unload the game dll
	printf( "UnloadGameDLL();\n" );
	UnloadGameDLL();

	// dump warnings to "warnings.txt"
#ifdef _DEBUG
	DumpWarnings();
#endif
	// only shut down the log file after all output is done
	printf( "CloseLogFile();\n" );
	CloseLogFile();

	// shut down the file system
	printf( "fileSystem->Shutdown( false );\n" );
	fileSystem->Shutdown( false );
}

/*
=================
idCommonLocal::DebuggerCheckBreakpoint
=================
*/
void idCommonLocal::DebuggerCheckBreakpoint( idInterpreter* interpreter, idProgram* program, int instructionPointer )
{
#ifdef ID_ALLOW_TOOLS
	if( com_enableDebuggerServer.GetBool() )
	{
		DebuggerServerCheckBreakpoint( interpreter, program, instructionPointer );
	}
#endif
}