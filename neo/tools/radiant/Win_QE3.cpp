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

#include "qe3.h"
#include "mru.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Sys_MarkMapModified()
{
	idStr title;

	if( mapModified != 1 )
	{
		mapModified = 1;	// mark the map as changed
		title = currentmap;
		title += " *";
		title.BackSlashesToSlashes();
		Sys_SetTitle( title );
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Sys_SetTitle( const char* text )
{
	g_pParentWnd->SetWindowText( va( "%s: %s", EDITOR_WINDOWTEXT, text ) );
}

/*
 =======================================================================================================================
 Wait Functions
 =======================================================================================================================
 */
HCURSOR waitcursor;

void Sys_BeginWait()
{
	waitcursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );
}

bool Sys_Waiting()
{
	return ( waitcursor != NULL );
}

void Sys_EndWait()
{
	if( waitcursor )
	{
		SetCursor( waitcursor );
		waitcursor = NULL;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Sys_GetCursorPos( int* x, int* y )
{
	POINT	lpPoint;

	GetCursorPos( &lpPoint );
	*x = lpPoint.x;
	*y = lpPoint.y;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Sys_SetCursorPos( int x, int y )
{
	SetCursorPos( x, y );
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
double Sys_DoubleTime()
{
	return clock() / 1000.0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int WINAPI QEW_SetupPixelFormat( HDC hDC, bool zbuffer )
{
	int pixelFormat = ChoosePixelFormat( hDC, &win32.pfd );
	if( pixelFormat > 0 )
	{
		if( SetPixelFormat( hDC, pixelFormat, &win32.pfd ) == NULL )
		{
			idLib::Error( "SetPixelFormat failed." );
		}
	}
	else
	{
		idLib::Error( "ChoosePixelFormat failed." );
	}

	return pixelFormat;
}

/*
 =======================================================================================================================
	FILE DIALOGS
 =======================================================================================================================
 */
bool ConfirmModified()
{
	if( !mapModified )
	{
		return true;
	}

	if( g_pParentWnd->MessageBox( "This will lose changes to the map.", "Unsaved Changes", MB_OKCANCEL | MB_ICONWARNING ) == IDCANCEL )
	{
		return false;
	}

	return true;
}

static OPENFILENAME ofn;														/* common dialog box structure */
static char			szFile[MAX_PATH];											/* filename string */
static char			szFileTitle[260];											/* file title string */
static char			szFilter[260] =	"Map file (*.map, *.reg)\0*.map;*.reg\0";	/* filter string */

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void OpenDialog()
{
	/* Place the terminating null character in the szFile. */
	szFile[0] = '\0';

	/* Set the members of the OPENFILENAME structure. */
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = g_pParentWnd->GetSafeHwnd();
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof( szFileTitle );
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	/* Display the Open dialog box. */
	if( !GetOpenFileName( &ofn ) )
	{
		return; // canceled
	}

	// Add the file in MRU. FIXME
	AddNewItem( g_qeglobals.d_lpMruMenu, ofn.lpstrFile );

	// Refresh the File menu. FIXME
	PlaceMenuMRUItem( g_qeglobals.d_lpMruMenu, GetSubMenu( GetMenu( g_pParentWnd->GetSafeHwnd() ), 0 ), ID_FILE_EXIT );

	/* Open the file. */
	Map_LoadFile( ofn.lpstrFile );

	g_PrefsDlg.SavePrefs();

}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void SaveAsDialog( bool bRegion )
{
	/* Place the terminating null character in the szFile. */
	szFile[0] = '\0';

	/* Set the members of the OPENFILENAME structure. */
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = g_pParentWnd->GetSafeHwnd();
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof( szFileTitle );
	ofn.lpstrInitialDir = "";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	/* Display the Open dialog box. */
	if( !GetSaveFileName( &ofn ) )
	{
		return; // canceled
	}

	if( bRegion )
	{
		DefaultExtension( ofn.lpstrFile, ".reg" );
	}
	else
	{
		DefaultExtension( ofn.lpstrFile, ".map" );
	}

	if( !bRegion )
	{
		strcpy( currentmap, ofn.lpstrFile );
		AddNewItem( g_qeglobals.d_lpMruMenu, ofn.lpstrFile );
		PlaceMenuMRUItem( g_qeglobals.d_lpMruMenu, GetSubMenu( GetMenu( g_pParentWnd->GetSafeHwnd() ), 0 ), ID_FILE_EXIT );
	}

	Map_SaveFile( ofn.lpstrFile, bRegion );	// ignore region
}