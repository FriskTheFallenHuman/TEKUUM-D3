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

#if 0
#include "../sys/win32/rc/resource.h"

idCVar rbfg_DefaultWidth( "rbfg_DefaultWidth", "0", 0, "" );
idCVar rbfg_DefaultHeight( "rbfg_DefaultHeight", "0", 0, "" );

static idStr RBFName;

static bool CheckPow2( int Num )
{
	while( Num )
	{
		if( ( Num & 1 ) && ( Num != 1 ) )
		{
			return false;
		}

		Num >>= 1;
	}

	return true;
}

static void Com_WriteConfigToFile( const char* filename )
{
	common->Warning( "Some renderbump code called Com_WriteConfigTiFile(\"%s\") which is not implemented!\n", filename );
}

static BOOL CALLBACK RBFProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
			SetDlgItemInt( hwndDlg, IDC_RBF_WIDTH, rbfg_DefaultWidth.GetInteger(), FALSE );
			SetDlgItemInt( hwndDlg, IDC_RBF_HEIGHT, rbfg_DefaultHeight.GetInteger(), FALSE );
			SetDlgItemText( hwndDlg, IDC_RBF_FILENAME, RBFName );
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
				case IDOK:
				{
					int		width, height;

					width = GetDlgItemInt( hwndDlg, IDC_RBF_WIDTH, 0, FALSE );
					height = GetDlgItemInt( hwndDlg, IDC_RBF_HEIGHT, 0, FALSE );

					rbfg_DefaultWidth.SetInteger( width );
					rbfg_DefaultHeight.SetInteger( height );

					Com_WriteConfigToFile( CONFIG_FILE );

					if( !CheckPow2( width ) || !CheckPow2( height ) )
					{
						return TRUE;
					}

					DestroyWindow( hwndDlg );

					cmdSystem->BufferCommandText( CMD_EXEC_APPEND, va( "renderbumpflat -size %d %d %s\n", width, height, RBFName.c_str() ) );
					return TRUE;
				}

				case IDCANCEL:
					DestroyWindow( hwndDlg );
					return TRUE;
			}
	}

	return FALSE;
}

void DoRBFDialog( const char* FileName )
{
	RBFName = FileName;

	Sys_GrabMouseCursor( false );

	DialogBox( 0, MAKEINTRESOURCE( IDD_RENDERBUMPFLAT ), 0, ( DLGPROC )RBFProc );

	Sys_GrabMouseCursor( true );
}
#endif