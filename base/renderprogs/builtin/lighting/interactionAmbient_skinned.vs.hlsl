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
	float4 position		: POSITION;
	float4 texcoord1	: TEXCOORD1;
	float4 texcoord2	: TEXCOORD2;
	float4 texcoord3	: TEXCOORD3;
	float4 texcoord4	: TEXCOORD4;
	float4 texcoord5	: TEXCOORD5;
	float4 texcoord6	: TEXCOORD6;
	float4 color		: COLOR0;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{

	float4 vNormal = vertex.normal * 2.0 - 1.0;
	float4 vTangent = vertex.tangent * 2.0 - 1.0;
	float3 vBinormal = cross( vNormal.xyz, vTangent.xyz ) * vTangent.w;

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

	float3 normal;
	normal.x = dot3( matX, vNormal );
	normal.y = dot3( matY, vNormal );
	normal.z = dot3( matZ, vNormal );
	normal = normalize( normal );

	float3 tangent;
	tangent.x = dot3( matX, vTangent );
	tangent.y = dot3( matY, vTangent );
	tangent.z = dot3( matZ, vTangent );
	tangent = normalize( tangent );

	float3 binormal;
	binormal.x = dot3( matX, vBinormal );
	binormal.y = dot3( matY, vBinormal );
	binormal.z = dot3( matZ, vBinormal );
	binormal = normalize( binormal );

	float4 modelPosition;
	modelPosition.x = dot4( matX, vertex.position );
	modelPosition.y = dot4( matY, vertex.position );
	modelPosition.z = dot4( matZ, vertex.position );
	modelPosition.w = 1.0;

	result.position.x = dot4( modelPosition, rpMVPmatrixX );
	result.position.y = dot4( modelPosition, rpMVPmatrixY );
	result.position.z = dot4( modelPosition, rpMVPmatrixZ );
	result.position.w = dot4( modelPosition, rpMVPmatrixW );

	float4 defaultTexCoord = float4( 0.0f, 0.5f, 0.0f, 1.0f );

	//calculate vector to light in R0
	float4 toLight = rpLocalLightOrigin - modelPosition;

	//textures 1 takes the base coordinates by the texture matrix
	result.texcoord1 = defaultTexCoord;
	result.texcoord1.x = dot4( vertex.texcoord.xy, rpBumpMatrixS );
	result.texcoord1.y = dot4( vertex.texcoord.xy, rpBumpMatrixT );

	//# texture 2 has one texgen
	result.texcoord2 = defaultTexCoord;
	result.texcoord2.x = dot4( modelPosition, rpLightFalloffS );

	//# texture 3 has three texgens
	result.texcoord3.x = dot4( modelPosition, rpLightProjectionS );
	result.texcoord3.y = dot4( modelPosition, rpLightProjectionT );
	result.texcoord3.z = 0.0f;
	result.texcoord3.w = dot4( modelPosition, rpLightProjectionQ );

	//# textures 4 takes the base coordinates by the texture matrix
	result.texcoord4 = defaultTexCoord;
	result.texcoord4.x = dot4( vertex.texcoord.xy, rpDiffuseMatrixS );
	result.texcoord4.y = dot4( vertex.texcoord.xy, rpDiffuseMatrixT );

	//# textures 5 takes the base coordinates by the texture matrix
	result.texcoord5 = defaultTexCoord;
	result.texcoord5.x = dot4( vertex.texcoord.xy, rpSpecularMatrixS );
	result.texcoord5.y = dot4( vertex.texcoord.xy, rpSpecularMatrixT );

	//# texture 6's texcoords will be the halfangle in texture space

	//# calculate normalized vector to light in R0
	toLight = normalize( toLight );

	//# calculate normalized vector to viewer in R1
	float4 toView = normalize( rpLocalViewOrigin - modelPosition );

	//# add together to become the half angle vector in object space (non-normalized)
	float4 halfAngleVector = toLight + toView;

	//# put into texture space
	result.texcoord6.x = dot3( tangent, halfAngleVector );
	result.texcoord6.y = dot3( binormal, halfAngleVector );
	result.texcoord6.z = dot3( normal, halfAngleVector );
	result.texcoord6.w = 1.0f;

	//# generate the vertex color, which can be 1.0, color, or 1.0 - color
	//# for 1.0 : env[16] = 0, env[17] = 1
	//# for color : env[16] = 1, env[17] = 0
	//# for 1.0-color : env[16] = -1, env[17] = 1
	result.color = ( swizzleColor( vertex.color ) * rpVertexColorModulate ) + rpVertexColorAdd;
}