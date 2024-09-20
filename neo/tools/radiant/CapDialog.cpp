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
#include "CapDialog.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CCapDialog dialog

CCapDialog::CCapDialog( CWnd* pParent )
	: CDialogEx( CCapDialog::IDD, pParent )
{
	m_nCap = 0;
}


void CCapDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Radio( pDX, IDC_RADIO_CAP, m_nCap );
}

BEGIN_MESSAGE_MAP( CCapDialog, CDialogEx )
END_MESSAGE_MAP()