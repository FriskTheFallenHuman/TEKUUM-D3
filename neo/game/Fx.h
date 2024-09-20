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

#ifndef __GAME_FX_H__
#define __GAME_FX_H__

/*
===============================================================================

  Special effects.

===============================================================================
*/

typedef struct
{
	renderLight_t			renderLight;			// light presented to the renderer
	qhandle_t				lightDefHandle;			// handle to renderer light def
	renderEntity_t			renderEntity;			// used to present a model to the renderer
	int						modelDefHandle;			// handle to static renderer model
	float					delay;
	int						particleSystem;
	int						start;
	bool					soundStarted;
	bool					shakeStarted;
	bool					decalDropped;
	bool					launched;
} idFXLocalAction;

class idEntityFx : public idEntity
{
public:
	CLASS_PROTOTYPE( idEntityFx );

	idEntityFx();
	virtual					~idEntityFx();

	void					Spawn();

	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );

	virtual void			Think();
	void					Setup( const char* fx );
	void					Run( int time );
	void					Start( int time );
	void					Stop();
	const int				Duration();
	const char* 			EffectName();
	const char* 			Joint();
	const bool				Done();

	virtual void			WriteToSnapshot( idBitMsgDelta& msg ) const;
	virtual void			ReadFromSnapshot( const idBitMsgDelta& msg );
	virtual void			ClientPredictionThink();

	static idEntityFx* 		StartFx( const char* fx, const idVec3* useOrigin, const idMat3* useAxis, idEntity* ent, bool bind );

protected:
	void					Event_Trigger( idEntity* activator );
	void					Event_ClearFx();

	void					CleanUp();
	void					CleanUpSingleAction( const idFXSingleAction& fxaction, idFXLocalAction& laction );
	void					ApplyFade( const idFXSingleAction& fxaction, idFXLocalAction& laction, const int time, const int actualStart );

	int						started;
	int						nextTriggerTime;
	const idDeclFX* 		fxEffect;				// GetFX() should be called before using fxEffect as a pointer
	idList<idFXLocalAction>	actions;
	idStr					systemName;
};

class idTeleporter : public idEntityFx
{
public:
	CLASS_PROTOTYPE( idTeleporter );

private:
	// teleporters to this location
	void					Event_DoAction( idEntity* activator );
};

#endif /* !__GAME_FX_H__ */
