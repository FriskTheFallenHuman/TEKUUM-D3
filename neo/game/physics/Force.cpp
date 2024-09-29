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

CLASS_DECLARATION( idClass, idForce )
END_CLASS

idList<idForce*> idForce::forceList;

/*
================
idForce::idForce
================
*/
idForce::idForce()
{
	forceList.Append( this );
}

/*
================
idForce::~idForce
================
*/
idForce::~idForce()
{
	forceList.Remove( this );
}

/*
================
idForce::DeletePhysics
================
*/
void idForce::DeletePhysics( const idPhysics* phys )
{
	int i;

	for( i = 0; i < forceList.Num(); i++ )
	{
		forceList[i]->RemovePhysics( phys );
	}
}

/*
================
idForce::ClearForceList
================
*/
void idForce::ClearForceList()
{
	forceList.Clear();
}

/*
================
idForce::Evaluate
================
*/
void idForce::Evaluate( int time )
{
}

/*
================
idForce::RemovePhysics
================
*/
void idForce::RemovePhysics( const idPhysics* phys )
{
}
