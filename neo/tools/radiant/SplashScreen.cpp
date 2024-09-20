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
#include "SplashScreen.h"

CSplashScreen* CSplashScreen::c_pSplashWnd = NULL;

CSplashScreen::CSplashScreen()
{
}

CSplashScreen::~CSplashScreen()
{
}

BEGIN_MESSAGE_MAP( CSplashScreen, CWnd )
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSplashScreen::ShowSplashScreen( CWnd* pParentWnd )
{
	if( c_pSplashWnd == NULL )
	{
		c_pSplashWnd = new CSplashScreen;
		if( !c_pSplashWnd->Create( pParentWnd ) )
		{
			delete c_pSplashWnd;
		}
	}
}

void CSplashScreen::HideSplashScreen()
{
	if( c_pSplashWnd != NULL )
	{
		c_pSplashWnd->Hide();
		c_pSplashWnd = NULL;
	}
}

BOOL CSplashScreen::Create( CWnd* pParentWnd )
{
	if( !m_bitmap.LoadBitmap( IDB_BITMAP_LOGO ) )
	{
		return FALSE;
	}

	BITMAP bm;
	m_bitmap.GetBitmap( &bm );

	CRect rect( 0, 0, bm.bmWidth, bm.bmHeight );
	DWORD dwStyle = WS_POPUP | WS_VISIBLE;

	if( !CWnd::CreateEx( 0, AfxRegisterWndClass( 0, AfxGetApp()->LoadStandardCursor( IDC_ARROW ) ), NULL, dwStyle, rect, pParentWnd, 0 ) )
	{
		return FALSE;
	}

	// Set the splash screen always in the foreground
	SetWindowPos( &CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	CenterWindow();

//	SetTimer(1, 3000, NULL);

	UpdateWindow();

	return TRUE;
}

void CSplashScreen::Hide()
{
	KillTimer( 1 );
	DestroyWindow();
}

void CSplashScreen::PostNcDestroy()
{
	delete this;
}

void CSplashScreen::OnPaint()
{
	CPaintDC dc( this );
	DrawSplash( &dc );
}

void CSplashScreen::DrawSplash( CDC* pDC )
{
	CDC dcImage;
	dcImage.CreateCompatibleDC( pDC );

	CBitmap* pOldBitmap = dcImage.SelectObject( &m_bitmap );

	BITMAP bm;
	m_bitmap.GetBitmap( &bm );

	pDC->BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY );

	dcImage.SelectObject( pOldBitmap );
}

void CSplashScreen::OnTimer( UINT_PTR nIDEvent )
{
	//Hide();
}