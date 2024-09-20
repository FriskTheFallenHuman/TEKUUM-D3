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

#include "PreLightShadowVolume_local.h"

/*
===================
PreLightShadowVolumeJob
===================
*/
void PreLightShadowVolumeJob( const preLightShadowVolumeParms_t* parms )
{
	if( parms->tempCullBits == NULL )
	{
		*const_cast< byte** >( &parms->tempCullBits ) = ( byte* )_alloca16( TEMP_CULLBITS( parms->numVerts ) );
	}

	// Calculate the shadow depth bounds.
	float shadowZMin = parms->lightZMin;
	float shadowZMax = parms->lightZMax;
	if( parms->useShadowDepthBounds )
	{
		// NOTE: pre-light shadow volumes typically cover the whole light volume so
		// there is no real point in trying to calculate tighter depth bounds here
		shadowZMin = Max( shadowZMin, parms->lightZMin );
		shadowZMax = Min( shadowZMax, parms->lightZMax );
	}

	bool renderZFail = false;
	int numShadowIndices = 0;

	// The shadow volume may be depth culled if either the shadow volume was culled to the view frustum or if the
	// depth range of the visible part of the shadow volume is outside the depth range of the light volume.
	if( shadowZMin < shadowZMax )
	{
		// Check if we need to render the shadow volume with Z-fail.
		// If the view is potentially inside the shadow volume bounds we may need to render with Z-fail.
		if( parms->triangleBounds.Expand( parms->zNear * INSIDE_SHADOW_VOLUME_EXTRA_STRETCH ).ContainsPoint( parms->localViewOrigin ) )
		{
			// Optionally perform a more precise test to see whether or not the view is inside the shadow volume.
			if( parms->useShadowPreciseInsideTest && parms->verts != NULL && parms->indexes != NULL )
			{
				renderZFail = R_ViewInsideShadowVolume( parms->tempCullBits, parms->verts, parms->numVerts, parms->indexes, parms->numIndexes,
														parms->localLightOrigin, parms->localViewOrigin, parms->zNear * INSIDE_SHADOW_VOLUME_EXTRA_STRETCH );
			}
			else
			{
				renderZFail = true;
			}
		}

		// must always draw the caps because pre-light shadows do not project to infinity
		numShadowIndices = parms->numIndexes;
	}

	// write out the number of shadow indices
	if( parms->numShadowIndices != NULL )
	{
		*parms->numShadowIndices = numShadowIndices;
	}
	// write out whether or not the shadow volume needs to be rendered with Z-Fail
	if( parms->renderZFail != NULL )
	{
		*parms->renderZFail = renderZFail;
	}
	// write out the shadow depth bounds
	if( parms->shadowZMin != NULL )
	{
		*parms->shadowZMin = shadowZMin;
	}
	if( parms->shadowZMax != NULL )
	{
		*parms->shadowZMax = shadowZMax;
	}
	// write out the shadow volume state
	if( parms->shadowVolumeState != NULL )
	{
		*parms->shadowVolumeState = SHADOWVOLUME_DONE;
	}
}

REGISTER_PARALLEL_JOB( PreLightShadowVolumeJob, "PreLightShadowVolumeJob" );
