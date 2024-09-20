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
};

struct VS_OUT {
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
};
// *INDENT-ON*

void main( VS_IN vertex, out VS_OUT result )
{
	result.position = vertex.position;
	result.texcoord0 = vertex.texcoord;
}