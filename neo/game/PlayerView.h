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

#ifndef __GAME_PLAYERVIEW_H__
#define __GAME_PLAYERVIEW_H__

/*
===============================================================================

  Player view.

===============================================================================
*/

// screenBlob_t are for the on-screen damage claw marks, etc
typedef struct
{
	const idMaterial* 	material;
	float				x, y, w, h;
	float				s1, t1, s2, t2;
	int					finishTime;
	int					startFadeTime;
	float				driftAmount;
} screenBlob_t;

#define	MAX_SCREEN_BLOBS	8

class idPlayerView
{
public:
	idPlayerView();

	void				Save( idSaveGame* savefile ) const;
	void				Restore( idRestoreGame* savefile );

	void				SetPlayerEntity( class idPlayer* playerEnt );

	void				ClearEffects();

	void				DamageImpulse( idVec3 localKickDir, const idDict* damageDef );

	void				WeaponFireFeedback( const idDict* weaponDef );

	idAngles			AngleOffset() const;			// returns the current kick angle

	idMat3				ShakeAxis() const;			// returns the current shake angle

	void				CalculateShake();

	// this may involve rendering to a texture and displaying
	// that with a warp model or in double vision mode
	void				RenderPlayerView( idUserInterface* hud );

	void				Fade( idVec4 color, int time );

	void				Flash( idVec4 color, int time );

	void				AddBloodSpray( float duration );

	// temp for view testing
	void				EnableBFGVision( bool b )
	{
		bfgVision = b;
	};

private:
	void				SingleView( idUserInterface* hud, const renderView_t* view );
	void				DoubleVision( idUserInterface* hud, const renderView_t* view, int offset );
	void				BerserkVision( idUserInterface* hud, const renderView_t* view );
	void				InfluenceVision( idUserInterface* hud, const renderView_t* view );
	void				ScreenFade();

	screenBlob_t* 		GetScreenBlob();

	screenBlob_t		screenBlobs[MAX_SCREEN_BLOBS];

	int					dvFinishTime;		// double vision will be stopped at this time
	const idMaterial* 	dvMaterial;			// material to take the double vision screen shot

	int					kickFinishTime;		// view kick will be stopped at this time
	idAngles			kickAngles;

	bool				bfgVision;			//

	const idMaterial* 	tunnelMaterial;		// health tunnel vision
	const idMaterial* 	armorMaterial;		// armor damage view effect
	const idMaterial* 	berserkMaterial;	// berserk effect
	const idMaterial* 	irGogglesMaterial;	// ir effect
	const idMaterial* 	bloodSprayMaterial; // blood spray
	const idMaterial* 	bfgMaterial;		// when targeted with BFG
	const idMaterial* 	lagoMaterial;		// lagometer drawing
	float				lastDamageTime;		// accentuate the tunnel effect for a while

	idVec4				fadeColor;			// fade color
	idVec4				fadeToColor;		// color to fade to
	idVec4				fadeFromColor;		// color to fade from
	float				fadeRate;			// fade rate
	int					fadeTime;			// fade time

	idAngles			shakeAng;			// from the sound sources

	idPlayer* 			player;
	renderView_t		view;
};

#endif /* !__GAME_PLAYERVIEW_H__ */
