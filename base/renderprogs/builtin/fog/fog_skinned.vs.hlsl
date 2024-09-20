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


uniform matrices_ubo { float4 matrices[408]; };

// *INDENT-OFF*
struct VS_IN {
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 color : COLOR0;
    float4 color2 : COLOR1;
};

struct VS_OUT {
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	//--------------------------------------------------------------
	// GPU transformation of the normal / binormal / bitangent
	//
	// multiplying with 255.1 give us the same result and is faster than floor( w * 255 + 0.5 )
	//--------------------------------------------------------------
	const float w0 = vertex.color2.x;
	const float w1 = vertex.color2.y;
	const float w2 = vertex.color2.z;
	const float w3 = vertex.color2.w;

	float4 matX, matY, matZ;	// must be float4 for vec4
	int joint = int( vertex.color.x * 255.1 * 3.0 );
	matX = matrices[int( joint + 0 )] * w0;
	matY = matrices[int( joint + 1 )] * w0;
	matZ = matrices[int( joint + 2 )] * w0;

	joint = int( vertex.color.y * 255.1 * 3.0 );
	matX += matrices[int( joint + 0 )] * w1;
	matY += matrices[int( joint + 1 )] * w1;
	matZ += matrices[int( joint + 2 )] * w1;

	joint = int( vertex.color.z * 255.1 * 3.0 );
	matX += matrices[int( joint + 0 )] * w2;
	matY += matrices[int( joint + 1 )] * w2;
	matZ += matrices[int( joint + 2 )] * w2;

	joint = int( vertex.color.w * 255.1 * 3.0 );
	matX += matrices[int( joint + 0 )] * w3;
	matY += matrices[int( joint + 1 )] * w3;
	matZ += matrices[int( joint + 2 )] * w3;

	float4 modelPosition;
	modelPosition.x = dot4( matX, vertex.position );
	modelPosition.y = dot4( matY, vertex.position );
	modelPosition.z = dot4( matZ, vertex.position );
	modelPosition.w = 1.0;
	// end of skinning

	// start of fog portion
	result.position.x = dot4( modelPosition, rpMVPmatrixX );
	result.position.y = dot4( modelPosition, rpMVPmatrixY );
	result.position.z = dot4( modelPosition, rpMVPmatrixZ );
	result.position.w = dot4( modelPosition, rpMVPmatrixW );

	result.texcoord0.x = dot4( modelPosition, rpTexGen0S );
	result.texcoord0.y = dot4( modelPosition, rpTexGen0T );

	result.texcoord1.x = dot4( modelPosition, rpTexGen1S );
	result.texcoord1.y = dot4( modelPosition, rpTexGen1T );
}
