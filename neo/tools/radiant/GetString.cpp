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

#include "GetString.h"

// CGetString dialog

CGetString::CGetString( LPCSTR pPrompt, CString* pFeedback, CWnd* pParent )
	: CDialogEx( CGetString::IDD, pParent )
{
	m_strEditBox = _T( "" );

	m_pFeedback = pFeedback;
	m_pPrompt	= pPrompt;
}

CGetString::~CGetString()
{
}

void CGetString::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_EDIT1, m_strEditBox );
}

BOOL CGetString::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_PROMPT )->SetWindowText( m_pPrompt );
	return TRUE;
}

BEGIN_MESSAGE_MAP( CGetString, CDialogEx )
END_MESSAGE_MAP()

void CGetString::OnOK()
{
	UpdateData( DIALOG_TO_DATA );

	*m_pFeedback = m_strEditBox;

	CDialogEx::OnOK();
}

// returns NULL if CANCEL, else input string
LPCSTR GetString( LPCSTR psPrompt )
{
	static CString strReturn;

	CGetString Input( psPrompt, &strReturn );
	if( Input.DoModal() == IDOK )
	{
		strReturn.TrimLeft();
		strReturn.TrimRight();

		return ( LPCSTR )strReturn;
	}

	return NULL;
}


bool GetYesNo( const char* psQuery )
{
	if( MessageBoxA( g_pParentWnd->GetSafeHwnd(), psQuery, "Query", MB_YESNO | MB_ICONWARNING ) == IDYES )
	{
		return true;
	}

	return false;
}

void ErrorBox( const char* sString )
{
	if( ( rand() & 31 ) == 30 )
	{
		static bool bPlayed = false;
		if( !bPlayed )
		{
			bPlayed = true;
			PlaySound( "k:\\util\\overlay.bin", NULL, SND_FILENAME | SND_ASYNC );
		}
	}
	MessageBoxA( g_pParentWnd->GetSafeHwnd(), sString, "Error",		MB_OK | MB_ICONERROR | MB_TASKMODAL );
}

void InfoBox( const char* sString )
{
	MessageBoxA( g_pParentWnd->GetSafeHwnd(), sString, "Info",		MB_OK | MB_ICONINFORMATION | MB_TASKMODAL );
}

void WarningBox( const char* sString )
{
	MessageBoxA( g_pParentWnd->GetSafeHwnd(), sString, "Warning",	MB_OK | MB_ICONWARNING | MB_TASKMODAL );
}
