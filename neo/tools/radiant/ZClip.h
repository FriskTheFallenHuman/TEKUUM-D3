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

#ifndef ZCLIP_H
#define ZCLIP_H

// I don't like doing macros without braces and with whitespace, but the compiler moans if I do these differently,
//	and since they're only for use within glColor3f() calls anyway then this is ok...  (that's my excuse anyway)
//
#define ZCLIP_COLOUR		1.0f, 0.0f, 1.0f
#define ZCLIP_COLOUR_DIM	0.8f, 0.0f, 0.8f


class CZClip
{
public:
	CZClip();
	~CZClip();

	int		GetTop();
	int		GetBottom();
	void	SetTop( int iNewZ );
	void	SetBottom( int iNewZ );
	void	Reset();
	bool	IsEnabled();
	bool	Enable( bool bOnOff );
	void	Paint();

protected:
	void	Legalise();

	bool	m_bEnabled;
	int		m_iZClipTop;
	int		m_iZClipBottom;
};


#endif	// #ifndef ZCLIP_H


///////////// eof ///////////////
