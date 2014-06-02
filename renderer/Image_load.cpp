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

#pragma hdrstop
#include "precompiled.h"

#include "tr_local.h"
#include "../renderer/DXT//DXTCodec.h"

// RB begin
idCVar image_exportPNG( "image_exportPNG", "0", CVAR_RENDERER | CVAR_BOOL, "export certain images as PNG files (development tool)" );
// RB end

/*
================
BitsForFormat
================
*/
int BitsForFormat( textureFormat_t format )
{
	switch( format )
	{
		case FMT_NONE:
			return 0;
		case FMT_RGBA8:
			return 32;
		case FMT_XRGB8:
			return 32;
		case FMT_RGB565:
			return 16;
		case FMT_L8A8:
			return 16;
		case FMT_ALPHA:
			return 8;
		case FMT_LUM8:
			return 8;
		case FMT_INT8:
			return 8;
		case FMT_DXT1:
			return 4;
		case FMT_DXT5:
			return 8;
		// RB: added ETC compression
		case FMT_ETC1_RGB8_OES:
			return 4;
		case FMT_SHADOW_ARRAY:
			return ( 32 * 6 );
		// RB end
		case FMT_DEPTH:
			return 32;
		case FMT_X16:
			return 16;
		case FMT_Y16_X16:
			return 32;
		default:
			assert( 0 );
			return 0;
	}
}

/*
========================
idImage::DeriveOpts
========================
*/
ID_INLINE void idImage::DeriveOpts()
{

	if( opts.format == FMT_NONE )
	{
		opts.colorFormat = CFM_DEFAULT;
		
		switch( usage )
		{
			case TD_COVERAGE:
#if defined(__ANDROID__)
				//if( opts.width > 64 && opts.height > 64 )
			{
				opts.format = FMT_ETC1_RGB8_OES;
				opts.colorFormat = CFM_GREEN_ALPHA;
			}
				//else
				//{
				//	opts.format = FMT_RGBA8;
				//	opts.colorFormat = CFM_GREEN_ALPHA;
				//}
			
#elif ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
			opts.format = FMT_RGBA8;
			opts.colorFormat = CFM_GREEN_ALPHA;
#else
			opts.format = FMT_DXT1;
			opts.colorFormat = CFM_GREEN_ALPHA;
#endif
			break;
			
			case TD_DEPTH:
				opts.format = FMT_DEPTH;
				break;
				
			case TD_SHADOW_ARRAY:
				opts.format = FMT_SHADOW_ARRAY;
				break;
				
			case TD_RGBA16F:
				opts.format = FMT_RGBA16F;
				break;
				
			case TD_DIFFUSE:
#if ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
				// TD_DIFFUSE gets only set to when its a diffuse texture for an interaction
				//opts.gammaMips = true;
				opts.format = FMT_RGBA8;
				opts.colorFormat = CFM_YCOCG_RGBA8;
#else
				opts.gammaMips = true;
				opts.format = FMT_DXT5;
				opts.colorFormat = CFM_YCOCG_DXT5;
#endif
				break;
				
			case TD_SPECULAR:
#if defined(__ANDROID__)
				//if( opts.width > 64 && opts.height > 64 )
			{
				opts.gammaMips = true;
				opts.format = FMT_ETC1_RGB8_OES;
			}
				//else
				//{
				//	opts.format = FMT_RGBA8;
				//	opts.gammaMips = true;
				//}
			
#elif ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
			opts.gammaMips = true;
			opts.format = FMT_RGBA8;
#else
			opts.gammaMips = true;
			opts.format = FMT_DXT1;
#endif
			break;
			
			case TD_DEFAULT:
#if ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
				opts.gammaMips = true;
				opts.format = FMT_RGBA8;
#else
				opts.gammaMips = true;
				opts.format = FMT_DXT5;
#endif
				break;
				
			case TD_BUMP:
#if defined(__ANDROID__)
				//if( opts.width > 64 && opts.height > 64 )
			{
				opts.format = FMT_ETC1_RGB8_OES;
			}
				//else
				//{
				//	opts.format = FMT_RGBA8;
				//}
			
#elif ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
			opts.format = FMT_RGBA8;
#else
			opts.format = FMT_DXT5;
			opts.colorFormat = CFM_NORMAL_DXT5;
#endif
			break;
			
			case TD_FONT:
#if ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
				opts.format = FMT_RGBA8;
#else
				opts.format = FMT_DXT1;
#endif
				opts.colorFormat = CFM_GREEN_ALPHA;
				opts.numLevels = 4; // We only support 4 levels because we align to 16 in the exporter
				opts.gammaMips = true;
				break;
				
			case TD_LIGHT:
#if ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
				opts.format = FMT_RGBA8;
				opts.gammaMips = true;
#else
				opts.format = FMT_RGBA8;
				opts.gammaMips = true;
#endif
				break;
				
			case TD_LOOKUP_TABLE_MONO:
				opts.format = FMT_INT8;
				break;
				
			case TD_LOOKUP_TABLE_ALPHA:
#if ( defined(USE_GLES2) || defined(USE_GLES3) ) && !defined(USE_MESA)
				opts.format = FMT_RGBA8;
#else
				opts.format = FMT_ALPHA;
#endif
				break;
				
			case TD_LOOKUP_TABLE_RGB1:
			case TD_LOOKUP_TABLE_RGBA:
				opts.format = FMT_RGBA8;
				break;
				
			default:
				assert( false );
				opts.format = FMT_RGBA8;
		}
	}
	
	if( opts.numLevels == 0 )
	{
		opts.numLevels = 1;
		
		if( filter == TF_LINEAR || filter == TF_NEAREST )
		{
			// don't create mip maps if we aren't going to be using them
		}
		else
		{
			int	temp_width = opts.width;
			int	temp_height = opts.height;
			while( temp_width > 1 || temp_height > 1 )
			{
				temp_width >>= 1;
				temp_height >>= 1;
				if( ( opts.format == FMT_DXT1 || opts.format == FMT_DXT5 || opts.format == FMT_ETC1_RGB8_OES ) &&
						( ( temp_width & 0x3 ) != 0 || ( temp_height & 0x3 ) != 0 ) )
				{
					break;
				}
				opts.numLevels++;
			}
		}
	}
}

