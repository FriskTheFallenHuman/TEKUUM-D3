﻿/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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
#pragma hdrstop
#include "precompiled.h"
#include "Font.h"

#if defined(USE_IDFONT)

#ifdef BUILD_FREETYPE
// RB: changed to local includes
//#include <freetype/fterrors.h>
#include <freetype/ftsystem.h>
#include <freetype/ftimage.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#endif
// RB end

#if defined(STANDALONE)
const char* DEFAULT_FONT = "bitstream_vera_sans";
#else
const char* DEFAULT_FONT = "arial_narrow";
#endif

static const float old_scale2 = 0.6f;
static const float old_scale1 = 0.3f;

/*
==============================
Old_SelectValueForScale
==============================
*/
ID_INLINE float Old_SelectValueForScale( float scale, float v0, float v1, float v2 )
{
	return ( scale >= old_scale2 ) ? v2 : ( scale >= old_scale1 ) ? v1 : v0;
}

/*
==============================
idFont::RemapFont
==============================
*/
idFont* idFont::RemapFont( const char* baseName )
{
	idStr cleanName = baseName;
	
	if( cleanName == DEFAULT_FONT )
	{
		return NULL;
	}
	
	//: RB added suggestion to avoid infinite loop
	idStr suggestion = "#font_" + cleanName;
	const char* remapped = common->GetLanguageDict()->GetString( suggestion );
	if( remapped != NULL && remapped[0] != '\0' && suggestion.Cmp( remapped ) != 0 )
	{
		return renderSystem->RegisterFont( remapped );
	}
	
	suggestion = "#font_" + cleanName;
	const char* wildcard = common->GetLanguageDict()->GetString( "#font_*" );
	if( wildcard != NULL && cleanName.Icmp( wildcard ) != 0 && suggestion.Cmp( remapped ) != 0 )
	{
		return renderSystem->RegisterFont( wildcard );
	}
	// RB end
	
	// Note single | so both sides are always executed
	if( cleanName.ReplaceChar( ' ', '_' ) | cleanName.ReplaceChar( '-', '_' ) )
	{
		return renderSystem->RegisterFont( cleanName );
	}
	
	return NULL;
}

/*
==============================
idFont::~idFont
==============================
*/
idFont::~idFont()
{
	delete fontInfo;
}

/*
==============================
idFont::idFont
==============================
*/
idFont::idFont( const char* n ) : name( n )
{
	fontInfo = NULL;
	alias = RemapFont( n );
	
	if( alias != NULL )
	{
		// Make sure we don't have a circular reference
		for( idFont* f = alias; f != NULL; f = f->alias )
		{
			if( f == this )
			{
				idLib::FatalError( "Font alias \"%s\" is a circular reference!", n );
			}
		}
		return;
	}
	
	if( !LoadFont() )
	{
		if( name.Icmp( DEFAULT_FONT ) == 0 )
		{
			idLib::FatalError( "Could not load default font \"%s\"", DEFAULT_FONT );
		}
		else
		{
			idLib::Warning( "Could not load font %s", n );
			alias = renderSystem->RegisterFont( DEFAULT_FONT );
		}
	}
}

struct oldGlyphInfo_t
{
	int					height;			// number of scan lines
	int					top;			// top of glyph in buffer
	int					bottom;			// bottom of glyph in buffer
	int					pitch;			// width for copying
	int					xSkip;			// x adjustment
	int					imageWidth;		// width of actual image
	int					imageHeight;	// height of actual image
	float				s;				// x offset in image where glyph starts
	float				t;				// y offset in image where glyph starts
	float				s2;
	float				t2;
	int					junk;
	char				materialName[32];
};

const int GLYPH_START			= 0;
const int GLYPH_END				= 255;
const int GLYPH_CHARSTART		= 32;
const int GLYPH_CHAREND			= 127;
const int GLYPHS_PER_FONT		= GLYPH_END - GLYPH_START + 1;
//const int GLYPHS_PER_FONT = 256;

