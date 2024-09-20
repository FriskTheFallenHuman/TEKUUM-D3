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


// *INDENT-OFF*
uniform sampler2D	samp0 : register(s0); // texture 0 is _current Render
uniform sampler2D	samp1 : register(s1); // texture 1 is the per-surface bump map

struct PS_IN {
	float4 position		: VPOS;
	float2 texcoord		: TEXCOORD0_centroid;
	float4 color		: COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{

	float2 screenTexCoord = fragment.texcoord;

	// compute warp factor
	float4 warpFactor = 1.0 - ( tex2D( samp1, screenTexCoord.xy ) * fragment.color );
	screenTexCoord -= float2( 0.5, 0.5 );
	screenTexCoord *= warpFactor.xy;
	screenTexCoord += float2( 0.5, 0.5 );

	// load the screen render
	result.color = tex2D( samp0, screenTexCoord );

}