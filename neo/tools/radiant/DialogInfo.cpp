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
#include "Radiant.h"
#include "DialogInfo.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CDialogInfo dialog
CDialogInfo g_dlgInfo;

void ShowInfoDialog( const char* pText )
{
	if( g_dlgInfo.GetSafeHwnd() )
	{
		g_dlgInfo.m_wndInfo.SetWindowText( pText );
		g_dlgInfo.ShowWindow( SW_SHOW );
	}
	else
	{
		g_dlgInfo.Create( IDD_DLG_INFORMATION );
		g_dlgInfo.m_wndInfo.SetWindowText( pText );
		g_dlgInfo.ShowWindow( SW_SHOW );
	}
	g_pParentWnd->SetFocus();
}

void HideInfoDialog()
{
	if( g_dlgInfo.GetSafeHwnd() )
	{
		g_dlgInfo.ShowWindow( SW_HIDE );
	}
}


CDialogInfo::CDialogInfo( CWnd* pParent )
	: CDialogEx( CDialogInfo::IDD, pParent )
{
}


void CDialogInfo::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_EDIT1, m_wndInfo );
}

BEGIN_MESSAGE_MAP( CDialogInfo, CDialogEx )
END_MESSAGE_MAP()

// CDialogInfo message handlers

BOOL CDialogInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