/*
==============================
LoadOldGlyphData
==============================
*/
static bool LoadOldGlyphData( const char* filename, oldGlyphInfo_t glyphInfo[GLYPHS_PER_FONT] )
{
#if 0
	return false;
#else
	idFile* fd = fileSystem->OpenFileRead( filename );
	if( fd == NULL )
	{
		return false;
	}
	fd->Read( glyphInfo, GLYPHS_PER_FONT * sizeof( oldGlyphInfo_t ) );
	for( int i = 0; i < GLYPHS_PER_FONT; i++ )
	{
		idSwap::Little( glyphInfo[i].height );
		idSwap::Little( glyphInfo[i].top );
		idSwap::Little( glyphInfo[i].bottom );
		idSwap::Little( glyphInfo[i].pitch );
		idSwap::Little( glyphInfo[i].xSkip );
		idSwap::Little( glyphInfo[i].imageWidth );
		idSwap::Little( glyphInfo[i].imageHeight );
		idSwap::Little( glyphInfo[i].s );
		idSwap::Little( glyphInfo[i].t );
		idSwap::Little( glyphInfo[i].s2 );
		idSwap::Little( glyphInfo[i].t2 );
		assert( glyphInfo[i].imageWidth == glyphInfo[i].pitch );
		assert( glyphInfo[i].imageHeight == glyphInfo[i].height );
		assert( glyphInfo[i].imageWidth == ( glyphInfo[i].s2 - glyphInfo[i].s ) * 256 );
		assert( glyphInfo[i].imageHeight == ( glyphInfo[i].t2 - glyphInfo[i].t ) * 256 );
		assert( glyphInfo[i].junk == 0 );
	}
	delete fd;
	return true;
#endif
}

static bool SaveOldGlyphData( const char* filename, oldGlyphInfo_t glyphInfo[GLYPHS_PER_FONT] )
{
	idFile* fd = fileSystem->OpenFileWrite( filename );
	if( fd == NULL )
	{
		return false;
	}
	
	for( int i = 0; i < GLYPHS_PER_FONT; i++ )
	{
		fd->WriteBig( glyphInfo[i].height );
		fd->WriteBig( glyphInfo[i].top );
		fd->WriteBig( glyphInfo[i].bottom );
		fd->WriteBig( glyphInfo[i].pitch );
		fd->WriteBig( glyphInfo[i].xSkip );
		fd->WriteBig( glyphInfo[i].imageWidth );
		fd->WriteBig( glyphInfo[i].imageHeight );
		fd->WriteBig( glyphInfo[i].s );
		fd->WriteBig( glyphInfo[i].t );
		fd->WriteBig( glyphInfo[i].s2 );
		fd->WriteBig( glyphInfo[i].t2 );
		
		fd->WriteBig( glyphInfo[i].junk );
		
		for( int j = 0; j < 32; j++ )
		{
			fd->WriteChar( glyphInfo[i].materialName[j] );
		}
	}
	delete fd;
	return true;
}