/*
========================
idImage::AllocImage
========================
*/
void idImage::AllocImage( const idImageOpts& imgOpts, textureFilter_t tf, textureRepeat_t tr )
{
	filter = tf;
	repeat = tr;
	opts = imgOpts;
	DeriveOpts();
	AllocImage();
}

/*
================
GenerateImage
================
*/
void idImage::GenerateImage( const byte* pic, int width, int height, textureFilter_t filterParm, textureRepeat_t repeatParm, textureUsage_t usageParm, int msaaSamples )
{
	PurgeImage();
	
	filter = filterParm;
	repeat = repeatParm;
	usage = usageParm;
	cubeFiles = CF_2D;
	
	opts.textureType = ( msaaSamples > 0 ) ? TT_2D_MULTISAMPLE : TT_2D;
	opts.width = width;
	opts.height = height;
	opts.numLevels = 0;
	opts.msaaSamples = msaaSamples;
	DeriveOpts();
	
	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before the render starts would miss
	// the generated texture
	if( !R_IsInitialized() )
	{
		return;
	}
	
	// RB: allow pic == NULL for internal framebuffer images
	if( pic == NULL || opts.textureType == TT_2D_MULTISAMPLE )
	{
		AllocImage();
	}
	else
	{
		idBinaryImage im( GetName() );
		im.Load2DFromMemory( width, height, pic, opts.numLevels, opts.format, opts.colorFormat, opts.gammaMips );
		
		AllocImage();
		
		for( int i = 0; i < im.NumImages(); i++ )
		{
			const bimageImage_t& img = im.GetImageHeader( i );
			const byte* data = im.GetImageData( i );
			SubImageUpload( img.level, 0, 0, img.destZ, img.width, img.height, data );
		}
	}
	// RB end
}

