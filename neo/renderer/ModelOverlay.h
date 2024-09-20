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

#ifndef __MODELOVERLAY_H__
#define __MODELOVERLAY_H__

/*
===============================================================================

	Overlays are used for adding decals on top of dynamic models.
	Projects an overlay onto deformable geometry and can be added to
	a render entity to allow decals on top of dynamic models.
	This does not generate tangent vectors, so it can't be used with
	light interaction shaders. Materials for overlays should always
	be clamped, because the projected texcoords can run well off the
	texture since no new clip vertexes are generated.
	Overlays with common materials will be merged together, but additional
	overlays will be allocated as needed. The material should not be
	one that receives lighting, because no interactions are generated
	for these lightweight surfaces.

===============================================================================
*/

static const int MAX_DEFERRED_OVERLAYS		= 4;
static const int DEFFERED_OVERLAY_TIMEOUT	= 200;	// don't create a overlay if it wasn't visible within the first 200 milliseconds
static const int MAX_OVERLAYS				= 8;

compile_time_assert( CONST_ISPOWEROFTWO( MAX_OVERLAYS ) );

struct overlayProjectionParms_t
{
	idPlane				localTextureAxis[2];
	const idMaterial* 	material;
	int					startTime;
};

struct overlayVertex_t
{
	int					vertexNum;
	halfFloat_t			st[2];
};

struct overlay_t
{
	int					surfaceNum;
	int					surfaceId;
	int					maxReferencedVertex;
	int					numIndexes;
	triIndex_t* 		indexes;
	int					numVerts;
	overlayVertex_t* 	verts;
	const idMaterial* 	material;
	mutable bool		writtenToDemo;
};

class idRenderModelOverlay
{
public:
	idRenderModelOverlay();
	~idRenderModelOverlay();

	void						ReUse();

	void						AddDeferredOverlay( const overlayProjectionParms_t& localParms );
	void						CreateDeferredOverlays( const idRenderModel* model );

	unsigned int				GetNumOverlayDrawSurfs();
	struct drawSurf_t* 			CreateOverlayDrawSurf( const struct viewEntity_t* space, const idRenderModel* baseModel, unsigned int index );

	void						ReadFromDemoFile( class idDemoFile* f );
	void						WriteToDemoFile( class idDemoFile* f ) const;

	int							index;
	int							demoSerialWrite;
	int							demoSerialCurrent;
private:
	overlay_t					overlays[MAX_OVERLAYS];
	unsigned int				firstOverlay;
	unsigned int				nextOverlay;

	overlayProjectionParms_t	deferredOverlays[MAX_DEFERRED_OVERLAYS];
	unsigned int				firstDeferredOverlay;
	unsigned int				nextDeferredOverlay;

	const idMaterial* 			overlayMaterials[MAX_OVERLAYS];
	unsigned int				numOverlayMaterials;

	void						CreateOverlay( const idRenderModel* model, const idPlane localTextureAxis[2], const idMaterial* material );
	void						FreeOverlay( overlay_t& overlay );
};

#endif /* !__MODELOVERLAY_H__ */
