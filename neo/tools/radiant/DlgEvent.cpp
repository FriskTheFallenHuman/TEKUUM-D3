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
#include "DlgEvent.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CDlgEvent dialog

CDlgEvent::CDlgEvent( CWnd* pParent )
	: CDialogEx( CDlgEvent::IDD, pParent )
{
	m_strParm = _T( "" );
	m_event = 0;
}


void CDlgEvent::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_EDIT_PARAM, m_strParm );
	DDX_Radio( pDX, IDC_RADIO_EVENT, m_event );
}

BEGIN_MESSAGE_MAP( CDlgEvent, CDialogEx )
END_MESSAGE_MAP()