/*
====================
GenerateCubeImage

Non-square cube sides are not allowed
====================
*/
void idImage::GenerateCubeImage( const byte* pic[6], int size, textureFilter_t filterParm, textureUsage_t usageParm )
{
	PurgeImage();
	
	filter = filterParm;
	repeat = TR_CLAMP;
	usage = usageParm;
	cubeFiles = CF_NATIVE;
	
	opts.textureType = TT_CUBIC;
	opts.width = size;
	opts.height = size;
	opts.numLevels = 0;
	DeriveOpts();
	
	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before the render starts would miss
	// the generated texture
	if( !R_IsInitialized() )
	{
		return;
	}
	
	idBinaryImage im( GetName() );
	im.LoadCubeFromMemory( size, pic, opts.numLevels, opts.format, opts.gammaMips );
	
	AllocImage();
	
	for( int i = 0; i < im.NumImages(); i++ )
	{
		const bimageImage_t& img = im.GetImageHeader( i );
		const byte* data = im.GetImageData( i );
		SubImageUpload( img.level, 0, 0, img.destZ, img.width, img.height, data );
	}
}

// RB begin
void idImage::GenerateShadowArray( int width, int height, textureFilter_t filterParm, textureRepeat_t repeatParm, textureUsage_t usageParm )
{
	PurgeImage();
	
	filter = filterParm;
	repeat = repeatParm;
	usage = usageParm;
	cubeFiles = CF_2D_ARRAY;
	
	opts.textureType = TT_2D_ARRAY;
	opts.width = width;
	opts.height = height;
	opts.numLevels = 0;
	DeriveOpts();
	
	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before the render starts would miss
	// the generated texture
	if( !R_IsInitialized() )
	{
		return;
	}
	
	//idBinaryImage im( GetName() );
	//im.Load2DFromMemory( width, height, pic, opts.numLevels, opts.format, opts.colorFormat, opts.gammaMips );
	
	AllocImage();
	
	/*
	for( int i = 0; i < im.NumImages(); i++ )
	{
		const bimageImage_t& img = im.GetImageHeader( i );
		const byte* data = im.GetImageData( i );
		SubImageUpload( img.level, 0, 0, img.destZ, img.width, img.height, data );
	}
	*/
}
// RB end

/*
===============
GetGeneratedName

name contains GetName() upon entry
===============
*/
void idImage::GetGeneratedName( idStr& _name, const textureUsage_t& _usage, const cubeFiles_t& _cube )
{
	idStrStatic< 64 > extension;
	
	_name.ExtractFileExtension( extension );
	_name.StripFileExtension();
	
	_name += va( "#__%02d%02d", ( int )_usage, ( int )_cube );
	if( extension.Length() > 0 )
	{
		_name.SetFileExtension( extension );
	}
}


