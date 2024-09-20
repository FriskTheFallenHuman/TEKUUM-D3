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
#ifndef __RENDERWINDOW_H
#define __RENDERWINDOW_H

class idUserInterfaceLocal;
class idRenderWindow : public idWindow
{
public:
	idRenderWindow( idUserInterfaceLocal* gui );
	virtual ~idRenderWindow();

	virtual void PostParse();
	virtual void Draw( int time, float x, float y );
	virtual size_t Allocated()
	{
		return idWindow::Allocated();
	};
//
//
	virtual idWinVar* GetWinVarByName( const char* _name, bool winLookup = false, drawWin_t** owner = NULL );
//

private:
	void CommonInit();
	virtual bool ParseInternalVar( const char* name, idTokenParser* src );
	void Render( int time );
	void PreRender();
	void BuildAnimation( int time );
	renderView_t refdef;
	idRenderWorld* world;
	renderEntity_t worldEntity;
	renderLight_t rLight;
	const idMD5Anim* modelAnim;

	qhandle_t	worldModelDef;
	qhandle_t	lightDef;
	qhandle_t   modelDef;
	idWinStr modelName;
	idWinStr animName;
	idStr	 animClass;
	idWinVec4 lightOrigin;
	idWinVec4 lightColor;
	idWinVec4 modelOrigin;
	idWinVec4 modelRotate;
	idWinVec4 viewOffset;
	idWinBool needsRender;
	int animLength;
	int animEndTime;
	bool updateAnimation;
};

#endif // __RENDERWINDOW_H
