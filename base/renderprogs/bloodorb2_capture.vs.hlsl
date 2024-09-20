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
uniform float4 rpUser0 : register( c128 ); //rpCenterScaleTex0
uniform float4 rpUser1 : register( c129 ); //rpRotateTex0
uniform float4 rpUser2 : register( c130 ); //rpCenterScaleTex1

struct VS_IN {
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 color : COLOR0;
};

struct VS_OUT {
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	result.position.x = dot4( vertex.position, rpMVPmatrixX );
	result.position.y = dot4( vertex.position, rpMVPmatrixY );
	result.position.z = dot4( vertex.position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.position, rpMVPmatrixW );

	const float4 centerScaleTex0 = rpUser0;
	const float4 rotateTex0 = rpUser1;
	const float4 centerScaleTex1 = rpUser2;

	//center scale and rotate for _accum
	float2 tc0 = CenterScale( vertex.texcoord, centerScaleTex0.xy );
	result.texcoord0 = Rotate2D( tc0, rotateTex0.xy );

	//center scale for mask
	result.texcoord1 = CenterScale( vertex.texcoord, centerScaleTex1.xy );

	// pass through for currentrender
	result.texcoord2 = vertex.texcoord;
}