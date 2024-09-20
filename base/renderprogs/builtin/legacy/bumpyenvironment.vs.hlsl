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
	float4 color : COLOR0;
};

struct VS_OUT {
	float4 position		: POSITION;
	float2 texcoord0	: TEXCOORD0;
	float3 texcoord1	: TEXCOORD1;
	float3 texcoord2	: TEXCOORD2;
	float3 texcoord3	: TEXCOORD3;
	float3 texcoord4	: TEXCOORD4;
	float4 color		: COLOR0;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{

	float4 normal = vertex.normal * 2.0 - 1.0;
	float4 tangent = vertex.tangent * 2.0 - 1.0;
	float3 binormal = cross( normal.xyz, tangent.xyz ) * tangent.w;

	result.position.x = dot4( vertex.position, rpMVPmatrixX );
	result.position.y = dot4( vertex.position, rpMVPmatrixY );
	result.position.z = dot4( vertex.position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.position, rpMVPmatrixW );

	result.texcoord0 = vertex.texcoord.xy;

	float4 toEye = rpLocalViewOrigin - vertex.position;
	result.texcoord1.x = dot3( toEye, rpModelMatrixX );
	result.texcoord1.y = dot3( toEye, rpModelMatrixY );
	result.texcoord1.z = dot3( toEye, rpModelMatrixZ );

	result.texcoord2.x = dot3( tangent, rpModelMatrixX );
	result.texcoord3.x = dot3( tangent, rpModelMatrixY );
	result.texcoord4.x = dot3( tangent, rpModelMatrixZ );

	result.texcoord2.y = dot3( binormal, rpModelMatrixX );
	result.texcoord3.y = dot3( binormal, rpModelMatrixY );
	result.texcoord4.y = dot3( binormal, rpModelMatrixZ );

	result.texcoord2.z = dot3( normal, rpModelMatrixX );
	result.texcoord3.z = dot3( normal, rpModelMatrixY );
	result.texcoord4.z = dot3( normal, rpModelMatrixZ );

	result.color = rpColor;
}