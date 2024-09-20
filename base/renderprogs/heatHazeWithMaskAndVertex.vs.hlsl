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


// RB: no GPU skinning with ES 2.0
#if defined(USE_GPU_SKINNING)
uniform matrices_ubo { float4 matrices[408]; };
#endif
// RB end

// *INDENT-OFF*
uniform float4 rpUser0 : register(c128); // rpScroll
uniform	float4 rpUser1 : register(c129); // rpDeformMagnitude

struct VS_IN {
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 color : COLOR0;
	float4 color2 : COLOR1;
};

struct VS_OUT {
	float4 position		: POSITION;
	float4 texcoord0	: TEXCOORD0;
	float4 texcoord1	: TEXCOORD1;
	float4 texcoord2	: TEXCOORD2;
	float4 color		: COLOR0;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{

#include "renderprogs/skinning.inc.hlsl"


	// texture 0 takes the texture coordinates unmodified
	result.texcoord0 = float4( vertex.texcoord, 0 , 0 );

	// texture 1 takes the texture coordinates and adds a scroll
	const float4 textureScroll = rpUser0;
	result.texcoord1 = float4( vertex.texcoord, 0, 0 ) + textureScroll;

	// texture 2 takes the deform magnitude and scales it by the projection distance
	float4 vec = float4( 0, 1, 0, 1 );
	vec.z = dot4( modelPosition, rpModelViewMatrixZ );

	// magicProjectionAdjust is a magic scalar that scales the projection since we changed from
	// using the X axis to the Y axis to calculate x.  It is an approximation to closely match
	// what the original game did
	const float magicProjectionAdjust = 0.43f;
	float x = dot4( vec, rpProjectionMatrixY ) * magicProjectionAdjust;
	float w = dot4( vec, rpProjectionMatrixW );

	// don't let the recip get near zero for polygons that cross the view plane
	w = max( w, 1.0f );
	x /= w;
	//x = x * ( 1.0f / w );

	// clamp the distance so the the deformations don't get too wacky near the view
	x = min( x, 0.02f );

	const float4 deformMagnitude = rpUser1;
	result.texcoord2 = x * deformMagnitude;
	result.color = swizzleColor( vertex.color );
}