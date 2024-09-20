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
uniform sampler2D samp0 : register(s0); // Y
uniform sampler2D samp1 : register(s1); // Cr
uniform sampler2D samp2 : register(s2); // Cb

struct PS_IN {
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	const float3 crc = float3( 1.595794678, -0.813476563, 0 );
	const float3 crb = float3( 0, -0.391448975, 2.017822266 );
	const float3 adj = float3( -0.87065506, 0.529705048f, -1.081668854f );
	const float3 YScalar = float3( 1.164123535f, 1.164123535f, 1.164123535f );

	float Y = tex2D( samp0, fragment.texcoord0.xy ).x;
	float Cr = tex2D( samp1, fragment.texcoord0.xy ).x;
	float Cb = tex2D( samp2, fragment.texcoord0.xy ).x;

	float3 p = ( YScalar * Y );
	p += ( crc * Cr ) + ( crb * Cb ) + adj;

	float4 color;
	color.xyz = p;
	color.w = 1.0;
	color *= rpColor;

	result.color = sRGBAToLinearRGBA( color );
}