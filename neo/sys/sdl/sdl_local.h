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
#ifndef __SDL_LOCAL_H__
#define __SDL_LOCAL_H__

// glimp.cpp
const int GRAB_ENABLE		= ( 1 << 0 );
const int GRAB_REENABLE		= ( 1 << 1 );
const int GRAB_HIDECURSOR	= ( 1 << 2 );
const int GRAB_SETSTATE		= ( 1 << 3 );

extern idCVar in_nograb;

void GLimp_GrabInput( int flags );

char*	Posix_ConsoleInput();

#endif
