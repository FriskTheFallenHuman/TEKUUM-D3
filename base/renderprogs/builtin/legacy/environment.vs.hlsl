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
struct VS_IN {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR0;
};

struct VS_OUT {
	float4 position		: POSITION;
	float3 texcoord0	: TEXCOORD0;
	float3 texcoord1	: TEXCOORD1;
	float4 color		: COLOR0;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{

	float4 vNormal = vertex.normal * 2.0 - 1.0;

	result.position.x = dot4( vertex.position, rpMVPmatrixX );
	result.position.y = dot4( vertex.position, rpMVPmatrixY );
	result.position.z = dot4( vertex.position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.position, rpMVPmatrixW );

	float4 toEye = rpLocalViewOrigin - vertex.position;

	result.texcoord0 = toEye.xyz;
	result.texcoord1 = vNormal.xyz;

	result.color = sRGBAToLinearRGBA( rpColor );
}
