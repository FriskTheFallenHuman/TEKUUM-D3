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

#ifndef GEVIEWER_H_
#define GEVIEWER_H_

class rvGEViewer
{
public:

	rvGEViewer( );

	bool				Create( HWND parent );
	bool				Destroy();
	bool				OpenFile( const char* filename );

	void				RunFrame();

	HWND				GetWindow();

protected:

	void				Render( HDC dc );
	void				Play();
	void				Pause();

	HWND					mWnd;
	int						mWindowWidth;
	int						mWindowHeight;
	int						mToolbarHeight;
	idUserInterfaceLocal*	mInterface;
	bool					mPaused;
	HWND					mToolbar;
	int						mLastTime;
	int						mTime;

	LRESULT		HandlePaint( WPARAM wParam, LPARAM lParam );

private:

	bool	SetupPixelFormat();

	static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

ID_INLINE HWND rvGEViewer::GetWindow()
{
	return mWnd;
}

#endif // GEVIEWER_H_