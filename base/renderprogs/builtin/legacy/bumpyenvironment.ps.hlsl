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
uniform samplerCUBE	samp0 : register(s0); // texture 0 is the cube map
uniform sampler2D	samp1 : register(s1); // normal map

struct PS_IN {
	float4 position		: VPOS;
	float2 texcoord0	: TEXCOORD0_centroid;
	float3 texcoord1	: TEXCOORD1_centroid;
	float3 texcoord2	: TEXCOORD2_centroid;
	float3 texcoord3	: TEXCOORD3_centroid;
	float3 texcoord4	: TEXCOORD4_centroid;
	float4 color		: COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{

	float4 bump = tex2D( samp1, fragment.texcoord0 ) * 2.0f - 1.0f;
	// RB begin
	float3 localNormal;
#if defined(USE_NORMAL_FMT_RGB8)
	localNormal = float3( bump.rg, 0.0f );
#else
	localNormal = float3( bump.wy, 0.0f );
#endif
	// RB end
	localNormal.z = sqrt( 1.0f - dot3( localNormal, localNormal ) );

	float3 globalNormal;
	globalNormal.x = dot3( localNormal, fragment.texcoord2 );
	globalNormal.y = dot3( localNormal, fragment.texcoord3 );
	globalNormal.z = dot3( localNormal, fragment.texcoord4 );

	float3 globalEye = normalize( fragment.texcoord1 );

	float3 reflectionVector = globalNormal * dot3( globalEye, globalNormal );
	reflectionVector = ( reflectionVector * 2.0f ) - globalEye;

	float4 envMap = texCUBE( samp0, reflectionVector );

	result.color = float4( sRGBToLinearRGB( envMap.xyz ), 1.0f ) * fragment.color;
}
