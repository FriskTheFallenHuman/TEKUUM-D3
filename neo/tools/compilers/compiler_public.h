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

#ifndef __COMPILER_PUBLIC_H__
#define __COMPILER_PUBLIC_H__

/*
===============================================================================

	Compilers for map, model, video etc. processing.

===============================================================================
*/

// map processing (also see SuperOptimizeOccluders in tr_local.h)
void Dmap_f( const idCmdArgs& args );

// bump map generation
void RenderBump_f( const idCmdArgs& args );
void RenderBumpFlat_f( const idCmdArgs& args );

// AAS file compiler
void RunAAS_f( const idCmdArgs& args );
void RunAASDir_f( const idCmdArgs& args );
void RunReach_f( const idCmdArgs& args );

// video file encoding
void RoQFileEncode_f( const idCmdArgs& args );

#endif	/* !__COMPILER_PUBLIC_H__ */