/*
==============================
idFont::LoadFont
==============================
*/
bool idFont::LoadFont()
{
	idStr fontName = va( "newfonts/%s/48.dat", GetName() );
	idFile* fd = fileSystem->OpenFileRead( fontName );
	if( fd == NULL )
	{
		if( !LoadFromTrueTypeFont() )
		{
			return false;
		}
		else
		{
			WriteFont();
		}
	}
	
	const int FONT_INFO_VERSION = 42;
	const int FONT_INFO_MAGIC = ( FONT_INFO_VERSION | ( 'i' << 24 ) | ( 'd' << 16 ) | ( 'f' << 8 ) );
	
	uint32 version = 0;
	fd->ReadBig( version );
	if( version != FONT_INFO_MAGIC )
	{
		idLib::Warning( "Wrong version in %s", GetName() );
		delete fd;
		return false;
	}
	
	fontInfo = new fontInfo_t;
	
	short pointSize = 0;
	
	fd->ReadBig( pointSize );
	assert( pointSize == 48 );
	
	fd->ReadBig( fontInfo->ascender );
	fd->ReadBig( fontInfo->descender );
	
	fd->ReadBig( fontInfo->numGlyphs );
	
	fontInfo->glyphData = ( glyphInfo_t* )Mem_Alloc( sizeof( glyphInfo_t ) * fontInfo->numGlyphs );
	fontInfo->charIndex = ( uint32* )Mem_Alloc( sizeof( uint32 ) * fontInfo->numGlyphs );
	
	fd->Read( fontInfo->glyphData, fontInfo->numGlyphs * sizeof( glyphInfo_t ) );
	
	for( int i = 0; i < fontInfo->numGlyphs; i++ )
	{
		idSwap::Little( fontInfo->glyphData[i].width );
		idSwap::Little( fontInfo->glyphData[i].height );
		idSwap::Little( fontInfo->glyphData[i].top );
		idSwap::Little( fontInfo->glyphData[i].left );
		idSwap::Little( fontInfo->glyphData[i].xSkip );
		idSwap::Little( fontInfo->glyphData[i].s );
		idSwap::Little( fontInfo->glyphData[i].t );
	}
	
	fd->Read( fontInfo->charIndex, fontInfo->numGlyphs * sizeof( uint32 ) );
	idSwap::LittleArray( fontInfo->charIndex, fontInfo->numGlyphs );
	
	memset( fontInfo->ascii, -1, sizeof( fontInfo->ascii ) );
	for( int i = 0; i < fontInfo->numGlyphs; i++ )
	{
		if( fontInfo->charIndex[i] < 128 )
		{
			fontInfo->ascii[fontInfo->charIndex[i]] = i;
		}
		else
		{
			// Since the characters are sorted, as soon as we find a non-ascii character, we can stop
			break;
		}
	}
	
	idStr fontTextureName = fontName;
	fontTextureName.SetFileExtension( "tga" );
	
	fontInfo->material = declManager->FindMaterial( fontTextureName );
	fontInfo->material->SetSort( SS_GUI );
	
	// Load the old glyph data because we want our new fonts to fit in the old glyph metrics
	int pointSizes[3] = { 12, 24, 48 };
	float scales[3] = { 4.0f, 2.0f, 1.0f };
	for( int i = 0; i < 3; i++ )
	{
		oldGlyphInfo_t oldGlyphInfo[GLYPHS_PER_FONT];
		const char* oldFileName = va( "newfonts/%s/old_%d.dat", GetName(), pointSizes[i] );
		if( LoadOldGlyphData( oldFileName, oldGlyphInfo ) )
		{
			int mh = 0;
			int mw = 0;
			for( int g = 0; g < GLYPHS_PER_FONT; g++ )
			{
				if( mh < oldGlyphInfo[g].height )
				{
					mh = oldGlyphInfo[g].height;
				}
				if( mw < oldGlyphInfo[g].xSkip )
				{
					mw = oldGlyphInfo[g].xSkip;
				}
			}
			fontInfo->oldInfo[i].maxWidth = scales[i] * mw;
			fontInfo->oldInfo[i].maxHeight = scales[i] * mh;
		}
		else
		{
			int mh = 0;
			int mw = 0;
			for( int g = 0; g < fontInfo->numGlyphs; g++ )
			{
				if( mh < fontInfo->glyphData[g].height )
				{
					mh = fontInfo->glyphData[g].height;
				}
				if( mw < fontInfo->glyphData[g].xSkip )
				{
					mw = fontInfo->glyphData[g].xSkip;
				}
			}
			fontInfo->oldInfo[i].maxWidth = mw;
			fontInfo->oldInfo[i].maxHeight = mh;
		}
	}
	delete fd;
	return true;
}

/*
==============================
idFont::GetGlyphIndex
==============================
*/
int	idFont::GetGlyphIndex( uint32 idx ) const
{
	if( idx < 128 )
	{
		return fontInfo->ascii[idx];
	}
	if( fontInfo->numGlyphs == 0 )
	{
		return -1;
	}
	if( fontInfo->charIndex == NULL )
	{
		return idx;
	}
	int len = fontInfo->numGlyphs;
	int mid = fontInfo->numGlyphs;
	int offset = 0;
	while( mid > 0 )
	{
		mid = len >> 1;
		if( fontInfo->charIndex[offset + mid] <= idx )
		{
			offset += mid;
		}
		len -= mid;
	}
	return ( fontInfo->charIndex[offset] == idx ) ? offset : -1;
}

/*
==============================
idFont::GetLineHeight
==============================
*/
float idFont::GetLineHeight( float scale ) const
{
	if( alias != NULL )
	{
		return alias->GetLineHeight( scale );
	}
	if( fontInfo != NULL )
	{
		return scale * Old_SelectValueForScale( scale, fontInfo->oldInfo[0].maxHeight, fontInfo->oldInfo[1].maxHeight, fontInfo->oldInfo[2].maxHeight );
	}
	return 0.0f;
}

/*
==============================
idFont::GetAscender
==============================
*/
/*
float idFont::GetAscender( float scale ) const
{
	if( alias != NULL )
	{
		return alias->GetAscender( scale );
	}
	if( fontInfo != NULL )
	{
		return scale * fontInfo->ascender;
	}
	return 0.0f;
}
*/

