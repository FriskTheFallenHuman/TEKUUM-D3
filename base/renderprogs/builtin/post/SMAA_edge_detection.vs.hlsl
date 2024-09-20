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


#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 0
#include "SMAA.inc.hlsl"

// *INDENT-OFF*
struct VS_IN 
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 color : COLOR0;
};

struct VS_OUT
{
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float4 texcoord1 : TEXCOORD1;
	float4 texcoord2 : TEXCOORD2;
	float4 texcoord3 : TEXCOORD3;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	result.position = vertex.position;

	float2 texcoord = vertex.texcoord;
	//float2 texcoord = float2( vertex.texcoord.s, 1.0 - vertex.texcoord.t );

	result.texcoord0 = texcoord;

	float4 offset[3];
	SMAAEdgeDetectionVS( texcoord, offset );

	result.texcoord1 = offset[0];
	result.texcoord2 = offset[1];
	result.texcoord3 = offset[2];
}