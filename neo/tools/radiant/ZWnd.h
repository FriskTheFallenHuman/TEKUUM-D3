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

#include "zclip.h"

// CZWnd window

class CZWnd : public CDialogEx
{
	DECLARE_DYNCREATE( CZWnd );

public:
	CZWnd();

protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

public:
	virtual ~CZWnd();

	CZClip* m_pZClip;

protected:
	HDC m_dcZ;
	HGLRC m_hglrcZ;

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSizing( UINT nSide, LPRECT lpRect );
	afx_msg void OnMoving( UINT nSide, LPRECT lpRect );
	afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp );
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnClose();
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );

	DECLARE_MESSAGE_MAP()
};