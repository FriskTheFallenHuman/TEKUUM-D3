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

//===============================================================
//
//	idVecX
//
//===============================================================

float	idVecX::temp[VECX_MAX_TEMP + 4];
// RB: changed int to intptr_t
float* 	idVecX::tempPtr = ( float* )( ( ( intptr_t ) idVecX::temp + 15 ) & ~15 );
// RB end
int		idVecX::tempIndex = 0;

/*
=============
idVecX::ToString
=============
*/
const char* idVecX::ToString( int precision ) const
{
	return idStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
}
