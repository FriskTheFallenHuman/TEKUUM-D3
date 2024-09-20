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

// CSurfaceDlg dialog

class CSurfaceDlg : public CDialogEx
{
	bool m_bPatchMode;
	CWnd* focusControl;

public:
	CSurfaceDlg( CWnd* pParent = NULL );   // standard constructor
	void SetTexMods();

	enum { IDD = IDD_SURFACE };
	CEdit	m_wndRotateEdit;
	CEdit	m_wndVert;
	CEdit	m_wndHorz;
	CSliderCtrl	m_wndVerticalSubdivisions;
	CSliderCtrl	m_wndHorzSubdivisions;
	CSpinButtonCtrl	m_wndWidth;
	CSpinButtonCtrl	m_wndHeight;
	CSpinButtonCtrl	m_wndVShift;
	CSpinButtonCtrl	m_wndVScale;
	CSpinButtonCtrl	m_wndRotate;
	CSpinButtonCtrl	m_wndHShift;
	CSpinButtonCtrl	m_wndHScale;
	int		m_nHorz;
	int		m_nVert;
	float	m_horzScale;
	float	m_horzShift;
	float	m_rotate;
	float	m_vertScale;
	float	m_vertShift;
	CString	m_strMaterial;
	BOOL	m_subdivide;
	float	m_fHeight;
	float	m_fWidth;
	BOOL	m_absolute;

public:
	virtual BOOL PreTranslateMessage( MSG* pMsg );
protected:
	virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

protected:

	void UpdateSpinners( int nScrollCode, int nPos, CScrollBar* pBar );
	void UpdateSpinners( bool bUp, int nID );

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnColor();
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDeltaPosSpin( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnBtnPatchdetails();
	afx_msg void OnBtnPatchnatural();
	afx_msg void OnBtnPatchreset();
	afx_msg void OnBtnAxial();
	afx_msg void OnBtnBrushfit();
	afx_msg void OnBtnFacefit();
	afx_msg void OnCheckSubdivide();
	afx_msg void OnChangeEditHorz();
	afx_msg void OnChangeEditVert();
	afx_msg void OnSetfocusHscale();
	afx_msg void OnKillfocusHscale();
	afx_msg void OnKillfocusVscale();
	afx_msg void OnSetfocusVscale();
	afx_msg void OnKillfocusEditWidth();
	afx_msg void OnSetfocusEditWidth();
	afx_msg void OnKillfocusEditHeight();
	afx_msg void OnSetfocusEditHeight();
	afx_msg void OnBtnFlipx();
	afx_msg void OnBtnFlipy();
	afx_msg void OnKillfocusRotate();
	afx_msg void OnSetfocusRotate();

	DECLARE_MESSAGE_MAP()
};