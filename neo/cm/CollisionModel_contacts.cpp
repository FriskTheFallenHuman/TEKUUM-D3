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

/*
===============================================================================

	Trace model vs. polygonal model collision detection.

===============================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "CollisionModel_local.h"

/*
===============================================================================

Retrieving contacts

===============================================================================
*/

/*
==================
idCollisionModelManagerLocal::Contacts
==================
*/
int idCollisionModelManagerLocal::Contacts( contactInfo_t* contacts, const int maxContacts, const idVec3& start, const idVec6& dir, const float depth,
		const idTraceModel* trm, const idMat3& trmAxis, int contentMask,
		cmHandle_t model, const idVec3& origin, const idMat3& modelAxis )
{
	trace_t results;
	idVec3 end;

	// same as Translation but instead of storing the first collision we store all collisions as contacts
	idCollisionModelManagerLocal::getContacts = true;
	idCollisionModelManagerLocal::contacts = contacts;
	idCollisionModelManagerLocal::maxContacts = maxContacts;
	idCollisionModelManagerLocal::numContacts = 0;
	end = start + dir.SubVec3( 0 ) * depth;
	idCollisionModelManagerLocal::Translation( &results, start, end, trm, trmAxis, contentMask, model, origin, modelAxis );
	if( dir.SubVec3( 1 ).LengthSqr() != 0.0f )
	{
		// FIXME: rotational contacts
	}
	idCollisionModelManagerLocal::getContacts = false;
	idCollisionModelManagerLocal::maxContacts = 0;

	return idCollisionModelManagerLocal::numContacts;
}
