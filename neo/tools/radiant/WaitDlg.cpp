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
#include "WaitDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CWaitDlg dialog

CWaitDlg::CWaitDlg( CWnd* pParent, const char* msg )
	: CDialogEx( CWaitDlg::IDD, pParent )
{
	waitStr = msg;
	cancelPressed = false;
	Create( CWaitDlg::IDD );
}

CWaitDlg::~CWaitDlg()
{
	g_pParentWnd->SetBusy( false );
}

void CWaitDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_WAITSTR, waitStr );
	DDX_Control( pDX, IDD_WAITDLG_PROGRESS, m_progress );
}

BEGIN_MESSAGE_MAP( CWaitDlg, CDialogEx )
END_MESSAGE_MAP()

// CWaitDlg message handlers

BOOL CWaitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//GetDlgItem(IDC_WAITSTR)->SetWindowText(waitStr);
	GetDlgItem( IDC_WAITSTR )->SetFocus();
	UpdateData( FALSE );
	ShowWindow( SW_SHOW );

	// cancel disabled by default
	AllowCancel( false );

	return TRUE;
}

void CWaitDlg::SetText( const char* msg, float percent, bool append )
{
	if( append )
	{
		waitStr = text;
		waitStr += "\r\n";
		waitStr += msg;
	}
	else
	{
		waitStr = msg;
		text = msg;
	}

	m_progress.SetRange( 0, 100 );
	m_progress.SetPos( percent * 100 );

	UpdateData( FALSE );
	Invalidate();
	UpdateWindow();
	ShowWindow( SW_SHOWNORMAL );
}

void CWaitDlg::AllowCancel( bool enable )
{
	// this shows or hides the Cancel button
	CWnd* pCancelButton = GetDlgItem( IDCANCEL );
	ASSERT( pCancelButton );
	if( enable )
	{
		pCancelButton->ShowWindow( SW_NORMAL );
	}
	else
	{
		pCancelButton->ShowWindow( SW_HIDE );
	}
}

bool CWaitDlg::CancelPressed()
{
	MSG* msg = AfxGetCurrentMessage();			// TODO Robert fix me!!
	while( ::PeekMessage( msg, NULL, NULL, NULL, PM_NOREMOVE ) )
	{
		// pump message
		if( !AfxGetApp()->PumpMessage() )
		{
		}
	}

	return cancelPressed;
}

void CWaitDlg::OnCancel()
{
	cancelPressed = true;
}
