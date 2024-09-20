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

// CPatchDialog dialog

class CPatchDialog : public CDialogEx
{
	patchMesh_t* m_Patch;

public:
	void UpdateInfo();
	void SetPatchInfo();
	void GetPatchInfo();
	CPatchDialog( CWnd* pParent = NULL );   // standard constructor
	void UpdateSpinners( bool bUp, int nID );

	enum { IDD = IDD_DIALOG_PATCH };
	CSpinButtonCtrl	m_wndVShift;
	CSpinButtonCtrl	m_wndVScale;
	CSpinButtonCtrl	m_wndRotate;
	CSpinButtonCtrl	m_wndHShift;
	CSpinButtonCtrl	m_wndHScale;
	CComboBox	m_wndType;
	CComboBox	m_wndRows;
	CComboBox	m_wndCols;
	CString	m_strName;
	float	m_fS;
	float	m_fT;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fHScale;
	float	m_fHShift;
	float	m_fRotate;
	float	m_fVScale;
	float	m_fVShift;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:
	void UpdateRowColInfo();

	afx_msg void OnBtnPatchdetails();
	afx_msg void OnBtnPatchfit();
	afx_msg void OnBtnPatchnatural();
	afx_msg void OnBtnPatchreset();
	afx_msg void OnSelchangeComboCol();
	afx_msg void OnSelchangeComboRow();
	afx_msg void OnSelchangeComboType();
	virtual void OnOK();
	afx_msg void OnDeltaposSpin( NMHDR* pNMHDR, LRESULT* pResult );
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnApply();

	DECLARE_MESSAGE_MAP()
};