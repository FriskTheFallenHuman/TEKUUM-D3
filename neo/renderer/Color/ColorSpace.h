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
#ifndef __COLORSPACE_H__
#define __COLORSPACE_H__

/*
================================================================================================
Contains the ColorSpace conversion declarations.
================================================================================================
*/

namespace idColorSpace
{
void	ConvertRGBToYCoCg( byte* dst, const byte* src, int width, int height );
void	ConvertYCoCgToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertRGBToCoCg_Y( byte* dst, const byte* src, int width, int height );
void	ConvertCoCg_YToRGB( byte* dst, const byte* src, int width, int height );
void	ConvertCoCgSYToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertRGBToYCoCg420( byte* dst, const byte* src, int width, int height );
void	ConvertYCoCg420ToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertRGBToYCbCr( byte* dst, const byte* src, int width, int height );
void	ConvertYCbCrToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertRGBToCbCr_Y( byte* dst, const byte* src, int width, int height );
void	ConvertCbCr_YToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertRGBToYCbCr420( byte* dst, const byte* src, int width, int height );
void	ConvertYCbCr420ToRGB( byte* dst, const byte* src, int width, int height );

void	ConvertNormalMapToStereographicHeightMap( byte* heightMap, const byte* normalMap, int width, int height, float& scale );
void	ConvertStereographicHeightMapToNormalMap( byte* normalMap, const byte* heightMap, int width, int height, float scale );

void	ConvertRGBToMonochrome( byte* mono, const byte* rgb, int width, int height );
void	ConvertMonochromeToRGB( byte* rgb, const byte* mono, int width, int height );
};

#endif // !__COLORSPACE_H__