/*
===============
ActuallyLoadImage

Absolutely every image goes through this path
On exit, the idImage will have a valid OpenGL texture number that can be bound
===============
*/
void idImage::ActuallyLoadImage( bool fromBackEnd )
{

	// if we don't have a rendering context yet, just return
	if( !R_IsInitialized() )
	{
		return;
	}
	
	// this is the ONLY place generatorFunction will ever be called
	if( generatorFunction )
	{
		generatorFunction( this );
		return;
	}
	
	if( com_productionMode.GetInteger() != 0 )
	{
		sourceFileTime = FILE_NOT_FOUND_TIMESTAMP;
		if( cubeFiles != CF_2D )
		{
			opts.textureType = TT_CUBIC;
			repeat = TR_CLAMP;
		}
	}
	else
	{
		// RB begin
		if( cubeFiles == CF_2D_ARRAY )
		{
			opts.textureType = TT_2D_ARRAY;
		}
		// RB end
		else if( cubeFiles != CF_2D )
		{
			opts.textureType = TT_CUBIC;
			repeat = TR_CLAMP;
			R_LoadCubeImages( GetName(), cubeFiles, NULL, NULL, &sourceFileTime );
		}
		else
		{
			opts.textureType = TT_2D;
			R_LoadImageProgram( GetName(), NULL, NULL, NULL, &sourceFileTime, &usage );
		}
	}
	
	// Figure out opts.colorFormat and opts.format so we can make sure the binary image is up to date
	DeriveOpts();
	
	idStrStatic< MAX_OSPATH > generatedName = GetName();
	GetGeneratedName( generatedName, usage, cubeFiles );
	
	idBinaryImage im( generatedName );
	binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
	
	// BFHACK, do not want to tweak on buildgame so catch these images here
#if 0
	if( binaryFileTime == FILE_NOT_FOUND_TIMESTAMP && fileSystem->UsingResourceFiles() )
	{
		int c = 1;
		while( c-- > 0 )
		{
			if( generatedName.Find( "guis/assets/white#__0000", false ) >= 0 )
			{
				generatedName.Replace( "white#__0000", "white#__0200" );
				im.SetName( generatedName );
				binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
				break;
			}
			if( generatedName.Find( "guis/assets/white#__0100", false ) >= 0 )
			{
				generatedName.Replace( "white#__0100", "white#__0200" );
				im.SetName( generatedName );
				binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
				break;
			}
			if( generatedName.Find( "textures/black#__0100", false ) >= 0 )
			{
				generatedName.Replace( "black#__0100", "black#__0200" );
				im.SetName( generatedName );
				binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
				break;
			}
			if( generatedName.Find( "textures/decals/bulletglass1_d#__0100", false ) >= 0 )
			{
				generatedName.Replace( "bulletglass1_d#__0100", "bulletglass1_d#__0200" );
				im.SetName( generatedName );
				binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
				break;
			}
			if( generatedName.Find( "models/monsters/skeleton/skeleton01_d#__1000", false ) >= 0 )
			{
				generatedName.Replace( "skeleton01_d#__1000", "skeleton01_d#__0100" );
				im.SetName( generatedName );
				binaryFileTime = im.LoadFromGeneratedFile( sourceFileTime );
				break;
			}
		}
	}
#endif
	
	const bimageFile_t& header = im.GetFileHeader();
	
	// RB begin
	if( image_exportPNG.GetBool() && ( usage == TD_FONT ) && binaryFileTime != FILE_NOT_FOUND_TIMESTAMP )
	{
		const bimageImage_t& img = im.GetImageHeader( 0 );
		
		const byte* data = im.GetImageData( 0 );
		
		//( img.level, 0, 0, img.destZ, img.width, img.height, data );
		
		idTempArray<byte> rgba( img.width * img.height * 4 );
		memset( rgba.Ptr(), 255, rgba.Size() );
		
		if( header.format == FMT_DXT1 )
		{
			idDxtDecoder dxt;
			dxt.DecompressImageDXT1( data, rgba.Ptr(), img.width, img.height );
			
			if( header.colorFormat == CFM_GREEN_ALPHA )
			{
				byte* pic = rgba.Ptr();
				for( int i = 0; i < img.width * img.height; i++ )
				{
					pic[i * 4 + 3] = pic[i * 4 + 1];
					pic[i * 4 + 0] = 255;
					pic[i * 4 + 1] = 255;
					pic[i * 4 + 2] = 255;
				}
			}
		}
		else if( header.format == FMT_DXT5 )
		{
			idDxtDecoder dxt;
			
			if( header.colorFormat == CFM_NORMAL_DXT5 )
			{
				dxt.DecompressNormalMapDXT5( data, rgba.Ptr(), img.width, img.height );
			}
			else if( header.colorFormat == CFM_YCOCG_DXT5 )
			{
				dxt.DecompressYCoCgDXT5( data, rgba.Ptr(), img.width, img.height );
			}
			else
			{
			
				dxt.DecompressImageDXT5( data, rgba.Ptr(), img.width, img.height );
			}
		}
		else if( header.format == FMT_LUM8 || header.format == FMT_INT8 )
		{
			// LUM8 and INT8 just read the red channel
			byte* pic = rgba.Ptr();
			for( int i = 0; i < img.dataSize; i++ )
			{
				pic[ i * 4 ] = data[ i ];
			}
		}
		else if( header.format == FMT_ALPHA )
		{
			// ALPHA reads the alpha channel
			byte* pic = rgba.Ptr();
			for( int i = 0; i < img.dataSize; i++ )
			{
				pic[ i * 4 + 3 ] = data[ i ];
			}
		}
		else if( header.format == FMT_L8A8 )
		{
			// L8A8 reads the alpha and red channels
			byte* pic = rgba.Ptr();
			for( int i = 0; i < img.dataSize / 2; i++ )
			{
				pic[ i * 4 + 0 ] = data[ i * 2 + 0 ];
				pic[ i * 4 + 3 ] = data[ i * 2 + 1 ];
			}
		}
		else if( header.format == FMT_RGB565 )
		{
			// FIXME
			/*
			byte* pic = rgba.Ptr();
			for( int i = 0; i < img.dataSize / 2; i++ )
			{
				unsigned short color = ( ( pic[ i * 4 + 0 ] >> 3 ) << 11 ) | ( ( pic[ i * 4 + 1 ] >> 2 ) << 5 ) | ( pic[ i * 4 + 2 ] >> 3 );
				img.data[ i * 2 + 0 ] = ( color >> 8 ) & 0xFF;
				img.data[ i * 2 + 1 ] = color & 0xFF;
			}
			*/
		}
		else
		{
			byte* pic = rgba.Ptr();
			for( int i = 0; i < img.dataSize; i++ )
			{
				pic[ i ] = data[ i ];
			}
		}
		
		idStr pngFileNameExport = generatedName;
		pngFileNameExport.SetFileExtension( ".png" );
		
		R_WritePNG( pngFileNameExport, rgba.Ptr(), 4, img.width, img.height, true, "fs_basepath" );
	}
	// RB end
	
	
	// RB TODO
	if( /*( fileSystem->InProductionMode() && binaryFileTime != FILE_NOT_FOUND_TIMESTAMP ) ||*/ ( ( binaryFileTime != FILE_NOT_FOUND_TIMESTAMP )
			&& ( header.colorFormat == opts.colorFormat )
			&& ( header.format == opts.format )
			&& ( header.textureType == opts.textureType )
																								) )
	{
		opts.width = header.width;
		opts.height = header.height;
		opts.numLevels = header.numLevels;
		opts.colorFormat = ( textureColor_t )header.colorFormat;
		opts.format = ( textureFormat_t )header.format;
		opts.textureType = ( textureType_t )header.textureType;
		
		/*
		if( cvarSystem->GetCVarBool( "fs_buildresources" ) )
		{
			// for resource gathering write this image to the preload file for this map
			fileSystem->AddImagePreload( GetName(), filter, repeat, usage, cubeFiles );
		}
		*/
	}
	else
	{
		if( cubeFiles != CF_2D )
		{
			int size;
			byte* pics[6];
			
			if( !R_LoadCubeImages( GetName(), cubeFiles, pics, &size, &sourceFileTime ) || size == 0 )
			{
				idLib::Warning( "Couldn't load cube image: %s", GetName() );
				return;
			}
			
			opts.textureType = TT_CUBIC;
			repeat = TR_CLAMP;
			opts.width = size;
			opts.height = size;
			opts.numLevels = 0;
			DeriveOpts();
			im.LoadCubeFromMemory( size, ( const byte** )pics, opts.numLevels, opts.format, opts.gammaMips );
			repeat = TR_CLAMP;
			
			for( int i = 0; i < 6; i++ )
			{
				if( pics[i] )
				{
					Mem_Free( pics[i] );
				}
			}
		}
		else
		{
			int width, height;
			byte* pic;
			
			// load the full specification, and perform any image program calculations
			R_LoadImageProgram( GetName(), &pic, &width, &height, &sourceFileTime, &usage );
			
			if( pic == NULL )
			{
				idLib::Warning( "Couldn't load image: %s : %s", GetName(), generatedName.c_str() );
				// create a default so it doesn't get continuously reloaded
				opts.width = 8;
				opts.height = 8;
				opts.numLevels = 1;
				DeriveOpts();
				AllocImage();
				
				// clear the data so it's not left uninitialized
				idTempArray<byte> clear( opts.width * opts.height * 4 );
				memset( clear.Ptr(), 0, clear.Size() );
				for( int level = 0; level < opts.numLevels; level++ )
				{
					SubImageUpload( level, 0, 0, 0, opts.width >> level, opts.height >> level, clear.Ptr() );
				}
				
				return;
			}
			
			opts.width = width;
			opts.height = height;
			opts.numLevels = 0;
			DeriveOpts();
			im.Load2DFromMemory( opts.width, opts.height, pic, opts.numLevels, opts.format, opts.colorFormat, opts.gammaMips );
			
			Mem_Free( pic );
		}
		binaryFileTime = im.WriteGeneratedFile( sourceFileTime );
	}
	
	AllocImage();
	
	
	for( int i = 0; i < im.NumImages(); i++ )
	{
		const bimageImage_t& img = im.GetImageHeader( i );
		const byte* data = im.GetImageData( i );
		SubImageUpload( img.level, 0, 0, img.destZ, img.width, img.height, data );
	}
}

