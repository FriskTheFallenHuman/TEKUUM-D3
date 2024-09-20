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

#pragma once

// NOTE: already included in qe3.h but won't compile without including it again !?
#include "../../sys/win32/rc/resource.h"

class CGetString : public CDialogEx
{
public:
	CGetString( LPCSTR pPrompt, CString* pFeedback, CWnd* pParent = NULL ); // standard constructor
	virtual ~CGetString();
// Overrides

// Dialog Data

	enum { IDD = IDD_DIALOG_GETSTRING };

	CString	m_strEditBox;
	CString* m_pFeedback;
	LPCSTR	m_pPrompt;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

LPCSTR GetString( LPCSTR psPrompt );
bool GetYesNo( const char* psQuery );
void ErrorBox( const char* sString );
void InfoBox( const char* sString );
void WarningBox( const char* sString );