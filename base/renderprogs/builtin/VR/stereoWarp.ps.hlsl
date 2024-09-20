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


/*

This shader will cover a square block of pixel coordinates, but some of them might
be scissored off if the edges of the screen or the midpoint divider are visible through
the optics.

*/

// *INDENT-OFF*
uniform sampler2D samp0 : register(s0);

struct PS_IN {
	vec4 texcoord0	: TEXCOORD0_centroid;
};


struct PS_OUT {
	float4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	const float screenWarp_range   = 1.45;

	const vec2    warpCenter = vec2( 0.5, 0.5 );
	vec2    centeredTexcoord = fragment.texcoord0.xy - warpCenter;

	float	radialLength = length( centeredTexcoord );
	vec2	radialDir = normalize( centeredTexcoord );

	// get it down into the 0 - PI/2 range
	float	range = screenWarp_range;
	float	scaledRadialLength = radialLength * range;
	float	tanScaled = tan( scaledRadialLength );

	float   rescaleValue = tan( 0.5 * range );

	// If radialLength was 0.5, we want rescaled to also come out
	// as 0.5, so the edges of the rendered image are at the edges
	// of the warped image.
	float	rescaled = tanScaled / rescaleValue;

	vec2	warped = warpCenter + vec2( 0.5, 0.5 ) * radialDir * rescaled;

	result.color = tex2D( samp0, warped );
}