/*
==============
Bind

Automatically enables 2D mapping or cube mapping if needed
==============
*/
void idImage::Bind()
{

	RENDERLOG_PRINTF( "idImage::Bind( %s )\n", GetName() );
	
	// load the image if necessary (FIXME: not SMP safe!)
	if( !IsLoaded() )
	{
		// load the image on demand here, which isn't our normal game operating mode
		ActuallyLoadImage( true );
	}
	
	const int texUnit = backEnd.glState.currenttmu;
	
	// RB: added support for more types
	tmu_t* tmu = &backEnd.glState.tmu[texUnit];
	// bind the texture
	if( opts.textureType == TT_2D )
	{
		if( tmu->current2DMap != texnum )
		{
			tmu->current2DMap = texnum;
			
#if !defined(USE_GLES2) && !defined(USE_GLES3)
			if( glConfig.directStateAccess )
			{
				glBindMultiTextureEXT( GL_TEXTURE0 + texUnit, GL_TEXTURE_2D, texnum );
			}
			else
#endif
			{
				glActiveTexture( GL_TEXTURE0 + texUnit );
				glBindTexture( GL_TEXTURE_2D, texnum );
			}
		}
	}
	else if( opts.textureType == TT_CUBIC )
	{
		if( tmu->currentCubeMap != texnum )
		{
			tmu->currentCubeMap = texnum;
			
#if !defined(USE_GLES2) && !defined(USE_GLES3)
			if( glConfig.directStateAccess )
			{
				glBindMultiTextureEXT( GL_TEXTURE0 + texUnit, GL_TEXTURE_CUBE_MAP, texnum );
			}
			else
#endif
			{
				glActiveTexture( GL_TEXTURE0 + texUnit );
				glBindTexture( GL_TEXTURE_CUBE_MAP, texnum );
			}
		}
	}
	else if( opts.textureType == TT_2D_ARRAY )
	{
		if( tmu->current2DArray != texnum )
		{
			tmu->current2DArray = texnum;
			
#if !defined(USE_GLES2) && !defined(USE_GLES3)
			if( glConfig.directStateAccess )
			{
				glBindMultiTextureEXT( GL_TEXTURE0 + texUnit, GL_TEXTURE_2D_ARRAY, texnum );
			}
			else
#endif
			{
				glActiveTexture( GL_TEXTURE0 + texUnit );
				glBindTexture( GL_TEXTURE_2D_ARRAY, texnum );
			}
		}
	}
	else if( opts.textureType == TT_2D_MULTISAMPLE )
	{
		if( tmu->current2DMap != texnum )
		{
			tmu->current2DMap = texnum;
			
#if !defined(USE_GLES2) && !defined(USE_GLES3)
			if( glConfig.directStateAccess )
			{
				glBindMultiTextureEXT( GL_TEXTURE0 + texUnit, GL_TEXTURE_2D_MULTISAMPLE, texnum );
			}
			else
#endif
			{
				glActiveTexture( GL_TEXTURE0 + texUnit );
				glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, texnum );
			}
		}
	}
	// RB end
}

