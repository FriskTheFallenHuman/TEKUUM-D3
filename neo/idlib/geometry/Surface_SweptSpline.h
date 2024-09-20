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

#ifndef __SURFACE_SWEPTSPLINE_H__
#define __SURFACE_SWEPTSPLINE_H__

/*
===============================================================================

	Swept Spline surface.

===============================================================================
*/

class idSurface_SweptSpline : public idSurface
{
public:
	idSurface_SweptSpline();
	~idSurface_SweptSpline();

	void					SetSpline( idCurve_Spline<idVec4>* spline );
	void					SetSweptSpline( idCurve_Spline<idVec4>* sweptSpline );
	void					SetSweptCircle( const float radius );

	void					Tessellate( const int splineSubdivisions, const int sweptSplineSubdivisions );

	void					Clear();

protected:
	idCurve_Spline<idVec4>* spline;
	idCurve_Spline<idVec4>* sweptSpline;

	void					GetFrame( const idMat3& previousFrame, const idVec3 dir, idMat3& newFrame );
};

/*
====================
idSurface_SweptSpline::idSurface_SweptSpline
====================
*/
ID_INLINE idSurface_SweptSpline::idSurface_SweptSpline()
{
	spline = NULL;
	sweptSpline = NULL;
}

/*
====================
idSurface_SweptSpline::~idSurface_SweptSpline
====================
*/
ID_INLINE idSurface_SweptSpline::~idSurface_SweptSpline()
{
	delete spline;
	delete sweptSpline;
}

/*
====================
idSurface_SweptSpline::Clear
====================
*/
ID_INLINE void idSurface_SweptSpline::Clear()
{
	idSurface::Clear();
	delete spline;
	spline = NULL;
	delete sweptSpline;
	sweptSpline = NULL;
}

#endif /* !__SURFACE_SWEPTSPLINE_H__ */