/*
==============================
idFont::GetMaxCharWidth
==============================
*/
float idFont::GetMaxCharWidth( float scale ) const
{
	if( alias != NULL )
	{
		return alias->GetMaxCharWidth( scale );
	}
	if( fontInfo != NULL )
	{
		return scale * Old_SelectValueForScale( scale, fontInfo->oldInfo[0].maxWidth, fontInfo->oldInfo[1].maxWidth, fontInfo->oldInfo[2].maxWidth );
	}
	return 0.0f;
}

/*
==============================
idFont::GetGlyphWidth
==============================
*/
float idFont::GetGlyphWidth( float scale, uint32 idx ) const
{
	if( alias != NULL )
	{
		return alias->GetGlyphWidth( scale, idx );
	}
	if( fontInfo != NULL )
	{
		int i = GetGlyphIndex( idx );
		const int asterisk = 42;
		if( i == -1 && idx != asterisk )
		{
			i = GetGlyphIndex( asterisk );
		}
		if( i >= 0 )
		{
			return scale * fontInfo->glyphData[i].xSkip;
		}
	}
	return 0.0f;
}

/*
==============================
idFont::GetScaledGlyph
==============================
*/
void idFont::GetScaledGlyph( float scale, uint32 idx, scaledGlyphInfo_t& glyphInfo ) const
{
	if( alias != NULL )
	{
		return alias->GetScaledGlyph( scale, idx, glyphInfo );
	}
	if( fontInfo != NULL )
	{
		int i = GetGlyphIndex( idx );
		const int asterisk = 42;
		if( i == -1 && idx != asterisk )
		{
			i = GetGlyphIndex( asterisk );
		}
		if( i >= 0 )
		{
			int materialWidth = fontInfo->material->GetImageWidth();
			int materialHeight = fontInfo->material->GetImageHeight();
			float invMaterialWidth = 1.0f / materialWidth;
			float invMaterialHeight = 1.0f / materialHeight;
			glyphInfo_t& gi = fontInfo->glyphData[i];
			glyphInfo.xSkip = scale * gi.xSkip;
			glyphInfo.top = scale * gi.top;
			glyphInfo.left = scale * gi.left;
			glyphInfo.width = scale * gi.width;
			glyphInfo.height = scale * gi.height;
			glyphInfo.s1 = ( gi.s - 0.5f ) * invMaterialWidth;
			glyphInfo.t1 = ( gi.t - 0.5f ) * invMaterialHeight;
			glyphInfo.s2 = ( gi.s + gi.width + 0.5f ) * invMaterialWidth;
			glyphInfo.t2 = ( gi.t + gi.height + 0.5f ) * invMaterialHeight;
			glyphInfo.material = fontInfo->material;
			return;
		}
	}
	memset( &glyphInfo, 0, sizeof( glyphInfo ) );
}

/*
==============================
idFont::Touch
==============================
*/
void idFont::Touch()
{
	if( alias != NULL )
	{
		alias->Touch();
	}
	if( fontInfo != NULL )
	{
		const_cast<idMaterial*>( fontInfo->material )->EnsureNotPurged();
		fontInfo->material->SetSort( SS_GUI );
	}
}









#ifdef BUILD_FREETYPE

static FT_Library ftLibrary = NULL;

const int FONT_SIZE				= 256;




/*
============
R_GetGlyphInfo
============
*/
void R_GetGlyphInfo( FT_GlyphSlot glyph, int* left, int* right, int* width, int* top, int* bottom, int* height, int* pitch )
{
#define _FLOOR(x)  ((x) & -64)
#define _CEIL(x)   (((x)+63) & -64)
#define _TRUNC(x)  ((x) >> 6)


	*left  = _FLOOR( glyph->metrics.horiBearingX );
	*right = _CEIL( glyph->metrics.horiBearingX + glyph->metrics.width );
	*width = _TRUNC( *right - *left );
	
	*top    = _CEIL( glyph->metrics.horiBearingY );
	*bottom = _FLOOR( glyph->metrics.horiBearingY - glyph->metrics.height );
	*height = _TRUNC( *top - *bottom );
	
	//*pitch  = ( qtrue ? (*width+3) & -4 : (*width+7) >> 3 );
	*pitch  = ( *width + 3 ) & -4;
	
#undef _FLOOR
#undef _CEIL
#undef _TRUNC
}

