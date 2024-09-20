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
	//float4 color	 : COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{

	float4 redTint = float4( 1, 0.98, 0.98, 1 );
	float4 accumSample = tex2D( samp0, fragment.texcoord0 ) * redTint;

	float4 maskSample = tex2D( samp2, fragment.texcoord1 );

	float4 tint = float4( 1.0, 0.8, 0.8, 1 );
	float4 currentRenderSample = tex2D( samp1, fragment.texcoord2 ) * tint;

	result.color = lerp( accumSample, currentRenderSample, maskSample.a );
}