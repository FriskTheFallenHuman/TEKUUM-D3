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


#define SMAA_INCLUDE_VS 0
#define SMAA_INCLUDE_PS 1

#include "SMAA.inc.hlsl"

// *INDENT-OFF*
uniform sampler2D samp0		: register(s0);		// _smaaEdges
uniform sampler2D samp1		: register(s1);		// _smaaArea
uniform sampler2D samp2		: register(s2);		// _smaaSearch

struct PS_IN
{
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
	float4 texcoord1 : TEXCOORD1_centroid;
	float4 texcoord2 : TEXCOORD2_centroid;
	float4 texcoord3 : TEXCOORD3_centroid;
	float4 texcoord4 : TEXCOORD4_centroid;
};

struct PS_OUT
{
	float4 color : COLOR;
};
// *INDENT-ON*


void main( PS_IN fragment, out PS_OUT result )
{
	float2 texcoord = fragment.texcoord0;

	float4 offset[3];
	offset[0] = fragment.texcoord1;
	offset[1] = fragment.texcoord2;
	offset[2] = fragment.texcoord3;

	float2 pixcoord = fragment.texcoord4.st;

	// TODO
	//float4 subsampleIndices = float4( 1.0, 1.0, 1.0, 0.0 );
	float4 subsampleIndices = float4( 0.0, 0.0, 0.0, 0.0 );

	float4 color = SMAABlendingWeightCalculationPS( texcoord,
				   pixcoord,
				   offset,
				   samp0,
				   samp1,
				   samp2,
				   subsampleIndices );

	//color = float4( texcoord.s, texcoord.t, 0.0, 1.0 );
	result.color = color;
}