/*
================
MakePowerOfTwo
================
*/
int MakePowerOfTwo( int num )
{
	int	pot;
	for( pot = 1; pot < num; pot <<= 1 )
	{
	}
	return pot;
}

/*
====================
CopyFramebuffer
====================
*/
void idImage::CopyFramebuffer( int x, int y, int imageWidth, int imageHeight )
{
	int target = GL_TEXTURE_2D;
	switch( opts.textureType )
	{
		case TT_2D:
			target = GL_TEXTURE_2D;
			break;
		case TT_CUBIC:
			target = GL_TEXTURE_CUBE_MAP;
			break;
		case TT_2D_ARRAY:
			target = GL_TEXTURE_2D_ARRAY;
			break;
		case TT_2D_MULTISAMPLE:
			target = GL_TEXTURE_2D_MULTISAMPLE;
			break;
		default:
			//idLib::FatalError( "%s: bad texture type %d", GetName(), opts.textureType );
			return;
	}
	
	glBindTexture( target, texnum );
	
#if !defined(USE_GLES2)
	if( Framebuffer::IsDefaultFramebufferActive() )
	{
		glReadBuffer( GL_BACK );
	}
#endif
	
	opts.width = imageWidth;
	opts.height = imageHeight;
	
#if defined(USE_GLES2)
	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, x, y, imageWidth, imageHeight, 0 );
