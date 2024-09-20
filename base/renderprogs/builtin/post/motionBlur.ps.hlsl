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
uniform sampler2D samp0 : register(s0);   // view color
uniform sampler2D samp1 : register(s1);   // view depth

struct PS_IN {
	float2 texcoord0 : TEXCOORD0_centroid;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
#if 0
	if( fragment.texcoord0.x < 0.5 )
	{
		// only draw on half the screen for comparison
		discard;
	}
#endif
	// don't motion blur the hands, which were drawn with alpha = 0
	if( tex2D( samp0, fragment.texcoord0 ).w == 0.0 )
	{
		discard;
	}

	// derive clip space from the depth buffer and screen position
	float windowZ = tex2D( samp1, fragment.texcoord0 ).x;
	float3 ndc = float3( fragment.texcoord0 * 2.0 - 1.0, windowZ * 2.0 - 1.0 );
	float clipW = -rpProjectionMatrixZ.w / ( -rpProjectionMatrixZ.z - ndc.z );

	float4 clip = float4( ndc * clipW, clipW );

	// convert from clip space this frame to clip space previous frame
	float4 reClip;
	reClip.x = dot( rpMVPmatrixX, clip );
	reClip.y = dot( rpMVPmatrixY, clip );
	reClip.z = dot( rpMVPmatrixZ, clip );
	reClip.w = dot( rpMVPmatrixW, clip );

	// convert to NDC values
	float2 prevTexCoord;
	prevTexCoord.x = ( reClip.x / reClip.w ) * 0.5 + 0.5;
	prevTexCoord.y = ( reClip.y / reClip.w ) * 0.5 + 0.5;

	// sample along the line from prevTexCoord to fragment.texcoord0

	float2 texCoord = prevTexCoord; //fragment.texcoord0;
	float2 delta = ( fragment.texcoord0 - prevTexCoord );

	float3 sum = float3( 0.0 );
	float goodSamples = 0.0;
	float samples = rpOverbright.x;

	for( float i = 0.0 ; i < samples ; i = i + 1.0 )
	{
		float2 pos = fragment.texcoord0 + delta * ( ( i / ( samples - 1.0 ) ) - 0.5 );
		float4 color = tex2D( samp0, pos );
		// only take the values that are not part of the weapon
		sum += color.xyz * color.w;
		goodSamples += color.w;
	}
	float invScale = 1.0 / goodSamples;

	result.color = float4( sum * invScale, 1.0 );
}
