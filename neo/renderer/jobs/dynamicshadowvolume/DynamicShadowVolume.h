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
#ifndef __DYNAMICSHADOWVOLUME_H__
#define __DYNAMICSHADOWVOLUME_H__

/*
================================================================================================

Dynamic Shadow Volume Setup

A dynamic shadow is cast from a model touching a light where either the model or light is dynamic or both.
A dynamic shadow volume extends to infinity which allows the end caps to be omitted
when the view is outside and far enough away from the shadow volume.

A dynamic shadow volume is created at run-time in this job. This job also determines whether
or not end caps need to be generated and rendered, whether or not the shadow volume needs
to be rendered with Z-Fail, and optionally calculates the shadow volume depth bounds.

The occluder triangles are optionally culled to the light projection matrix. This is
particularly important for spot lights that would otherwise cast shadows in all directions.
However, there can also be significant savings when a small point light touches a large
model like for instance a world model.

================================================================================================
*/

#define TEMP_ROUND16( x )				( ( x + 15 ) & ~15 )
#define TEMP_FACING( numIndexes )		TEMP_ROUND16( ( ( numIndexes / 3 + 3 ) & ~3 ) + 1 )	// rounded up for SIMD, plus 1 for dangling edges
#define TEMP_CULL( numIndexes )			TEMP_ROUND16( ( ( numIndexes / 3 + 3 ) & ~3 ) )		// rounded up for SIMD
#define TEMP_VERTS( numVerts )			TEMP_ROUND16( numVerts * sizeof( idVec4 ) )
#define OUTPUT_INDEX_BUFFER_SIZE		4096

struct silEdge_t
{
	// NOTE: using triIndex_t for the planes is dubious, as there can be 2x the faces as verts
	triIndex_t					p1, p2;					// planes defining the edge
	triIndex_t					v1, v2;					// verts defining the edge
};

/*
================================================
dynamicShadowVolumeParms_t
================================================
*/
struct dynamicShadowVolumeParms_t
{
	// input
	const idDrawVert* 				verts;					// streamed in from main memory
	int								numVerts;
	const triIndex_t* 				indexes;				// streamed in from main memory
	int								numIndexes;
	const silEdge_t*					silEdges;				// streamed in from main memory
	int								numSilEdges;
	const idJointMat* 				joints;					// in SPU local memory
	int								numJoints;
	idBounds						triangleBounds;
	idRenderMatrix					triangleMVP;
	idVec3							localLightOrigin;
	idVec3							localViewOrigin;
	idRenderMatrix					localLightProject;
	float							zNear;
	float							lightZMin;
	float							lightZMax;
	bool							cullShadowTrianglesToLight;
	bool							forceShadowCaps;
	bool							useShadowPreciseInsideTest;
	bool							useShadowDepthBounds;
	// temp
	byte* 							tempFacing;				// temp buffer in SPU local memory
	byte* 							tempCulled;				// temp buffer in SPU local memory
	idVec4* 						tempVerts;				// temp buffer in SPU local memory
	// output
	triIndex_t* 					indexBuffer;			// output buffer in SPU local memory
	triIndex_t* 					shadowIndices;			// streamed out to main memory
	int								maxShadowIndices;
	int* 							numShadowIndices;		// streamed out to main memory
	triIndex_t* 					lightIndices;			// streamed out to main memory
	int								maxLightIndices;
	int* 							numLightIndices;		// streamed out to main memory
	int* 							renderZFail;			// streamed out to main memory
	float* 							shadowZMin;				// streamed out to main memory
	float* 							shadowZMax;				// streamed out to main memory
	volatile shadowVolumeState_t* 	shadowVolumeState;		// streamed out to main memory
	// next in chain on view entity
	dynamicShadowVolumeParms_t* 	next;
	int								pad;
};


void DynamicShadowVolumeJob( const dynamicShadowVolumeParms_t* parms );

#endif // !__DYNAMICSHADOWVOLUME_H__
