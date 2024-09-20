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
#include "FindTextureDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CFindTextureDlg dialog

CFindTextureDlg g_TexFindDlg;
CFindTextureDlg& g_dlgFind = g_TexFindDlg;
static bool g_bFindActive = true;

void CFindTextureDlg::updateTextures( const char* p )
{
	if( isOpen() )
	{
		if( g_bFindActive )
		{
			setFindStr( p );
		}
		else
		{
			setReplaceStr( p );
		}
	}
}

CFindTextureDlg::CFindTextureDlg( CWnd* pParent )
	: CDialogEx( CFindTextureDlg::IDD, pParent ),
	  m_bSelectedOnly( FALSE ),
	  m_strFind( _T( "" ) ),
	  m_strReplace( _T( "" ) ),
	  m_bForce( FALSE ),
	  m_bLive( TRUE )
{
}

void CFindTextureDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Check( pDX, IDC_CHECK_SELECTED, m_bSelectedOnly );
	DDX_Text( pDX, IDC_EDIT_FIND, m_strFind );
	DDX_Text( pDX, IDC_EDIT_REPLACE, m_strReplace );
	DDX_Check( pDX, IDC_CHECK_FORCE, m_bForce );
	DDX_Check( pDX, IDC_CHECK_LIVE, m_bLive );
}

BEGIN_MESSAGE_MAP( CFindTextureDlg, CDialogEx )
	ON_BN_CLICKED( IDOK, OnOK )
	ON_BN_CLICKED( IDCANCEL, OnCancel )
	ON_BN_CLICKED( IDC_BTN_APPLY, &CFindTextureDlg::OnBtnApply )
	ON_EN_SETFOCUS( IDC_EDIT_FIND, &CFindTextureDlg::OnSetfocusEditFind )
	ON_EN_SETFOCUS( IDC_EDIT_REPLACE, &CFindTextureDlg::OnSetfocusEditReplace )
END_MESSAGE_MAP()

void CFindTextureDlg::OnBtnApply()
{
	UpdateData( TRUE );
	CRect rct;
	GetWindowRect( rct );
	SaveRegistryInfo( "radiant_texturefindwindow", &rct, sizeof( rct ) );
	FindReplaceTextures( m_strFind, m_strReplace, m_bSelectedOnly != FALSE, m_bForce != FALSE );
}

void CFindTextureDlg::OnOK()
{
	UpdateData( TRUE );
	CRect rct;
	GetWindowRect( rct );
	SaveRegistryInfo( "radiant_texturefindwindow", &rct, sizeof( rct ) );
	FindReplaceTextures( m_strFind, m_strReplace, m_bSelectedOnly != FALSE, m_bForce != FALSE );
	CDialogEx::OnOK();
}

void CFindTextureDlg::show()
{
	if( !g_dlgFind.GetSafeHwnd() || !IsWindow( g_dlgFind.GetSafeHwnd() ) )
	{
		g_dlgFind.Create( IDD_DIALOG_FINDREPLACE );
		g_dlgFind.ShowWindow( SW_SHOW );
	}
	else
	{
		g_dlgFind.ShowWindow( SW_SHOW );
	}
	CRect rct;
	LONG lSize = sizeof( rct );
	if( LoadRegistryInfo( "radiant_texturefindwindow", &rct, &lSize ) )
	{
		g_dlgFind.SetWindowPos( nullptr, rct.left, rct.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
	}
}

bool CFindTextureDlg::isOpen()
{
	return g_dlgFind.GetSafeHwnd() && ::IsWindowVisible( g_dlgFind.GetSafeHwnd() );
}

void CFindTextureDlg::setFindStr( const char* p )
{
	g_dlgFind.UpdateData( TRUE );
	if( g_dlgFind.m_bLive )
	{
		g_dlgFind.m_strFind = p;
		g_dlgFind.UpdateData( FALSE );
	}
}

void CFindTextureDlg::setReplaceStr( const char* p )
{
	g_dlgFind.UpdateData( TRUE );
	if( g_dlgFind.m_bLive )
	{
		g_dlgFind.m_strReplace = p;
		g_dlgFind.UpdateData( FALSE );
	}
}

void CFindTextureDlg::OnCancel()
{
	CRect rct;
	GetWindowRect( rct );
	SaveRegistryInfo( "radiant_texturefindwindow", &rct, sizeof( rct ) );
	CDialogEx::OnCancel();
}

void CFindTextureDlg::OnSetfocusEditFind()
{
	g_bFindActive = true;
}

void CFindTextureDlg::OnSetfocusEditReplace()
{
	g_bFindActive = false;
}