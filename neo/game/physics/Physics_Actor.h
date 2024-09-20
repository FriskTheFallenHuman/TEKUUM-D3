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

#ifndef __PHYSICS_ACTOR_H__
#define __PHYSICS_ACTOR_H__

/*
===================================================================================

	Actor physics base class

	An actor typically uses one collision model which is aligned with the gravity
	direction. The collision model is usually a simple box with the origin at the
	bottom center.

===================================================================================
*/

class idPhysics_Actor : public idPhysics_Base
{

public:
	CLASS_PROTOTYPE( idPhysics_Actor );

	idPhysics_Actor();
	~idPhysics_Actor();

	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );

	// get delta yaw of master
	float					GetMasterDeltaYaw() const;
	// returns the ground entity
	idEntity* 				GetGroundEntity() const;
	// align the clip model with the gravity direction
	void					SetClipModelAxis();

public:	// common physics interface
	void					SetClipModel( idClipModel* model, float density, int id = 0, bool freeOld = true );
	idClipModel* 			GetClipModel( int id = 0 ) const;
	int						GetNumClipModels() const;

	void					SetMass( float mass, int id = -1 );
	float					GetMass( int id = -1 ) const;

	void					SetContents( int contents, int id = -1 );
	int						GetContents( int id = -1 ) const;

	const idBounds& 		GetBounds( int id = -1 ) const;
	const idBounds& 		GetAbsBounds( int id = -1 ) const;

	bool					IsPushable() const;

	const idVec3& 			GetOrigin( int id = 0 ) const;
	const idMat3& 			GetAxis( int id = 0 ) const;

	void					SetGravity( const idVec3& newGravity );
	const idMat3& 			GetGravityAxis() const;

	void					ClipTranslation( trace_t& results, const idVec3& translation, const idClipModel* model ) const;
	void					ClipRotation( trace_t& results, const idRotation& rotation, const idClipModel* model ) const;
	int						ClipContents( const idClipModel* model ) const;

	void					DisableClip();
	void					EnableClip();

	void					UnlinkClip();
	void					LinkClip();

	bool					EvaluateContacts();

protected:
	idClipModel* 			clipModel;			// clip model used for collision detection
	idMat3					clipModelAxis;		// axis of clip model aligned with gravity direction

	// derived properties
	float					mass;
	float					invMass;

	// master
	idEntity* 				masterEntity;
	float					masterYaw;
	float					masterDeltaYaw;

	// results of last evaluate
	idEntityPtr<idEntity>	groundEntityPtr;
};

#endif /* !__PHYSICS_ACTOR_H__ */
