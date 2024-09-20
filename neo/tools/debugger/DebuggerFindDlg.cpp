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


#include "../../sys/win32/rc/resource.h"
#include "DebuggerApp.h"
#include "DebuggerFindDlg.h"

char rvDebuggerFindDlg::mFindText[ 256 ];

/*
================
rvDebuggerFindDlg::rvDebuggerFindDlg
================
*/
rvDebuggerFindDlg::rvDebuggerFindDlg()
{
}

/*
================
rvDebuggerFindDlg::DoModal

Launch the dialog
================
*/
bool rvDebuggerFindDlg::DoModal( rvDebuggerWindow* parent )
{
	if( DialogBoxParam( parent->GetInstance(), MAKEINTRESOURCE( IDD_DBG_FIND ), parent->GetWindow(), DlgProc, ( LPARAM )this ) )
	{
		return true;
	}

	return false;
}

/*
================
rvrvDebuggerFindDlg::DlgProc

Dialog Procedure for the find dialog
================
*/
INT_PTR CALLBACK rvDebuggerFindDlg::DlgProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	rvDebuggerFindDlg* dlg = ( rvDebuggerFindDlg* ) GetWindowLongPtr( wnd, GWLP_USERDATA );

	switch( msg )
	{
		case WM_CLOSE:
			EndDialog( wnd, 0 );
			break;

		case WM_INITDIALOG:
			dlg = ( rvDebuggerFindDlg* ) lparam;

			SetWindowLongPtr( wnd, GWLP_USERDATA, ( LONG_PTR ) dlg );
			dlg->mWnd = wnd;
			SetWindowText( GetDlgItem( dlg->mWnd, IDC_DBG_FIND ), dlg->mFindText );
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD( wparam ) )
			{
				case IDOK:
				{
					GetWindowText( GetDlgItem( wnd, IDC_DBG_FIND ), dlg->mFindText, sizeof( dlg->mFindText ) - 1 );
					EndDialog( wnd, 1 );
					break;
				}

				case IDCANCEL:
					EndDialog( wnd, 0 );
					break;
			}
			break;
	}

	return FALSE;
}