#else
	if( r_useHDR.GetBool() && globalFramebuffers.hdrFBO->IsBound() )
	{
	
		//if( backEnd.glState.currentFramebuffer != NULL && backEnd.glState.currentFramebuffer->IsMultiSampled() )
	
		if( globalFramebuffers.hdrFBO->IsMultiSampled() )
		{
			glBindFramebuffer( GL_READ_FRAMEBUFFER, globalFramebuffers.hdrFBO->GetFramebuffer() );
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, globalFramebuffers.hdrNonMSAAFBO->GetFramebuffer() );
			glBlitFramebuffer( 0, 0, glConfig.nativeScreenWidth, glConfig.nativeScreenHeight,
							   0, 0, glConfig.nativeScreenWidth, glConfig.nativeScreenHeight,
							   GL_COLOR_BUFFER_BIT,
							   GL_LINEAR );
	
			globalFramebuffers.hdrNonMSAAFBO->Bind();
	
			glCopyTexImage2D( target, 0, GL_RGBA16F, x, y, imageWidth, imageHeight, 0 );
	
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			glCopyTexImage2D( target, 0, GL_RGBA16F, x, y, imageWidth, imageHeight, 0 );
		}
	}
	else
	{
		glCopyTexImage2D( target, 0, GL_RGBA8, x, y, imageWidth, imageHeight, 0 );
	}
#endif
	
	// these shouldn't be necessary if the image was initialized properly
	glTexParameterf( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glTexParameterf( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	backEnd.pc.c_copyFrameBuffer++;
}

/*
====================
CopyDepthbuffer
====================
*/
void idImage::CopyDepthbuffer( int x, int y, int imageWidth, int imageHeight )
{
	glBindTexture( ( opts.textureType == TT_CUBIC ) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texnum );
	
	opts.width = imageWidth;
	opts.height = imageHeight;
	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, x, y, imageWidth, imageHeight, 0 );
	
	backEnd.pc.c_copyFrameBuffer++;
}

/*
=============
RB_UploadScratchImage

if rows = cols * 6, assume it is a cube map animation
=============
*/
void idImage::UploadScratch( const byte* data, int cols, int rows )
{

	// if rows = cols * 6, assume it is a cube map animation
	if( rows == cols * 6 )
	{
		rows /= 6;
		const byte* pic[6];
		for( int i = 0; i < 6; i++ )
		{
			pic[i] = data + cols * rows * 4 * i;
		}
		
		if( opts.textureType != TT_CUBIC || usage != TD_LOOKUP_TABLE_RGBA )
		{
			GenerateCubeImage( pic, cols, TF_LINEAR, TD_LOOKUP_TABLE_RGBA );
			return;
		}
		if( opts.width != cols || opts.height != rows )
		{
			opts.width = cols;
			opts.height = rows;
			AllocImage();
		}
		SetSamplerState( TF_LINEAR, TR_CLAMP );
		for( int i = 0; i < 6; i++ )
		{
			SubImageUpload( 0, 0, 0, i, opts.width, opts.height, pic[i] );
		}
		
	}
	else
	{
		if( opts.textureType != TT_2D || usage != TD_LOOKUP_TABLE_RGBA )
		{
			GenerateImage( data, cols, rows, TF_LINEAR, TR_REPEAT, TD_LOOKUP_TABLE_RGBA );
			return;
		}
		if( opts.width != cols || opts.height != rows )
		{
			opts.width = cols;
			opts.height = rows;
			AllocImage();
		}
		SetSamplerState( TF_LINEAR, TR_REPEAT );
		SubImageUpload( 0, 0, 0, 0, opts.width, opts.height, data );
	}
}

