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
#ifndef __BINARYIMAGEDATA_H__
#define __BINARYIMAGEDATA_H__

/*
================================================================================================

This is where the Binary image headers go that are also included by external tools such as the cloud.

================================================================================================
*/

// These structures are used for memory mapping bimage files, but
// not for the normal loading, so be careful making changes.
// Values are big endien to reduce effort on consoles.
#define BIMAGE_VERSION 10
#define BIMAGE_MAGIC (unsigned int)( ('B'<<0)|('I'<<8)|('M'<<16)|(BIMAGE_VERSION<<24) )

struct bimageImage_t
{
	int		level;		// mip
	int		destZ;		// array slice
	int		width;
	int		height;
	int		dataSize;	// dataSize bytes follow
};

#pragma pack( push, 1 )
struct bimageFile_t
{
	ID_TIME_T	sourceFileTime;
	int		headerMagic;
	int		textureType;
	int		format;
	int		colorFormat;
	int		width;
	int		height;
	int		numLevels;
	// one or more bimageImage_t structures follow
};
#pragma pack( pop )

#endif // __BINARYIMAGEDATA_H__
