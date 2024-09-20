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

/*
============
idShadowVert::CreateShadowCache
============
*/
int idShadowVert::CreateShadowCache( idShadowVert* vertexCache, const idDrawVert* verts, const int numVerts )
{
	for( int i = 0; i < numVerts; i++ )
	{
		vertexCache[i * 2 + 0].xyzw[0] = verts[i].xyz[0];
		vertexCache[i * 2 + 0].xyzw[1] = verts[i].xyz[1];
		vertexCache[i * 2 + 0].xyzw[2] = verts[i].xyz[2];
		vertexCache[i * 2 + 0].xyzw[3] = 1.0f;

		vertexCache[i * 2 + 1].xyzw[0] = verts[i].xyz[0];
		vertexCache[i * 2 + 1].xyzw[1] = verts[i].xyz[1];
		vertexCache[i * 2 + 1].xyzw[2] = verts[i].xyz[2];
		vertexCache[i * 2 + 1].xyzw[3] = 0.0f;
	}
	return numVerts * 2;
}

/*
===================
idShadowVertSkinned::CreateShadowCache
===================
*/
int idShadowVertSkinned::CreateShadowCache( idShadowVertSkinned* vertexCache, const idDrawVert* verts, const int numVerts )
{
	for( int i = 0; i < numVerts; i++ )
	{
		vertexCache[0].xyzw[0] = verts[i].xyz[0];
		vertexCache[0].xyzw[1] = verts[i].xyz[1];
		vertexCache[0].xyzw[2] = verts[i].xyz[2];
		vertexCache[0].xyzw[3] = 1.0f;
		*( unsigned int* )vertexCache[0].color = *( unsigned int* )verts[i].color;
		*( unsigned int* )vertexCache[0].color2 = *( unsigned int* )verts[i].color2;

		vertexCache[1].xyzw[0] = verts[i].xyz[0];
		vertexCache[1].xyzw[1] = verts[i].xyz[1];
		vertexCache[1].xyzw[2] = verts[i].xyz[2];
		vertexCache[1].xyzw[3] = 0.0f;
		*( unsigned int* )vertexCache[1].color = *( unsigned int* )verts[i].color;
		*( unsigned int* )vertexCache[1].color2 = *( unsigned int* )verts[i].color2;

		vertexCache += 2;
	}
	return numVerts * 2;
}
