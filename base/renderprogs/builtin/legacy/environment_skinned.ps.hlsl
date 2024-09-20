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

struct PS_IN {
	float4 position		: VPOS;
	float3 texcoord0	: TEXCOORD0_centroid;
	float3 texcoord1	: TEXCOORD1_centroid;
	float4 color		: COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{

	float3 globalNormal = normalize( fragment.texcoord1 );
	float3 globalEye = normalize( fragment.texcoord0 );

	float3 reflectionVector = _float3( dot3( globalEye, globalNormal ) );
	reflectionVector *= globalNormal;
	reflectionVector = ( reflectionVector * 2.0f ) - globalEye;

	float4 envMap = texCUBE( samp0, reflectionVector );

	result.color = float4( sRGBToLinearRGB( envMap.xyz ), 1.0f ) * fragment.color;
}
