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
uniform float4 rpUser0 : register( c128 ); //rpCenterScaleTex
uniform float4 rpUser1 : register( c129 ); //rpRotateTex
uniform float4 rpUser2 : register( c130 ); //rpColorFactor

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
	float2 texcoord3 : TEXCOORD3;
	float2 texcoord4 : TEXCOORD4;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	result.position.x = dot4( vertex.position, rpMVPmatrixX );
	result.position.y = dot4( vertex.position, rpMVPmatrixY );
	result.position.z = dot4( vertex.position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.position, rpMVPmatrixW );

	//_accum 1
	const float4 centerScaleTex = rpUser0;
	const float4 rotateTex = rpUser1;
	float2 tc0 = CenterScale( vertex.texcoord, centerScaleTex.xy );
	result.texcoord0 = Rotate2D( tc0, rotateTex.xy );

	// accum 2
	result.texcoord1 = Rotate2D( tc0, float2( rotateTex.z, -rotateTex.w ) );

	// accum 3
	result.texcoord2 = Rotate2D( tc0, rotateTex.zw );

	// pass through for currentrender
	result.texcoord3 = vertex.texcoord;

	// pass through the color fator
	const float4 colorFactor = rpUser2;
	result.texcoord4 = colorFactor.xx;
}