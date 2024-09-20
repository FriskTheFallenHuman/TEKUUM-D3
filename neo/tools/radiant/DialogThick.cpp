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
#include "DialogThick.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CDialogThick dialog

CDialogThick::CDialogThick( CWnd* pParent )
	: CDialogEx( CDialogThick::IDD, pParent )
{
	m_bSeams = TRUE;
	m_nAmount = 8;
}


void CDialogThick::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Check( pDX, IDC_CHECK_SEAMS, m_bSeams );
	DDX_Text( pDX, IDC_EDIT_AMOUNT, m_nAmount );
}


BEGIN_MESSAGE_MAP( CDialogThick, CDialogEx )
END_MESSAGE_MAP()