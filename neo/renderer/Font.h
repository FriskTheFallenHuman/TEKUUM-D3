/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#ifndef __FONT_H__
#define __FONT_H__

#if defined(USE_IDFONT)

struct scaledGlyphInfo_t
{
	float	top, left;
	float	width, height;
	float	xSkip;
	float	s1, t1, s2, t2;
	const class idMaterial* material;
};

class idFont
{
public:
	idFont( const char* n );
	~idFont();

	void Touch();

	const char* GetName() const
	{
		return name;
	}

	float GetLineHeight( float scale ) const;
//	float GetAscender( float scale ) const;
	float GetMaxCharWidth( float scale ) const;

	float GetGlyphWidth( float scale, uint32_t idx ) const;
	void GetScaledGlyph( float scale, uint32_t idx, scaledGlyphInfo_t& glyphInfo ) const;

private:
	static idFont* RemapFont( const char* baseName );

	int	GetGlyphIndex( uint32_t idx ) const;

	bool LoadFont();

	bool LoadFromTrueTypeFont();
	bool WriteFont();

public:
	struct glyphInfo_t
	{
		// RB: FIXME byte is signed on ARM
#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_5__)
		char	width;	// width of glyph in pixels
		char	height;	// height of glyph in pixels
		byte	top;	// distance in pixels from the base line to the top of the glyph
		byte	left;	// distance in pixels from the pen to the left edge of the glyph
		char	xSkip;	// x adjustment after rendering this glyph
		uint16_t	s;		// x offset in image where glyph starts (in pixels)
		uint16_t	t;		// y offset in image where glyph starts (in pixels)
#else
		byte	width;	// width of glyph in pixels
		byte	height;	// height of glyph in pixels
		char	top;	// distance in pixels from the base line to the top of the glyph
		char	left;	// distance in pixels from the pen to the left edge of the glyph
		byte	xSkip;	// x adjustment after rendering this glyph
		uint16_t	s;		// x offset in image where glyph starts (in pixels)
		uint16_t	t;		// y offset in image where glyph starts (in pixels)
#endif
		// RB end
	};
	struct fontInfo_t
	{
		struct oldInfo_t
		{
			float maxWidth;
			float maxHeight;
		} oldInfo[3];

		short		ascender;
		short		descender;

		short		numGlyphs;
		glyphInfo_t* glyphData;

		// This is a sorted array of all characters in the font
		// This maps directly to glyphData, so if charIndex[0] is 42 then glyphData[0] is character 42
		uint32_t* 	charIndex;

		// As an optimization, provide a direct mapping for the ascii character set
		char		ascii[128];

		const idMaterial* 	material;
	};

private:
	// base name of the font (minus "fonts/" and ".dat")
	idStr			name;

	// Fonts can be aliases to other fonts
	idFont* alias;

	// If the font is NOT an alias, this is where the font data is located
	fontInfo_t* fontInfo;
};

#endif // #if defined(USE_IDFONT)

#endif
