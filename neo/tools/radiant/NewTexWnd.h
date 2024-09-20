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

#include "../../renderer/RenderCommon.h"

// CTexWnd window

class CNewTexWnd : public CWnd
{
	DECLARE_DYNCREATE( CNewTexWnd );

public:
	CNewTexWnd();
	void UpdateFilter( const char* pFilter );
	void UpdatePrefs();
	void FocusEdit();

public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL );
protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

public:
	void EnsureTextureIsVisible( const char* name );
	virtual ~CNewTexWnd();
	BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR, LRESULT* pResult );
	INT_PTR CNewTexWnd::OnToolHitTest( CPoint point, TOOLINFO* pTI );
	virtual BOOL PreTranslateMessage( MSG* pMsg );

protected:
	//CTexEdit m_wndFilter;
	//CButton  m_wndShaders;
	bool m_bNeedRange;
	HGLRC hglrcTexture;
	CDC* 	hdcTexture;
	CPoint cursor;
	CPoint origin;
	CPoint draw;
	CPoint drawRow;
	CPoint current;
	CRect rectClient;
	int currentRow;
	int currentIndex;
	idList<const idMaterial*> materialList;

	// Generated message map functions
protected:
	const idMaterial* NextPos();
	const idMaterial* getMaterialAtPoint( CPoint point );
	void InitPos();

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnParentNotify( UINT message, LPARAM lParam );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnPaint();
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnShaderClick();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus( CWnd* pOldWnd );
};