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

#ifndef __SIMPLEWIN_H__
#define __SIMPLEWIN_H__

class idUserInterfaceLocal;
class idDeviceContext;
class idSimpleWindow;

typedef struct
{
	idWindow* win;
	idSimpleWindow* simp;
} drawWin_t;

class idSimpleWindow
{
	friend class idWindow;
public:
	idSimpleWindow( idWindow* win );
	virtual			~idSimpleWindow();
	void			Redraw( float x, float y );
	void			StateChanged( bool redraw );

	idStr			name;

	idWinVar* 		GetWinVarByName( const char* _name );
	intptr_t		GetWinVarOffset( idWinVar* wv, drawWin_t* owner );
	size_t			Size();

	idWindow*		GetParent()
	{
		return mParent;
	}

	virtual void	WriteToSaveGame( idFile* savefile );
	virtual void	ReadFromSaveGame( idFile* savefile );

protected:
	void 			CalcClientRect( float xofs, float yofs );
	void 			SetupTransforms( float x, float y );
	void 			DrawBackground( const idRectangle& drawRect );
	void 			DrawBorderAndCaption( const idRectangle& drawRect );

	idUserInterfaceLocal* gui;
	int 			flags;
	idRectangle 	drawRect;			// overall rect
	idRectangle 	clientRect;			// client area
	idRectangle 	textRect;
	idVec2			origin;
	int 			fontNum;
	float 			matScalex;
	float 			matScaley;
	float 			borderSize;
	int 			textAlign;
	float 			textAlignx;
	float 			textAligny;
	int				textShadow;

	idWinStr		text;
	idWinBool		visible;
	idWinRectangle	rect;				// overall rect
	idWinVec4		backColor;
	idWinVec4		matColor;
	idWinVec4		foreColor;
	idWinVec4		borderColor;
	idWinFloat		textScale;
	idWinFloat		rotate;
	idWinVec2		shear;
	idWinBackground	backGroundName;

	const idMaterial* background;

	idWindow* 		mParent;

	idWinBool	hideCursor;
};

#endif /* !__SIMPLEWIN_H__ */
