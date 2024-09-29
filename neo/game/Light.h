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

#ifndef __GAME_LIGHT_H__
#define __GAME_LIGHT_H__

/*
===============================================================================

  Generic light.

===============================================================================
*/

extern const idEventDef EV_Light_GetLightParm;
extern const idEventDef EV_Light_SetLightParm;
extern const idEventDef EV_Light_SetLightParms;

class idLight : public idEntity
{
public:
	CLASS_PROTOTYPE( idLight );

	idLight();
	~idLight();

	void			Spawn();

	void			Save( idSaveGame* savefile ) const;					// archives object for save game file
	void			Restore( idRestoreGame* savefile );					// unarchives object from save game file

	virtual void	UpdateChangeableSpawnArgs( const idDict* source );
	virtual void	Think();
	virtual void	FreeLightDef();
	virtual bool	GetPhysicsToSoundTransform( idVec3& origin, idMat3& axis );
	void			Present();

	void			SaveState( idDict* args );
	virtual void	SetColor( float red, float green, float blue );
	virtual void	SetColor( const idVec3& color );
	virtual void	SetColor( const idVec4& color );
	virtual void	GetColor( idVec3& out ) const;
	virtual void	GetColor( idVec4& out ) const;
	const idVec3& 	GetBaseColor() const
	{
		return baseColor;
	}
	virtual idVec3	GetEditOrigin() const;
	void			SetShader( const char* shadername );
	void			SetLightParm( int parmnum, float value );
	void			SetLightParms( float parm0, float parm1, float parm2, float parm3 );
	void			SetRadiusXYZ( float x, float y, float z );
	void			SetRadius( float radius );
	void			On();
	void			Off();
	void			Fade( const idVec4& to, float fadeTime );
	void			FadeOut( float time );
	void			FadeIn( float time );
	void			Killed( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
	void			BecomeBroken( idEntity* activator );
	qhandle_t		GetLightDefHandle() const
	{
		return lightDefHandle;
	}
	void			SetLightParent( idEntity* lparent )
	{
		lightParent = lparent;
	}
	void			SetLightLevel();

	virtual void	ShowEditingDialog();

	const renderLight_t& GetRenderLight() const
	{
		return renderLight;
	}

	enum
	{
		EVENT_BECOMEBROKEN = idEntity::EVENT_MAXEVENTS,
		EVENT_MAXEVENTS
	};

	virtual void	ClientPredictionThink();
	virtual void	WriteToSnapshot( idBitMsgDelta& msg ) const;
	virtual void	ReadFromSnapshot( const idBitMsgDelta& msg );
	virtual bool	ClientReceiveEvent( int event, int time, const idBitMsg& msg );

private:
	renderLight_t	renderLight;				// light presented to the renderer
	idVec3			localLightOrigin;			// light origin relative to the physics origin
	idMat3			localLightAxis;				// light axis relative to physics axis
	qhandle_t		lightDefHandle;				// handle to renderer light def
	idStr			brokenModel;
	int				levels;
	int				currentLevel;
	idVec3			baseColor;
	bool			breakOnTrigger;
	int				count;
	int				triggercount;
	idEntity* 		lightParent;
	idVec4			fadeFrom;
	idVec4			fadeTo;
	int				fadeStart;
	int				fadeEnd;
	bool			soundWasPlaying;

private:
	void			PresentLightDefChange();
	void			PresentModelDefChange();

	void			Event_SetShader( const char* shadername );
	void			Event_GetLightParm( int parmnum );
	void			Event_SetLightParm( int parmnum, float value );
	void			Event_SetLightParms( float parm0, float parm1, float parm2, float parm3 );
	void			Event_SetRadiusXYZ( float x, float y, float z );
	void			Event_SetRadius( float radius );
	void			Event_Hide();
	void			Event_Show();
	void			Event_On();
	void			Event_Off();
	void			Event_ToggleOnOff( idEntity* activator );
	void			Event_SetSoundHandles();
	void			Event_FadeOut( float time );
	void			Event_FadeIn( float time );
};

#endif /* !__GAME_LIGHT_H__ */
