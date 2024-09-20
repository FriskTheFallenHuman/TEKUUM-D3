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

struct guiModelSurface_t
{
	const idMaterial* 	material;
	uint64_t				glState;
	int					firstIndex;
	int					numIndexes;
	stereoDepthType_t		stereoType;
};

class idRenderMatrix;

/*
namespace ImGui
{
struct ImDrawData;
}
*/

class idGuiModel
{
public:
	idGuiModel();

	void		Clear();

	void		WriteToDemo( idDemoFile* demo );
	void		ReadFromDemo( idDemoFile* demo );

	// allocates memory for verts and indexes in frame-temporary buffer memory
	void		BeginFrame();

	void		EmitToCurrentView( float modelMatrix[16], bool depthHack );
	void		EmitFullScreen();

	// RB
	//void		EmitImGui( ImDrawData* drawData );

	// the returned pointer will be in write-combined memory, so only make contiguous
	// 32 bit writes and never read from it.
	idDrawVert* AllocTris( int numVerts, const triIndex_t* indexes, int numIndexes, const idMaterial* material,
						   const uint64_t glState, const stereoDepthType_t stereoType );

	//---------------------------
private:
	void		AdvanceSurf();
	void		EmitSurfaces( float modelMatrix[16], float modelViewMatrix[16], bool depthHack, bool allowFullScreenStereoDepth, bool linkAsEntity );

	guiModelSurface_t* 			surf;

	float						shaderParms[ MAX_ENTITY_SHADER_PARMS ];

	static const float STEREO_DEPTH_NEAR;
	static const float STEREO_DEPTH_MID;
	static const float STEREO_DEPTH_FAR;

	// if we exceed these limits we stop rendering GUI surfaces
	static const int MAX_INDEXES = ( 20000 * 6 );
	static const int MAX_VERTS	 = ( 20000 * 4 );

	vertCacheHandle_t			vertexBlock;
	vertCacheHandle_t			indexBlock;
	idDrawVert* 				vertexPointer;
	triIndex_t* 				indexPointer;

	int							numVerts;
	int							numIndexes;

	idList<guiModelSurface_t>	surfaces;
};

