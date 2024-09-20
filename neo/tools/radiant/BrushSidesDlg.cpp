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
#include "BrushSidesDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC( CBrushSidesDlg, CDialogEx )

CBrushSidesDlg::CBrushSidesDlg( bool bDoCone, bool bDoSphere, CWnd* pParent )
	: CDialogEx( IDD_SIDES, pParent ), m_bDoCone( bDoCone ), m_bDoSphere( bDoSphere )
{
}


CBrushSidesDlg::~CBrushSidesDlg()
{
}

void CBrushSidesDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SIDES, m_editSides );
}

BEGIN_MESSAGE_MAP( CBrushSidesDlg, CDialogEx )
	ON_BN_CLICKED( IDOK, OnOK )
	ON_BN_CLICKED( IDCANCEL, OnCancel )
END_MESSAGE_MAP()

BOOL CBrushSidesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editSides.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

void CBrushSidesDlg::OnOK()
{
	CString str;
	m_editSides.GetWindowText( str );

	if( m_bDoCone )
	{
		Brush_MakeSidedCone( _ttoi( str ) );
	}
	else if( m_bDoSphere )
	{
		Brush_MakeSidedSphere( _ttoi( str ) );
	}
	else
	{
		Brush_MakeSided( _ttoi( str ) );
	}

	CDialogEx::OnOK();
}

void CBrushSidesDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

void DoSides( bool bCone, bool bSphere, bool bTorus )
{
	CBrushSidesDlg dlg( bCone, bSphere );
	dlg.DoModal();
}