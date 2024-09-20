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

#ifndef __MODEL_OBJ_H__
#define __MODEL_OBJ_H__

/*
===============================================================================

	Wavefront OBJ loader.
	This is meant to be a very simple model format and we don't even care for .mtl files
	because we want all material properties to be defined through the D3 material system

===============================================================================
*/

struct objObject_t
{
	idStrStatic< MAX_OSPATH >	material;

	idList<idVec3>				vertexes;
	idList<idVec2>				texcoords;
	idList<idVec3>				normals;
	idList<triIndex_t>			indexes;
};

struct objModel_t
{
	ID_TIME_T							timeStamp;
	idList<objObject_t*>		objects;
};


objModel_t* OBJ_Load( const char* fileName );
void		OBJ_Free( objModel_t* obj );

#endif /* !__MODEL_OBJ_H__ */
