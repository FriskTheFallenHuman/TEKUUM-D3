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
#include "CurveDlg.h"


// CCurveDlg dialog

IMPLEMENT_DYNAMIC( CCurveDlg, CDialogEx )
CCurveDlg::CCurveDlg( CWnd* pParent )
	: CDialogEx( CCurveDlg::IDD, pParent )
{
}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_CURVES, comboCurve );
}

void CCurveDlg::OnOK()
{
	UpdateData( TRUE );
	CString str;
	comboCurve.GetWindowText( str );
	strCurveType = str;
	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP( CCurveDlg, CDialogEx )
END_MESSAGE_MAP()