/*
==================
StorageSize
==================
*/
int idImage::StorageSize() const
{

	if( !IsLoaded() )
	{
		return 0;
	}
	int baseSize = opts.width * opts.height;
	if( opts.numLevels > 1 )
	{
		baseSize *= 4;
		baseSize /= 3;
	}
	baseSize *= BitsForFormat( opts.format );
	baseSize /= 8;
	return baseSize;
}

/*
==================
Print
==================
*/
void idImage::Print() const
{
	if( generatorFunction )
	{
		common->Printf( "F" );
	}
	else
	{
		common->Printf( " " );
	}
	
	switch( opts.textureType )
	{
		case TT_2D:
			common->Printf( " " );
			break;
		case TT_CUBIC:
			common->Printf( "C" );
			break;
		default:
			common->Printf( "<BAD TYPE:%i>", opts.textureType );
			break;
	}
	
	common->Printf( "%4i %4i ",	opts.width, opts.height );
	
	switch( opts.format )
	{
#define NAME_FORMAT( x ) case FMT_##x: common->Printf( "%-16s ", #x ); break;
			NAME_FORMAT( NONE );
			NAME_FORMAT( RGBA8 );
			NAME_FORMAT( XRGB8 );
			NAME_FORMAT( RGB565 );
			NAME_FORMAT( L8A8 );
			NAME_FORMAT( ALPHA );
			NAME_FORMAT( LUM8 );
			NAME_FORMAT( INT8 );
			NAME_FORMAT( DXT1 );
			NAME_FORMAT( DXT5 );
			// RB begin
			NAME_FORMAT( ETC1_RGB8_OES );
			// RB end
			NAME_FORMAT( DEPTH );
			NAME_FORMAT( X16 );
			NAME_FORMAT( Y16_X16 );
		default:
			common->Printf( "<%3i>", opts.format );
			break;
	}
	
	switch( filter )
	{
		case TF_DEFAULT:
			common->Printf( "mip  " );
			break;
		case TF_LINEAR:
			common->Printf( "linr " );
			break;
		case TF_NEAREST:
			common->Printf( "nrst " );
			break;
		default:
			common->Printf( "<BAD FILTER:%i>", filter );
			break;
	}
	
	switch( repeat )
	{
		case TR_REPEAT:
			common->Printf( "rept " );
			break;
		case TR_CLAMP_TO_ZERO:
			common->Printf( "zero " );
			break;
		case TR_CLAMP_TO_ZERO_ALPHA:
			common->Printf( "azro " );
			break;
		case TR_CLAMP:
			common->Printf( "clmp " );
			break;
		default:
			common->Printf( "<BAD REPEAT:%i>", repeat );
			break;
	}
	
	common->Printf( "%4ik ", StorageSize() / 1024 );
	
	common->Printf( " %s\n", GetName() );
}

/*
===============
idImage::Reload
===============
*/
void idImage::Reload( bool force )
{
	// always regenerate functional images
	if( generatorFunction )
	{
		common->DPrintf( "regenerating %s.\n", GetName() );
		generatorFunction( this );
		return;
	}
	
	// check file times
	if( !force )
	{
		ID_TIME_T current;
		if( cubeFiles != CF_2D )
		{
			R_LoadCubeImages( imgName, cubeFiles, NULL, NULL, &current );
		}
		else
		{
			// get the current values
			R_LoadImageProgram( imgName, NULL, NULL, NULL, &current );
		}
		if( current <= sourceFileTime )
		{
			return;
		}
	}
	
	common->DPrintf( "reloading %s.\n", GetName() );
	
	PurgeImage();
	
	// Load is from the front end, so the back end must be synced
	ActuallyLoadImage( false );
}

/*
========================
idImage::SetSamplerState
========================
*/
void idImage::SetSamplerState( textureFilter_t tf, textureRepeat_t tr )
{
	if( tf == filter && tr == repeat )
	{
		return;
	}
	filter = tf;
	repeat = tr;
	glBindTexture( ( opts.textureType == TT_CUBIC ) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texnum );
	SetTexParameters();
}
