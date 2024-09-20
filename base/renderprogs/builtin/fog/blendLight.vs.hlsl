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
	float2 texcoord : TEXCOORD0;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 color : COLOR0;
};

struct VS_OUT {
	float4 position : POSITION;
	float4 texcoord0 : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	result.position.x = dot4( vertex.position, rpMVPmatrixX );
	result.position.y = dot4( vertex.position, rpMVPmatrixY );
	result.position.z = dot4( vertex.position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.position, rpMVPmatrixW );

	result.texcoord0.x = dot4( vertex.position, rpTexGen0S );
	result.texcoord0.y = dot4( vertex.position, rpTexGen0T );
	result.texcoord0.z = 0.0;
	result.texcoord0.w = dot4( vertex.position, rpTexGen0Q );

	result.texcoord1.x = dot4( vertex.position, rpTexGen1S );
	result.texcoord1.y = 0.5;
}
