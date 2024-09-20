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
#ifndef __STATICSHADOWVOLUME_H__
#define __STATICSHADOWVOLUME_H__

/*
================================================================================================

Static Shadow Volume Setup

A static shadow is cast from a static model touching a static light.
A static shadow volume extends to infinity which allows the end caps to be omitted
when the view is outside and far enough away from the shadow volume.

A static shadow volume is created at map load time and this job determines whether or
not the end caps need to be rendered, whether or not the shadow volume needs to be
rendered with Z-Fail, and optionally calculates the shadow volume depth bounds.

================================================================================================
*/

/*
================================================
staticShadowVolumeParms_t
================================================
*/
struct staticShadowVolumeParms_t
{
	// input
	const idShadowVert* 			verts;					// streamed in from main memory
	int								numVerts;
	const triIndex_t* 				indexes;				// streamed in from main memory
	int								numIndexes;
	int								numShadowIndicesWithCaps;
	int								numShadowIndicesNoCaps;
	idBounds						triangleBounds;
	idRenderMatrix					triangleMVP;
	idVec3							localLightOrigin;
	idVec3							localViewOrigin;
	float							zNear;
	float							lightZMin;
	float							lightZMax;
	bool							forceShadowCaps;
	bool							useShadowPreciseInsideTest;
	bool							useShadowDepthBounds;
	// temp
	byte* 							tempCullBits;			// temp buffer in SPU local memory
	// output
	int* 							numShadowIndices;		// streamed out to main memory
	int* 							renderZFail;			// streamed out to main memory
	float* 							shadowZMin;				// streamed out to main memory
	float* 							shadowZMax;				// streamed out to main memory
	volatile shadowVolumeState_t* 	shadowVolumeState;		// streamed out to main memory
	// next in chain on view entity
	staticShadowVolumeParms_t* 		next;
	int								pad[3];
};


void StaticShadowVolumeJob( const staticShadowVolumeParms_t* parms );

#endif // !__STATICSHADOWVOLUME_H__
