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
uniform sampler2D samp0 : register(s0); //_accum
uniform sampler2D samp1 : register(s1); //_currentRender
uniform sampler2D samp2 : register(s2); //mask

struct PS_IN {
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
	float2 texcoord1 : TEXCOORD1_centroid;
	float2 texcoord2 : TEXCOORD2_centroid;
	float2 texcoord3 : TEXCOORD3_centroid;
	float2 texcoord4 : TEXCOORD4;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	float colorFactor = fragment.texcoord4.x;

	float4 color0 = float4( 1.0f - colorFactor,	1.0f - colorFactor,		1.0f,	1.0f );
	float4 color1 = float4( 1.0f,	0.95f - colorFactor,		0.95f,	0.5f );
	float4 color2 = float4( 0.015f,	0.015f, 0.015f, 0.01f );

	float4 accumSample0 = tex2D( samp0, fragment.texcoord0 ) * color0;
	float4 accumSample1 = tex2D( samp0, fragment.texcoord1 ) * color1;
	float4 accumSample2 = tex2D( samp0, fragment.texcoord2 ) * color2;
	float4 maskSample = tex2D( samp2, fragment.texcoord3 );

	float4 tint = float4( 0.8, 0.5, 0.5, 1 );
	float4 currentRenderSample = tex2D( samp1, fragment.texcoord3 ) * tint;

	// blend of the first 2 accumulation samples
	float4 accumColor = lerp( accumSample0, accumSample1, 0.5f );
	// add thrid sample
	accumColor += accumSample2;

	accumColor = lerp( accumColor, currentRenderSample, maskSample.a );
	result.color = accumColor;
}