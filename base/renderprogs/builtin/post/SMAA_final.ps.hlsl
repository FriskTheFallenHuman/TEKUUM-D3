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
uniform sampler2D samp0		: register(s0);		// _currentColor
uniform sampler2D samp1		: register(s1);		// _smaaBlend
//uniform sampler2D samp2		: register(s1);		// _velocity

struct PS_IN
{
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
	float4 texcoord1 : TEXCOORD1_centroid;
};

struct PS_OUT
{
	float4 color : COLOR;
};
// *INDENT-ON*


void main( PS_IN fragment, out PS_OUT result )
{
	float2 texcoord = fragment.texcoord0;

	float4 offset = fragment.texcoord1;

	float4 color = SMAANeighborhoodBlendingPS( texcoord,
				   offset,
				   samp0,
				   samp1
#if SMAA_REPROJECTION
				   , SMAATexture2D( velocityTex )
#endif
											 );

	//color = tex2D( samp1, texcoord );
	//color = float4( samp1 );
	result.color = color;
}
