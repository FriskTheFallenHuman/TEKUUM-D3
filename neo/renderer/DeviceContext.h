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

#ifndef __DEVICECONTEXT_H__
#define __DEVICECONTEXT_H__

// device context support for gui stuff
//

const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 480;
const int BLINK_DIVISOR = 200;

class idDeviceContext
{
public:
	idDeviceContext();
	~idDeviceContext() { }

	void				Init();
	void				Shutdown();
	bool				Initialized()
	{
		return initialized;
	}
	void				EnableLocalization();

	void				GetTransformInfo( idVec3& origin, idMat3& mat );

	void				SetTransformInfo( const idVec3& origin, const idMat3& mat );
	void				DrawMaterial( float x, float y, float w, float h, const idMaterial* mat, const idVec4& color, float scalex = 1.0, float scaley = 1.0 );
	void				DrawRect( float x, float y, float width, float height, float size, const idVec4& color );
	void				DrawFilledRect( float x, float y, float width, float height, const idVec4& color );
	int					DrawText( const char* text, float textScale, int textAlign, idVec4 color, idRectangle rectDraw, bool wrap, int cursor = -1, bool calcOnly = false, idList<int>* breaks = NULL, int limit = 0 );
	void				DrawMaterialRect( float x, float y, float w, float h, float size, const idMaterial* mat, const idVec4& color );
	void				DrawStretchPic( float x, float y, float w, float h, float s0, float t0, float s1, float t1, const idMaterial* mat );
	void				DrawMaterialRotated( float x, float y, float w, float h, const idMaterial* mat, const idVec4& color, float scalex = 1.0, float scaley = 1.0, float angle = 0.0f );
	void				DrawStretchPicRotated( float x, float y, float w, float h, float s0, float t0, float s1, float t1, const idMaterial* mat, float angle = 0.0f );
	void				DrawWinding( idWinding& w, const idMaterial* mat );

	int					CharWidth( const char c, float scale );
	int					TextWidth( const char* text, float scale, int limit );
	int					TextHeight( const char* text, float scale, int limit );
	int					MaxCharHeight( float scale );
	int					MaxCharWidth( float scale );

	idRegion* 			GetTextRegion( const char* text, float textScale, idRectangle rectDraw, float xStart, float yStart );

	void				SetSize( float width, float height );
	void				SetOffset( float x, float y );

	const idMaterial* 	GetScrollBarImage( int index );

	void				DrawCursor( float* x, float* y, float size );
	void				SetCursor( int n );

	// clipping rects
	virtual bool		ClippedCoords( float* x, float* y, float* w, float* h, float* s1, float* t1, float* s2, float* t2 );
	virtual void		PushClipRect( idRectangle r );
	virtual void		PopClipRect();
	virtual void		EnableClipping( bool b );

	void				SetFont( int num );

	int					FindFont( const char* name );
	void				SetupFonts();

	void				SetOverStrike( bool b )
	{
		overStrikeMode = b;
	}

	bool				GetOverStrike()
	{
		return overStrikeMode;
	}

	void				DrawEditCursor( float x, float y, float scale );

	enum
	{
		CURSOR_ARROW,
		CURSOR_HAND,
		CURSOR_HAND_JOY1,
		CURSOR_HAND_JOY2,
		CURSOR_HAND_JOY3,
		CURSOR_HAND_JOY4,
		CURSOR_COUNT
	};

	enum
	{
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	enum
	{
		SCROLLBAR_HBACK,
		SCROLLBAR_VBACK,
		SCROLLBAR_THUMB,
		SCROLLBAR_RIGHT,
		SCROLLBAR_LEFT,
		SCROLLBAR_UP,
		SCROLLBAR_DOWN,
		SCROLLBAR_COUNT
	};

	static idVec4 colorPurple;
	static idVec4 colorOrange;
	static idVec4 colorYellow;
	static idVec4 colorGreen;
	static idVec4 colorBlue;
	static idVec4 colorRed;
	static idVec4 colorWhite;
	static idVec4 colorBlack;
	static idVec4 colorNone;

protected:
	virtual int			DrawText( float x, float y, float scale, idVec4 color, const char* text, float adjust, int limit, int style, int cursor = -1 );

	void				PaintChar( float x, float y, float width, float height, float scale, float	s, float	t, float	s2, float t2, const idMaterial* hShader );
	void				SetFontByScale( float scale );

	fontInfoEx_t*		activeFont;
	fontInfo_t*			useFont;
	idStr				fontName;

	static idList<fontInfoEx_t> fonts;
	idStr				fontLang;

	void				Clear();

	const idMaterial* 	cursorImages[CURSOR_COUNT];
	const idMaterial* 	scrollBarImages[SCROLLBAR_COUNT];
	const idMaterial* 	whiteImage;

	float				xScale;
	float				yScale;
	float				xOffset;
	float				yOffset;

	int					cursor;

	idList<idRectangle>	clipRects;

	bool				enableClipping;

	bool				overStrikeMode;

	idMat3				mat;
	bool				matIsIdentity;
	idVec3				origin;
	bool				initialized;
};

class idDeviceContextOptimized : public idDeviceContext
{

	virtual bool		ClippedCoords( float* x, float* y, float* w, float* h, float* s1, float* t1, float* s2, float* t2 );
	virtual void		PushClipRect( idRectangle r );
	virtual void		PopClipRect();
	virtual void		EnableClipping( bool b );

	float				clipX1;
	float				clipX2;
	float				clipY1;
	float				clipY2;
};

#endif /* !__DEVICECONTEXT_H__ */
