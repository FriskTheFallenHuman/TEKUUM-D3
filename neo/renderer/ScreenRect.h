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

#ifndef __SCREENRECT_H__
#define __SCREENRECT_H__

/*
================================================================================================

idScreenRect

idScreenRect gets carried around with each drawSurf, so it makes sense
to keep it compact, instead of just using the idBounds class
================================================================================================
*/

class idScreenRect
{
public:
	// Inclusive pixel bounds inside viewport
	short		x1;
	short		y1;
	short		x2;
	short		y2;

	// for depth bounds test
	float       zmin;
	float		zmax;

	bool		operator==( idScreenRect& other ) const;
	bool		operator!=( idScreenRect& other ) const;

	// clear to backwards values
	void		Clear();
	bool		IsEmpty() const;
	short		GetWidth() const
	{
		return x2 - x1 + 1;
	}
	short		GetHeight() const
	{
		return y2 - y1 + 1;
	}
	int			GetArea() const
	{
		return ( x2 - x1 + 1 ) * ( y2 - y1 + 1 );
	}

	// expand by one pixel each way to fix roundoffs
	void		Expand();

	// adds a point
	void		AddPoint( float x, float y );

	void		Intersect( const idScreenRect& rect );
	void		Union( const idScreenRect& rect );
	bool		Equals( const idScreenRect& rect ) const;
};

void R_ShowColoredScreenRect( const idScreenRect& rect, int colorIndex );

#endif /* !__SCREENRECT_H__ */
