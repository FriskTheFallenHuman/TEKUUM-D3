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
	float2 texcoord0 : TEXCOORD0_centroid;
};

struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	// texcoords will run from 0 to 1 across the entire screen
	if( fract( fragment.position.y * 0.5 ) < 0.5 )
	{
		result.color = tex2D( samp0, vec2( fragment.texcoord0 ) );
	}
	else
	{
		result.color = tex2D( samp1, vec2( fragment.texcoord0 ) );
	}
}