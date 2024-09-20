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

#include "renderprogs/global.inc.hlsl"


uniform sampler2D samp0 :
register( s0 );

// *INDENT-OFF*
struct PS_IN {
	float4 position : VPOS;
	float4 texcoord0 : TEXCOORD0_centroid;
	float4 color : COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{

	// we always do a projective texture lookup so that we can support texgen
	// materials without a separate shader. Basic materials will have texture
	// coordinates with w = 1 which will result in a NOP projection when tex2Dproj
	// gets called.
	float4 texSample = idtex2Dproj( samp0, fragment.texcoord0 );

	result.color = sRGBAToLinearRGBA( texSample ) * fragment.color;
}