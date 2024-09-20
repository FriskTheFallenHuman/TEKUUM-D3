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

#ifndef __PHYSICS_PLAYER_H__
#define __PHYSICS_PLAYER_H__

/*
===================================================================================

	Player physics

	Simulates the motion of a player through the environment. Input from the
	player is used to allow a certain degree of control over the motion.

===================================================================================
*/

// movementType
typedef enum
{
	PM_NORMAL,				// normal physics
	PM_DEAD,				// no acceleration or turning, but free falling
	PM_SPECTATOR,			// flying without gravity but with collision detection
	PM_FREEZE,				// stuck in place without control
	PM_NOCLIP				// flying without collision detection nor gravity
} pmtype_t;

typedef enum
{
	WATERLEVEL_NONE,
	WATERLEVEL_FEET,
	WATERLEVEL_WAIST,
	WATERLEVEL_HEAD
} waterLevel_t;

#define	MAXTOUCH					32

typedef struct playerPState_s
{
	idVec3					origin;
	idVec3					velocity;
	idVec3					localOrigin;
	idVec3					pushVelocity;
	float					stepUp;
	int						movementType;
	int						movementFlags;
	int						movementTime;

	playerPState_s() :
		origin( vec3_zero ),
		velocity( vec3_zero ),
		localOrigin( vec3_zero ),
		pushVelocity( vec3_zero ),
		stepUp( 0.0f ),
		movementType( 0 ),
		movementFlags( 0 ),
		movementTime( 0 )
	{
	}
} playerPState_t;

class idPhysics_Player : public idPhysics_Actor
{

public:
	CLASS_PROTOTYPE( idPhysics_Player );

	idPhysics_Player();

	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );

	// initialisation
	void					SetSpeed( const float newWalkSpeed, const float newCrouchSpeed );
	void					SetMaxStepHeight( const float newMaxStepHeight );
	float					GetMaxStepHeight() const;
	void					SetMaxJumpHeight( const float newMaxJumpHeight );
	void					SetMovementType( const pmtype_t type );
	void					SetPlayerInput( const usercmd_t& cmd, const idAngles& newViewAngles );
	void					SetKnockBack( const int knockBackTime );
	void					SetDebugLevel( bool set );
	// feed back from last physics frame
	waterLevel_t			GetWaterLevel() const;
	int						GetWaterType() const;
	bool					HasJumped() const;
	bool					HasSteppedUp() const;
	float					GetStepUp() const;
	bool					IsCrouching() const;
	bool					OnLadder() const;
	const idVec3& 			PlayerGetOrigin() const;	// != GetOrigin

public:	// common physics interface
	bool					Evaluate( int timeStepMSec, int endTimeMSec );
	void					UpdateTime( int endTimeMSec );
	int						GetTime() const;

	void					GetImpactInfo( const int id, const idVec3& point, impactInfo_t* info ) const;
	void					ApplyImpulse( const int id, const idVec3& point, const idVec3& impulse );
	bool					IsAtRest() const;
	int						GetRestStartTime() const;

	void					SaveState();
	void					RestoreState();

	void					SetOrigin( const idVec3& newOrigin, int id = -1 );
	void					SetAxis( const idMat3& newAxis, int id = -1 );

	void					Translate( const idVec3& translation, int id = -1 );
	void					Rotate( const idRotation& rotation, int id = -1 );

	void					SetLinearVelocity( const idVec3& newLinearVelocity, int id = 0 );

	const idVec3& 			GetLinearVelocity( int id = 0 ) const;

	void					SetPushed( int deltaTime );
	const idVec3& 			GetPushedLinearVelocity( const int id = 0 ) const;
	void					ClearPushedVelocity();

	void					SetMaster( idEntity* master, const bool orientated = true );

	void					WriteToSnapshot( idBitMsgDelta& msg ) const;
	void					ReadFromSnapshot( const idBitMsgDelta& msg );

private:
	// player physics state
	playerPState_t			current;
	playerPState_t			saved;

	// properties
	float					walkSpeed;
	float					crouchSpeed;
	float					maxStepHeight;
	float					maxJumpHeight;
	int						debugLevel;				// if set, diagnostic output will be printed

	// player input
	usercmd_t				command;
	idAngles				viewAngles;

	// run-time variables
	int						framemsec;
	float					frametime;
	float					playerSpeed;
	idVec3					viewForward;
	idVec3					viewRight;

	// walk movement
	bool					walking;
	bool					groundPlane;
	trace_t					groundTrace;
	const idMaterial* 		groundMaterial;

	// ladder movement
	bool					ladder;
	idVec3					ladderNormal;

	// results of last evaluate
	waterLevel_t			waterLevel;
	int						waterType;

private:
	float					CmdScale( const usercmd_t& cmd ) const;
	void					Accelerate( const idVec3& wishdir, const float wishspeed, const float accel );
	bool					SlideMove( bool gravity, bool stepUp, bool stepDown, bool push );
	void					Friction();
	void					WaterJumpMove();
	void					WaterMove();
	void					FlyMove();
	void					AirMove();
	void					WalkMove();
	void					DeadMove();
	void					NoclipMove();
	void					SpectatorMove();
	void					LadderMove();
	void					CorrectAllSolid( trace_t& trace, int contents );
	void					CheckGround();
	void					CheckDuck();
	void					CheckLadder();
	bool					CheckJump();
	bool					CheckWaterJump();
	void					SetWaterLevel();
	void					DropTimers();
	void					MovePlayer( int msec );
};

#endif /* !__PHYSICS_PLAYER_H__ */
