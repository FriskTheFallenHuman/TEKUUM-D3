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

// a texturename of the form (0 0 0) will
// create a solid color texture

void		Texture_ShowInuse();
void		Texture_ShowAll();
void		Texture_HideAll();

// TTimo: added bNoAlpha flag to ignore alpha channel when parsing a .TGA file, transparency is usually achieved through qer_trans keyword in shaders
// in some cases loading an empty alpha channel causes display bugs (brushes not seen)
//qtexture_t *Texture_ForName (const char *name, bool bReplace = false, bool bShader = false, bool bNoAlpha = false, bool bReload = false, bool makeShader = true);

const idMaterial* Texture_ForName( const char* name );

void		Texture_SetTexture( texdef_t* texdef, brushprimit_texdef_t* brushprimit_texdef, bool bFitScale = false, bool bSetSelection = true );

void		Texture_SetMode( int iMenu );	// GL_TEXTURE_NEAREST, etc..