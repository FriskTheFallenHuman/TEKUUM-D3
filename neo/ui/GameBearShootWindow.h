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
#ifndef __GAME_BEARSHOOT_WINDOW_H__
#define __GAME_BEARSHOOT_WINDOW_H__

class idGameBearShootWindow;

class BSEntity
{
public:
	const idMaterial* 		material;
	idStr					materialName;
	float					width, height;
	bool					visible;

	idVec4					entColor;
	idVec2					position;
	float					rotation;
	float					rotationSpeed;
	idVec2					velocity;

	bool					fadeIn;
	bool					fadeOut;

	idGameBearShootWindow* 	game;

public:
	BSEntity( idGameBearShootWindow* _game );
	virtual				~BSEntity();

	virtual void		WriteToSaveGame( idFile* savefile );
	virtual void		ReadFromSaveGame( idFile* savefile, idGameBearShootWindow* _game );

	void				SetMaterial( const char* name );
	void				SetSize( float _width, float _height );
	void				SetVisible( bool isVisible );

	virtual void		Update( float timeslice );
	virtual void		Draw();

private:
};


class idGameBearShootWindow : public idWindow
{
public:
	idGameBearShootWindow( idUserInterfaceLocal* gui );
	~idGameBearShootWindow();

	virtual void		WriteToSaveGame( idFile* savefile );
	virtual void		ReadFromSaveGame( idFile* savefile );

	virtual const char*	HandleEvent( const sysEvent_t* event, bool* updateVisuals );
	virtual void		PostParse();
	virtual void		Draw( int time, float x, float y );
	virtual idWinVar* 	GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );

private:
	void				CommonInit();
	void				ResetGameState();

	void				UpdateBear();
	void				UpdateHelicopter();
	void				UpdateTurret();
	void				UpdateButtons();
	void				UpdateGame();
	void				UpdateScore();

	virtual bool		ParseInternalVar( const char* name, idTokenParser* src );

private:

	idWinBool			gamerunning;
	idWinBool			onFire;
	idWinBool			onContinue;
	idWinBool			onNewGame;

	float				timeSlice;
	float				timeRemaining;
	bool				gameOver;

	int					currentLevel;
	int					goalsHit;
	bool				updateScore;
	bool				bearHitTarget;

	float				bearScale;
	bool				bearIsShrinking;
	int					bearShrinkStartTime;

	float				turretAngle;
	float				turretForce;

	float				windForce;
	int					windUpdateTime;

	idList<BSEntity*>	entities;

	BSEntity*			turret;
	BSEntity*			bear;
	BSEntity*			helicopter;
	BSEntity*			goal;
	BSEntity*			wind;
	BSEntity*			gunblast;
};

#endif //__GAME_BEARSHOOT_WINDOW_H__
