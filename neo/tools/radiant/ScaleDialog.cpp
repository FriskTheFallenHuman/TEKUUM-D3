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
#include "ScaleDialog.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CScaleDialog dialog

CScaleDialog::CScaleDialog( CWnd* pParent )
	: CDialogEx( CScaleDialog::IDD, pParent )
{
	m_fZ = 1.0f;
	m_fX = 1.0f;
	m_fY = 1.0f;
}


void CScaleDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_EDIT_Z, m_fZ );
	DDX_Text( pDX, IDC_EDIT_X, m_fX );
	DDX_Text( pDX, IDC_EDIT_Y, m_fY );
}

BEGIN_MESSAGE_MAP( CScaleDialog, CDialogEx )
END_MESSAGE_MAP()