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

#include "../Game_local.h"

ABSTRACT_DECLARATION( idClass, idPhysics )
END_CLASS


/*
================
idPhysics::~idPhysics
================
*/
idPhysics::~idPhysics()
{
}

/*
================
idPhysics::Save
================
*/
void idPhysics::Save( idSaveGame* savefile ) const
{
}

/*
================
idPhysics::Restore
================
*/
void idPhysics::Restore( idRestoreGame* savefile )
{
}

/*
================
idPhysics::SetClipBox
================
*/
void idPhysics::SetClipBox( const idBounds& bounds, float density )
{
	SetClipModel( new idClipModel( idTraceModel( bounds ) ), density );
}

/*
================
idPhysics::SnapTimeToPhysicsFrame
================
*/
int idPhysics::SnapTimeToPhysicsFrame( int t )
{
	return MSEC_ALIGN_TO_FRAME( t );
}
