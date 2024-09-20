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
#include "PatchDensityDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CPatchDensityDlg dialog

CPatchDensityDlg::CPatchDensityDlg( CWnd* pParent )
	: CDialogEx( CPatchDensityDlg::IDD, pParent )
{
}


void CPatchDensityDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_WIDTH, m_wndWidth );
	DDX_Control( pDX, IDC_COMBO_HEIGHT, m_wndHeight );
}


BEGIN_MESSAGE_MAP( CPatchDensityDlg, CDialogEx )
END_MESSAGE_MAP()

// CPatchDensityDlg message handlers

int g_nXLat[] = {3, 5, 7, 9, 11, 13, 15};

void CPatchDensityDlg::OnOK()
{
	int nWidth = m_wndWidth.GetCurSel();
	int nHeight = m_wndHeight.GetCurSel();

	if( nWidth >= 0 && nWidth <= 6 && nHeight >= 0 && nHeight <= 6 )
	{
		Patch_GenericMesh( g_nXLat[nWidth], g_nXLat[nHeight], ( int )g_pParentWnd->ActiveXY()->GetViewType() );
		Sys_UpdateWindows( W_ALL );
	}

	CDialogEx::OnOK();
}

BOOL CPatchDensityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wndWidth.SetCurSel( 0 );
	m_wndHeight.SetCurSel( 0 );
	return TRUE;
}
