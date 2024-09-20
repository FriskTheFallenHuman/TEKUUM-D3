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

#ifndef __GAME_SECURITYCAMERA_H__
#define __GAME_SECURITYCAMERA_H__

/*
===================================================================================

	Security camera

===================================================================================
*/


class idSecurityCamera : public idEntity
{
public:
	CLASS_PROTOTYPE( idSecurityCamera );

	void					Spawn();

	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );

	virtual void			Think();

	virtual renderView_t* 	GetRenderView();
	virtual void			Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
	virtual bool			Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
	virtual void			Present();


private:

	enum { SCANNING, LOSINGINTEREST, ALERT, ACTIVATED };

	float					angle;
	float					sweepAngle;
	int						modelAxis;
	bool					flipAxis;
	float					scanDist;
	float					scanFov;

	float					sweepStart;
	float					sweepEnd;
	bool					negativeSweep;
	bool					sweeping;
	int						alertMode;
	float					stopSweeping;
	float					scanFovCos;

	idVec3					viewOffset;

	int						pvsArea;
	idPhysics_RigidBody		physicsObj;
	idTraceModel			trm;

	void					StartSweep();
	bool					CanSeePlayer();
	void					SetAlertMode( int status );
	void					DrawFov();
	const idVec3			GetAxis() const;
	float					SweepSpeed() const;

	void					Event_ReverseSweep();
	void					Event_ContinueSweep();
	void					Event_Pause();
	void					Event_Alert();
	void					Event_AddLight();
};

#endif /* !__GAME_SECURITYCAMERA_H__ */
