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

extern void showCameraInspector();

// CDlgCamera dialog

class CDlgCamera : public CDialogEx
{
public:
	CDlgCamera( CWnd* pParent = NULL );   // standard constructor
	void setupFromCamera();

	enum { IDD = IDD_DLG_CAMERA };
	CScrollBar	m_wndSegments;
	CListBox	m_wndEvents;
	CComboBox	m_wndSplines;
	CString	m_strName;
	float	m_fSeconds;
	BOOL	m_trackCamera;
	int		m_numSegments;
	int		m_currentSegment;
	CString	m_strType;
	int		m_editPoints;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:
	afx_msg void OnBtnAddevent();
	afx_msg void OnBtnAddtarget();
	afx_msg void OnBtnDelevent();
	afx_msg void OnBtnDeltarget();
	afx_msg void OnDblclkComboSplines();
	afx_msg void OnSelchangeComboSplines();
	afx_msg void OnSelchangeListEvents();
	afx_msg void OnDblclkListEvents();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnTestcamera();
	afx_msg void OnBtnDeletepoints();
	afx_msg void OnBtnSelectall();
	afx_msg void OnRadioEditpoints();
	afx_msg void OnRadioAddPoints();

	DECLARE_MESSAGE_MAP()
};