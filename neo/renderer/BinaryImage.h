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
#ifndef __BINARYIMAGE_H__
#define __BINARYIMAGE_H__

#include "BinaryImageData.h"

/*
================================================
idBinaryImage is used by the idImage class for constructing mipmapped
textures and for loading and saving generated files by idImage.
Also used in a memory-mapped form for imageCPU for offline megatexture
generation.
================================================
*/
class idBinaryImage
{
public:
	idBinaryImage( const char* name ) : imgName( name ) { }

	const char* 		GetName() const
	{
		return imgName.c_str();
	}
	void				SetName( const char* _name )
	{
		imgName = _name;
	}

	void				Load2DFromMemory( int width, int height, const byte* pic_const, int numLevels, textureFormat_t& textureFormat, textureColor_t& colorFormat, bool gammaMips );
	void				Load2DAtlasMipchainFromMemory( int width, int height, const byte* pic_const, int numLevels, textureFormat_t& textureFormat, textureColor_t& colorFormat );
	void				LoadCubeFromMemory( int width, const byte* pics[6], int numLevels, textureFormat_t& textureFormat, bool gammaMips );

	bool				LoadFromGeneratedFile( idFile* f, ID_TIME_T sourceFileTime );
	ID_TIME_T			LoadFromGeneratedFile( ID_TIME_T sourceFileTime );
	ID_TIME_T			WriteGeneratedFile( ID_TIME_T sourceFileTime );

	const bimageFile_t& GetFileHeader()
	{
		return fileData;
	}

	int					NumImages()
	{
		return images.Num();
	}
	const bimageImage_t& 	GetImageHeader( int i ) const
	{
		return images[i];
	}
	const byte* 			GetImageData( int i ) const
	{
		return images[i].data;
	}
	static void			GetGeneratedFileName( idStr& gfn, const char* imageName );

private:
	idStr				imgName;			// game path, including extension (except for cube maps), may be an image program
	bimageFile_t		fileData;

	class idBinaryImageData : public bimageImage_t
	{
	public:
		byte* data;

		idBinaryImageData() : data( NULL ) { }
		~idBinaryImageData()
		{
			Free();
		}
		idBinaryImageData& operator=( idBinaryImageData& other )
		{
			if( this == &other )
			{
				return *this;
			}

			Alloc( other.dataSize );
			memcpy( data, other.data, other.dataSize );
			return *this;
		}
		idBinaryImageData& operator=( idBinaryImageData&& other )
		{
			if( this == &other )
			{
				return *this;
			}
			dataSize = other.dataSize;
			data = other.data;
			other.data = NULL;
			return *this;
		}
		void Free()
		{
			if( data != NULL )
			{
				Mem_Free( data );
				data = NULL;
				dataSize = 0;
			}
		}
		void Alloc( int size )
		{
			Free();
			dataSize = size;
			data = ( byte* )Mem_Alloc( size );
		}
	};

	idList< idBinaryImageData > images;

private:
	void				MakeGeneratedFileName( idStr& gfn );
};

#endif // __BINARYIMAGE_H__
