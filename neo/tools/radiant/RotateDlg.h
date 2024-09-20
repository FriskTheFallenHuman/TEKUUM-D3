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

// CRotateDlg dialog

class CRotateDlg : public CDialogEx
{
public:
	CRotateDlg( CWnd* pParent = NULL );   // standard constructor

	enum { IDD = IDD_ROTATE };
	CSpinButtonCtrl	m_wndSpin3;
	CSpinButtonCtrl	m_wndSpin2;
	CSpinButtonCtrl	m_wndSpin1;
	CString	m_strX;
	CString	m_strY;
	CString	m_strZ;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:
	void ApplyNoPaint();

	virtual void OnOK();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDeltaposSpin2( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDeltaposSpin3( NMHDR* pNMHDR, LRESULT* pResult );

	DECLARE_MESSAGE_MAP()
};