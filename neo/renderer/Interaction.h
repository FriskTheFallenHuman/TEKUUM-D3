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

#ifndef __INTERACTION_H__
#define __INTERACTION_H__

/*
===============================================================================

	Interaction between static entityDef surfaces and a static lightDef.

	Interactions with no lightTris and no shadowTris are still
	valid, because they show that a given entityDef / lightDef
	do not interact, even though they share one or more areas.

===============================================================================
*/

#define LIGHT_CULL_ALL_FRONT		((byte *)-1)
#define	LIGHT_CLIP_EPSILON			0.1f

// enabling this define allows the precise inside shadow volume test
// to be performed on interaction (static) shadow volumes
#define KEEP_INTERACTION_CPU_DATA

struct srfCullInfo_t
{
	// For each triangle a byte set to 1 if facing the light origin.
	byte* 					facing;

	// For each vertex a byte with the bits [0-5] set if the
	// vertex is at the back side of the corresponding clip plane.
	// If the 'cullBits' pointer equals LIGHT_CULL_ALL_FRONT all
	// vertices are at the front of all the clip planes.
	byte* 					cullBits;

	// Clip planes in surface space used to calculate the cull bits.
	idPlane					localClipPlanes[6];
};


// Pre-generated shadow volumes from dmap are not present in surfaceInteraction_t,
// they are added separately.
struct surfaceInteraction_t
{
	// The vertexes for light tris will always come from ambient triangles.
	// For interactions created at load time, the indexes will be uniquely
	// generated in static vertex memory.
	int						numLightTrisIndexes;
	vertCacheHandle_t		lightTrisIndexCache;

	// shadow volume triangle surface
	int						numShadowIndexes;
	int						numShadowIndexesNoCaps;	// if the view is outside the shadow, this can be used
	triIndex_t* 			shadowIndexes;			// only != NULL if KEEP_INTERACTION_CPU_DATA is defined
	vertCacheHandle_t		shadowIndexCache;
};


class idRenderEntityLocal;
class idRenderLightLocal;
class RenderEnvprobeLocal;

class idInteraction
{
public:
	// this may be 0 if the light and entity do not actually intersect
	// -1 = an untested interaction
	int						numSurfaces;

	// if there is a whole-entity optimized shadow hull, it will
	// be present as a surfaceInteraction_t with a NULL ambientTris, but
	// possibly having a shader to specify the shadow sorting order
	// (FIXME: actually try making shadow hulls?  we never did.)
	surfaceInteraction_t* 	surfaces;

	// get space from here, if NULL, it is a pre-generated shadow volume from dmap
	idRenderEntityLocal* 	entityDef;
	idRenderLightLocal* 	lightDef;

	idInteraction* 			lightNext;				// for lightDef chains
	idInteraction* 			lightPrev;
	idInteraction* 			entityNext;				// for entityDef chains
	idInteraction* 			entityPrev;

	bool					staticInteraction;		// true if the interaction was created at map load time in static buffer space

public:
	idInteraction();

	// because these are generated and freed each game tic for active elements all
	// over the world, we use a custom pool allocater to avoid memory allocation overhead
	// and fragmentation
	static idInteraction* 	AllocAndLink( idRenderEntityLocal* edef, idRenderLightLocal* ldef );

	// unlinks from the entity and light, frees all surfaceInteractions,
	// and puts it back on the free list
	void					UnlinkAndFree();

	// free the interaction surfaces
	void					FreeSurfaces();

	// makes the interaction empty for when the light and entity do not actually intersect
	// all empty interactions are linked at the end of the light's and entity's interaction list
	void					MakeEmpty();

	// returns true if the interaction is empty
	bool					IsEmpty() const
	{
		return ( numSurfaces == 0 );
	}

	// returns true if the interaction is not yet completely created
	bool					IsDeferred() const
	{
		return ( numSurfaces == -1 );
	}

	// returns true if the interaction has shadows
	bool					HasShadows() const;

	// called by GenerateAllInteractions
	void					CreateStaticInteraction();

private:
	// unlink from entity and light lists
	void					Unlink();
};

void R_ShowInteractionMemory_f( const idCmdArgs& args );

#endif /* !__INTERACTION_H__ */
