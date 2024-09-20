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

CLASS_DECLARATION( idForce, idForce_Drag )
END_CLASS

/*
================
idForce_Drag::idForce_Drag
================
*/
idForce_Drag::idForce_Drag()
{
	damping			= 0.5f;
	dragPosition	= vec3_zero;
	physics			= NULL;
	id				= 0;
	p				= vec3_zero;
	dragPosition	= vec3_zero;
}

/*
================
idForce_Drag::~idForce_Drag
================
*/
idForce_Drag::~idForce_Drag()
{
}

/*
================
idForce_Drag::Init
================
*/
void idForce_Drag::Init( float damping )
{
	if( damping >= 0.0f && damping < 1.0f )
	{
		this->damping = damping;
	}
}

/*
================
idForce_Drag::SetPhysics
================
*/
void idForce_Drag::SetPhysics( idPhysics* phys, int id, const idVec3& p )
{
	this->physics = phys;
	this->id = id;
	this->p = p;
}

/*
================
idForce_Drag::SetDragPosition
================
*/
void idForce_Drag::SetDragPosition( const idVec3& pos )
{
	this->dragPosition = pos;
}

/*
================
idForce_Drag::GetDragPosition
================
*/
const idVec3& idForce_Drag::GetDragPosition() const
{
	return this->dragPosition;
}

/*
================
idForce_Drag::GetDraggedPosition
================
*/
const idVec3 idForce_Drag::GetDraggedPosition() const
{
	return ( physics->GetOrigin( id ) + p * physics->GetAxis( id ) );
}

/*
================
idForce_Drag::Evaluate
================
*/
void idForce_Drag::Evaluate( int time )
{
	float l1, l2, mass;
	idVec3 dragOrigin, dir1, dir2, velocity, centerOfMass;
	idMat3 inertiaTensor;
	idRotation rotation;
	idClipModel* clipModel;

	if( !physics )
	{
		return;
	}

	clipModel = physics->GetClipModel( id );
	if( clipModel != NULL && clipModel->IsTraceModel() )
	{
		clipModel->GetMassProperties( 1.0f, mass, centerOfMass, inertiaTensor );
	}
	else
	{
		centerOfMass.Zero();
	}

	centerOfMass = physics->GetOrigin( id ) + centerOfMass * physics->GetAxis( id );
	dragOrigin = physics->GetOrigin( id ) + p * physics->GetAxis( id );

	dir1 = dragPosition - centerOfMass;
	dir2 = dragOrigin - centerOfMass;
	l1 = dir1.Normalize();
	l2 = dir2.Normalize();

	rotation.Set( centerOfMass, dir2.Cross( dir1 ), RAD2DEG( idMath::ACos( dir1 * dir2 ) ) );
	physics->SetAngularVelocity( rotation.ToAngularVelocity() / MS2SEC( gameLocal.time - gameLocal.previousTime ), id );

	velocity = physics->GetLinearVelocity( id ) * damping + dir1 * ( ( l1 - l2 ) * ( 1.0f - damping ) / MS2SEC( gameLocal.time - gameLocal.previousTime ) );
	physics->SetLinearVelocity( velocity, id );
}

/*
================
idForce_Drag::RemovePhysics
================
*/
void idForce_Drag::RemovePhysics( const idPhysics* phys )
{
	if( physics == phys )
	{
		physics = NULL;
	}
}