/*
============
R_RenderGlyph
============
*/
FT_Bitmap* R_RenderGlyph( FT_GlyphSlot glyph, idFont::glyphInfo_t* glyphOut )
{
	FT_Bitmap*  bit2;
	int left, right, width, top, bottom, height, pitch, size;
	
	R_GetGlyphInfo( glyph, &left, &right, &width, &top, &bottom, &height, &pitch );
	
	if( glyph->format == ft_glyph_format_outline )
	{
		size   = pitch * height;
		
		// RB: added missing cast
		bit2 = ( FT_Bitmap* ) Mem_Alloc( sizeof( FT_Bitmap ) );
		// RB end
		
		bit2->width      = width;
		bit2->rows       = height;
		bit2->pitch      = pitch;
		bit2->pixel_mode = ft_pixel_mode_grays;
		//bit2->pixel_mode = ft_pixel_mode_mono;
		
		// RB: added missing cast
		bit2->buffer     = ( byte* ) Mem_Alloc( pitch * height );
		// RB end
		
		bit2->num_grays = 256;
		
		memset( bit2->buffer, 0, size );
		
		FT_Outline_Translate( &glyph->outline, -left, -bottom );
		
		FT_Outline_Get_Bitmap( ftLibrary, &glyph->outline, bit2 );
		
		glyphOut->height = height;
		glyphOut->width = pitch;
		glyphOut->top = ( glyph->metrics.horiBearingY >> 6 ) + 1;
//		glyphOut->bottom = bottom;

		return bit2;
	}
	else
	{
		common->Printf( "Non-outline fonts are not supported\n" );
	}
	return NULL;
}

/*
============
RE_ConstructGlyphInfo
============
*/
static idFont::glyphInfo_t* RE_ConstructGlyphInfo( unsigned char* imageOut, int* xOut, int* yOut, int* maxHeight, FT_Face face, const unsigned char c, bool calcHeight )
{
	int i;
	static idFont::glyphInfo_t glyph;
	unsigned char* src, *dst;
	float scaled_width, scaled_height;
	FT_Bitmap* bitmap = NULL;
	
	memset( &glyph, 0, sizeof( idFont::glyphInfo_t ) );
	
	// make sure everything is here
	if( face != NULL )
	{
		FT_Load_Glyph( face, FT_Get_Char_Index( face, c ), FT_LOAD_DEFAULT );
		bitmap = R_RenderGlyph( face->glyph, &glyph );
		if( bitmap )
		{
			glyph.xSkip = ( face->glyph->metrics.horiAdvance >> 6 ) + 1;
		}
		else
		{
			return &glyph;
		}
		
		if( glyph.height > *maxHeight )
		{
			*maxHeight = glyph.height;
		}
		
		if( calcHeight )
		{
			Mem_Free( bitmap->buffer );
			Mem_Free( bitmap );
			return &glyph;
		}
		
		/*
				// need to convert to power of 2 sizes so we do not get
				// any scaling from the gl upload
				for (scaled_width = 1 ; scaled_width < glyph.pitch ; scaled_width<<=1)
					;
				for (scaled_height = 1 ; scaled_height < glyph.height ; scaled_height<<=1)
					;
		*/
		
		scaled_width = glyph.width;
		scaled_height = glyph.height;
		
		// we need to make sure we fit
		// RB: changed constants to FONT_SIZE -1
		if( *xOut + scaled_width + 1 >= ( FONT_SIZE - 1 ) )
		{
			// RB: fixed wrong yOut
			if( *yOut + ( *maxHeight + 1 ) * 2 >= ( FONT_SIZE - 1 ) )
			{
				*yOut = -1;
				*xOut = -1;
				Mem_Free( bitmap->buffer );
				Mem_Free( bitmap );
				return &glyph;
			}
			else
			{
				*xOut = 0;
				*yOut += *maxHeight + 1;
			}
		}
		else if( *yOut + *maxHeight + 1 >= ( FONT_SIZE - 1 ) )
		{
			*yOut = -1;
			*xOut = -1;
			Mem_Free( bitmap->buffer );
			Mem_Free( bitmap );
			return &glyph;
		}
		// RB end
		
		src = bitmap->buffer;
		// RB: changed constant to FONT_SIZE
		dst = imageOut + ( *yOut * FONT_SIZE ) + *xOut;
		// RB end
		
		if( bitmap->pixel_mode == ft_pixel_mode_mono )
		{
			for( i = 0; i < glyph.height; i++ )
			{
				int j;
				unsigned char* _src = src;
				unsigned char* _dst = dst;
				unsigned char mask = 0x80;
				unsigned char val = *_src;
				for( j = 0; j < glyph.width; j++ )
				{
					if( mask == 0x80 )
					{
						val = *_src++;
					}
					if( val & mask )
					{
						*_dst = 0xff;
					}
					mask >>= 1;
					
					if( mask == 0 )
					{
						mask = 0x80;
					}
					_dst++;
				}
				
				src += glyph.width;
				// RB: changed constant to FONT_SIZE
				dst += FONT_SIZE;
				// RB end
				
			}
		}
		else
		{
			for( i = 0; i < glyph.height; i++ )
			{
				memcpy( dst, src, glyph.width );
				src += glyph.width;
				// RB: changed constant to FONT_SIZE
				dst += FONT_SIZE;
				// RB end
			}
		}
		
		// we now have an 8 bit per pixel grey scale bitmap
		// that is width wide and pf->ftSize->metrics.y_ppem tall
		
//		glyph.imageHeight = scaled_height;
//		glyph.imageWidth = scaled_width;

		// RB: changed constants to FONT_SIZE
		glyph.s = ( float ) * xOut / FONT_SIZE;
		glyph.t = ( float ) * yOut / FONT_SIZE;
//		glyph.s2 = glyph.s + ( float )scaled_width / FONT_SIZE;
//		glyph.t2 = glyph.t + ( float )scaled_height / FONT_SIZE;
		// RB end
		
		*xOut += scaled_width + 1;
	}
	
	Mem_Free( bitmap->buffer );
	Mem_Free( bitmap );
	
	return &glyph;
}

