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
#ifndef GESTATUSBAR_H_
#define GESTATUSBAR_H_

class rvGEStatusBar
{
public:

	rvGEStatusBar( );

	bool	Create( HWND parent, UINT id, bool visible = true );
	void	Resize( int width, int height );

	HWND	GetWindow();

	void	SetZoom( int zoom );
	void	SetTriangles( int tris );
	void	SetSimple( bool simple );

	void	Show( bool state );
	void	Update();

protected:

	HWND	mWnd;
	bool	mSimple;
	int		mZoom;
	int		mTriangles;
};

ID_INLINE HWND rvGEStatusBar::GetWindow()
{
	return mWnd;
}

ID_INLINE void rvGEStatusBar::SetZoom( int zoom )
{
	if( mZoom != zoom )
	{
		mZoom = zoom;
		Update( );
	}
}

ID_INLINE void rvGEStatusBar::SetTriangles( int triangles )
{
	if( triangles != mTriangles )
	{
		mTriangles = triangles;
		Update( );
	}
}

ID_INLINE void rvGEStatusBar::SetSimple( bool simple )
{
	if( mSimple != simple )
	{
		mSimple = simple;
		Update( );
	}
}

#endif // GESTATUSBAR_H_