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
#ifndef __SHADOWSHARED_H__
#define __SHADOWSHARED_H__

enum shadowVolumeState_t
{
	SHADOWVOLUME_DONE		= 0,
	SHADOWVOLUME_UNFINISHED	= 1
};

// this extra stretch should also make the "inside shadow volume test" valid for both eyes of a stereo view
const float INSIDE_SHADOW_VOLUME_EXTRA_STRETCH = 4.0f;	// in theory, should vary with FOV

#define TEMP_ROUND4( x )				( ( x + 3 ) & ~3 )	// round up to a multiple of 4 for SIMD
#define TEMP_CULLBITS( numVerts )		TEMP_ROUND4( numVerts )

bool R_ViewPotentiallyInsideInfiniteShadowVolume( const idBounds& occluderBounds, const idVec3& localLight, const idVec3& localView, const float znear );

bool R_LineIntersectsTriangleExpandedWithSphere( const idVec3& lineStart, const idVec3& lineEnd, const idVec3& lineDir, const float lineLength,
		const float sphereRadius, const idVec3& triVert0, const idVec3& triVert1, const idVec3& triVert2 );
bool R_ViewInsideShadowVolume( byte* cullBits, const idShadowVert* verts, int numVerts, const triIndex_t* indexes, int numIndexes,
							   const idVec3& localLightOrigin, const idVec3& localViewOrigin, const float zNear );

#endif // !__SHADOWSHARED_H__
