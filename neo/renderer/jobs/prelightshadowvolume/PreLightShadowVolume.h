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
#ifndef __PRELIGHTSHADOWVOLUME_H__
#define __PRELIGHTSHADOWVOLUME_H__

/*
================================================================================================

Pre-Light Shadow Volume Setup

A pre-light shadow is cast from static world geometry touching a static light.
A pre-light shadow volume does not extend to infinity but is capped at the light
boundaries which means the end caps always need to be rendered.

A pre-light shadow volume is created at map compile time and this job determines whether or
not the shadow volume needs to be rendered with Z-Fail.

================================================================================================
*/

/*
================================================
preLightShadowVolumeParms_t
================================================
*/
struct preLightShadowVolumeParms_t
{
	// input
	const idShadowVert* 			verts;					// streamed in from main memory
	int								numVerts;
	const triIndex_t* 				indexes;				// streamed in from main memory
	int								numIndexes;
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
	// next in chain on view light
	preLightShadowVolumeParms_t* 	next;
	int								pad;
};


void PreLightShadowVolumeJob( const preLightShadowVolumeParms_t* parms );

#endif // !__PRELIGHTSHADOWVOLUME_H__