#if 0
static void FinalizeFontInfoEx( const char* fontName, fontInfoEx_t& font, idFont::fontInfo_t* outFont, int fontCount )
{
	idStr name;
	int mw = 0;
	int mh = 0;
	
	for( int i = GLYPH_START; i < GLYPH_END; i++ )
	{
		// RB: FIXME find better way
		if( idStr::Cmpn( outFont->glyphs[i].shaderName, "fonts/", 6 ) == 0 )
		{
			sprintf( name, "%s/%s", fontName, outFont->glyphs[i].shaderName + 6 );
		}
		else
		{
			sprintf( name, "%s/%s", fontName, outFont->glyphs[i].shaderName );
		}
		
		outFont->glyphs[i].glyph = declManager->FindMaterial( name );
		outFont->glyphs[i].glyph->SetSort( SS_GUI );
		
		if( mh < outFont->glyphs[i].height )
		{
			mh = outFont->glyphs[i].height;
		}
		
		if( mw < outFont->glyphs[i].xSkip )
		{
			mw = outFont->glyphs[i].xSkip;
		}
	}
	
	if( fontCount == 0 )
	{
		font.maxWidthSmall = mw;
		font.maxHeightSmall = mh;
	}
	else if( fontCount == 1 )
	{
		font.maxWidthMedium = mw;
		font.maxHeightMedium = mh;
	}
	else
	{
		font.maxWidthLarge = mw;
		font.maxHeightLarge = mh;
	}
}
#endif

#endif // #ifdef BUILD_FREETYPE

