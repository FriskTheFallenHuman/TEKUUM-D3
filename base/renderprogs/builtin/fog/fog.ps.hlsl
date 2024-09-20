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
uniform sampler2D samp0 : register(s0);
uniform sampler2D samp1 : register(s1);

struct PS_IN {
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
	float2 texcoord1 : TEXCOORD1_centroid;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	float4 c = tex2D( samp0, fragment.texcoord0 ) * tex2D( samp1, fragment.texcoord1 ) * rpColor;

#if defined( USE_LINEAR_RGB )
	c = clamp( c, 0.0, 1.0 );

	c = float4( Linear1( c.r ), Linear1( c.g ), Linear1( c.b ), Linear1( c.a ) );
#endif

	result.color = c;
}

