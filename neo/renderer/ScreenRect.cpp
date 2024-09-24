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

#include "RenderCommon.h"

/*
==========================================================================================

idScreenRect

==========================================================================================
*/

/*
======================
idScreenRect::operator==
======================
*/
bool idScreenRect::operator==( idScreenRect& other ) const
{
	return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2 && zmin == other.zmin && zmax == other.zmax;
}

/*
======================
idScreenRect::operator==
======================
*/
bool idScreenRect::operator!=( idScreenRect& other ) const
{
	return !( *this == other );
}

/*
======================
idScreenRect::Clear
======================
*/
void idScreenRect::Clear()
{
	x1 = y1 = 32000;
	x2 = y2 = -32000;
	zmin = 0.0f;
	zmax = 1.0f;
}

/*
======================
idScreenRect::AddPoint
======================
*/
void idScreenRect::AddPoint( float x, float y )
{
	int	ix = idMath::Ftoi( x );
	int iy = idMath::Ftoi( y );

	if( ix < x1 )
	{
		x1 = ix;
	}
	if( ix > x2 )
	{
		x2 = ix;
	}
	if( iy < y1 )
	{
		y1 = iy;
	}
	if( iy > y2 )
	{
		y2 = iy;
	}
}

/*
======================
idScreenRect::Expand
======================
*/
void idScreenRect::Expand()
{
	x1--;
	y1--;
	x2++;
	y2++;
}

/*
======================
idScreenRect::Intersect
======================
*/
void idScreenRect::Intersect( const idScreenRect& rect )
{
	if( rect.x1 > x1 )
	{
		x1 = rect.x1;
	}
	if( rect.x2 < x2 )
	{
		x2 = rect.x2;
	}
	if( rect.y1 > y1 )
	{
		y1 = rect.y1;
	}
	if( rect.y2 < y2 )
	{
		y2 = rect.y2;
	}
}

/*
======================
idScreenRect::Union
======================
*/
void idScreenRect::Union( const idScreenRect& rect )
{
	if( rect.x1 < x1 )
	{
		x1 = rect.x1;
	}
	if( rect.x2 > x2 )
	{
		x2 = rect.x2;
	}
	if( rect.y1 < y1 )
	{
		y1 = rect.y1;
	}
	if( rect.y2 > y2 )
	{
		y2 = rect.y2;
	}
}

/*
======================
idScreenRect::Equals
======================
*/
bool idScreenRect::Equals( const idScreenRect& rect ) const
{
	return ( x1 == rect.x1 && x2 == rect.x2 && y1 == rect.y1 && y2 == rect.y2 );
}

/*
======================
idScreenRect::IsEmpty
======================
*/
bool idScreenRect::IsEmpty() const
{
	return ( x1 > x2 || y1 > y2 );
}

/*
======================
R_ShowColoredScreenRect
======================
*/
void R_ShowColoredScreenRect( const idScreenRect& rect, int colorIndex )
{
	if( !rect.IsEmpty() )
	{
		static idVec4 colors[] = { colorRed, colorGreen, colorBlue, colorYellow, colorMagenta, colorCyan, colorWhite, colorPurple };
		tr.viewDef->renderWorld->DebugScreenRect( colors[colorIndex & 7], rect, tr.viewDef );
	}
}