bool idFont::LoadFromTrueTypeFont()
{
#if 1
	return false;
#else
	idStr fontName = va( "newfonts/%s", GetName() );
	fontName.SetFileExtension( ".ttf" );
	
	void* faceData;
	ID_TIME_T ftime;
	
	int len = fileSystem->ReadFile( name, &faceData, &ftime );
	if( len <= 0 )
	{
		common->Printf( "LoadFromTrueTypeFont: Unable to read font file: '%s'", GetName() );
		return false;
	}
	
	
	
	if( FT_Init_FreeType( &ftLibrary ) )
	{
		common->Printf( "LoadFromTrueTypeFont: Unable to initialize FreeType.\n" );
		return false;
	}
	
	// make a 256x256 image buffer, once it is full, register it, clean it and keep going
	// until all glyphs are rendered
	
	byte* out = ( byte* ) Mem_Alloc( 1024 * 1024 );
	if( out == NULL )
	{
		common->Printf( "LoadFromTrueTypeFont: Mem_Alloc failure during output image creation." );
		return false;
	}
	
	
	fontInfo = new fontInfo_t;
	
	FT_Face face;
	if( FT_New_Memory_Face( ftLibrary, ( const FT_Byte* ) faceData, len, 0, &face ) )
	{
		common->Printf( "LoadFromTrueTypeFont: FreeType2, unable to allocate new face." );
		return false;
	}
	
	float dpi = 72;
	short pointSize = 48;
	
	//fd->ReadBig( pointSize );
	//assert( pointSize == 48 );
	
	if( FT_Set_Char_Size( face, pointSize << 6, pointSize << 6, dpi, dpi ) )
	{
		common->Printf( "LoadFromTrueTypeFont: FreeType2, Unable to set face char size." );
		return false;
	}
	
	// RB: FIXME however those are not used by us
	fontInfo->ascender = 0;
	fontInfo->descender = 0;
	
	fontInfo->numGlyphs = GLYPHS_PER_FONT;
	
	
	fontInfo->glyphData = ( glyphInfo_t* )Mem_Alloc( sizeof( glyphInfo_t ) * fontInfo->numGlyphs );
	fontInfo->charIndex = ( uint32* )Mem_Alloc( sizeof( uint32 ) * fontInfo->numGlyphs );
	
	fd->Read( fontInfo->glyphData, fontInfo->numGlyphs * sizeof( glyphInfo_t ) );
	
	for( int i = 0; i < fontInfo->numGlyphs; i++ )
	{
		idSwap::Little( fontInfo->glyphData[i].width );
		idSwap::Little( fontInfo->glyphData[i].height );
		idSwap::Little( fontInfo->glyphData[i].top );
		idSwap::Little( fontInfo->glyphData[i].left );
		idSwap::Little( fontInfo->glyphData[i].xSkip );
		idSwap::Little( fontInfo->glyphData[i].s );
		idSwap::Little( fontInfo->glyphData[i].t );
	}
	
	fd->Read( fontInfo->charIndex, fontInfo->numGlyphs * sizeof( uint32 ) );
	idSwap::LittleArray( fontInfo->charIndex, fontInfo->numGlyphs );
	
	memset( fontInfo->ascii, -1, sizeof( fontInfo->ascii ) );
	for( int i = 0; i < fontInfo->numGlyphs; i++ )
	{
		if( fontInfo->charIndex[i] < 128 )
		{
			fontInfo->ascii[fontInfo->charIndex[i]] = i;
		}
		else
		{
			// Since the characters are sorted, as soon as we find a non-ascii character, we can stop
			break;
		}
	}
	
	idStr fontTextureName = fontName;
	fontTextureName.SetFileExtension( "tga" );
	
	fontInfo->material = declManager->FindMaterial( fontTextureName );
	fontInfo->material->SetSort( SS_GUI );
	
	// Load the old glyph data because we want our new fonts to fit in the old glyph metrics
	int pointSizes[3] = { 12, 24, 48 };
	float scales[3] = { 4.0f, 2.0f, 1.0f };
	for( int i = 0; i < 3; i++ )
	{
		oldGlyphInfo_t oldGlyphInfo[GLYPHS_PER_FONT];
		const char* oldFileName = va( "newfonts/%s/old_%d.dat", GetName(), pointSizes[i] );
		if( LoadOldGlyphData( oldFileName, oldGlyphInfo ) )
		{
			int mh = 0;
			int mw = 0;
			for( int g = 0; g < GLYPHS_PER_FONT; g++ )
			{
				if( mh < oldGlyphInfo[g].height )
				{
					mh = oldGlyphInfo[g].height;
				}
				if( mw < oldGlyphInfo[g].xSkip )
				{
					mw = oldGlyphInfo[g].xSkip;
				}
			}
			fontInfo->oldInfo[i].maxWidth = scales[i] * mw;
			fontInfo->oldInfo[i].maxHeight = scales[i] * mh;
		}
		else
		{
			int mh = 0;
			int mw = 0;
			for( int g = 0; g < fontInfo->numGlyphs; g++ )
			{
				if( mh < fontInfo->glyphData[g].height )
				{
					mh = fontInfo->glyphData[g].height;
				}
				if( mw < fontInfo->glyphData[g].xSkip )
				{
					mw = fontInfo->glyphData[g].xSkip;
				}
			}
			fontInfo->oldInfo[i].maxWidth = mw;
			fontInfo->oldInfo[i].maxHeight = mh;
		}
	}
#endif
	
	return true;
}

bool idFont::WriteFont()
{
	// TODO
	return false;
}

#endif