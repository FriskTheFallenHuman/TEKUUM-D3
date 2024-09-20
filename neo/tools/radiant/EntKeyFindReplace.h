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

// return vals for modal dialogue, any values will do that don't clash with the first 9 or so defined by IDOK etc
//
#define ID_RET_REPLACE	100
#define ID_RET_FIND		101


// CEntKeyFindReplace dialog

class CEntKeyFindReplace : public CDialogEx
{
// Construction
public:
	CEntKeyFindReplace( CString* p_strFindKey,
						CString* p_strFindValue,
						CString* p_strReplaceKey,
						CString* p_strReplaceValue,
						bool* 	p_bWholeStringMatchOnly,
						bool* 	p_bSelectAllMatchingEnts,
						CWnd* 	pParent = NULL );  // standard constructor

	enum { IDD = IDD_ENTFINDREPLACE };
	CString	m_strFindKey;
	CString	m_strFindValue;
	CString	m_strReplaceKey;
	CString	m_strReplaceValue;
	BOOL	m_bWholeStringMatchOnly;
	BOOL	m_bSelectAllMatchingEnts;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:

	virtual void OnCancel();
	afx_msg void OnReplace();
	afx_msg void OnFind();
	afx_msg void OnKeycopy();
	afx_msg void OnValuecopy();

	DECLARE_MESSAGE_MAP()

	CString* m_pStrFindKey;
	CString* m_pStrFindValue;
	CString* m_pStrReplaceKey;
	CString* m_pStrReplaceValue;
	bool* 	 m_pbWholeStringMatchOnly;
	bool* 	 m_pbSelectAllMatchingEnts;

	void CopyFields();
};