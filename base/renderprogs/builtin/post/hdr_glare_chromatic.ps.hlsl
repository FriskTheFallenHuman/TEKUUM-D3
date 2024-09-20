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
uniform sampler2D samp0		: register(s0);

struct PS_IN
{
	float4 position : VPOS;
	float2 texcoord0 : TEXCOORD0_centroid;
};

struct PS_OUT
{
	float4 color : COLOR;
};
// *INDENT-ON*

float linterp( float t )
{
	return saturate( 1.0 - abs( 2.0 * t - 1.0 ) );
}

float remap( float t, float a, float b )
{
	return saturate( ( t - a ) / ( b - a ) );
}

float3 spectrumoffset( float t )
{
	float lo = step( t, 0.5 );
	float hi = 1.0 - lo;
	float w = linterp( remap( t, 1.0 / 6.0, 5.0 / 6.0 ) );
	float3 ret = float3( lo, 1.0, hi ) * float3( 1.0 - w, w, 1.0 - w );

	return ret; // pow( ret, float3( 1.0 / 2.2 ) );
}

void main( PS_IN fragment, out PS_OUT result )
{
	float2 st = fragment.texcoord0;

	// base color with tone mapping applied
	float4 color = tex2D( samp0, st );

	const float gaussFact[9] = float[9]( 0.13298076, 0.12579441, 0.10648267, 0.08065691, 0.05467002, 0.03315905, 0.01799699, 0.00874063, 0.00379866 );

	const float3 chromaticOffsets[9] = float3[](
										   float3( 0.5, 0.5, 0.5 ), // w
										   float3( 0.8, 0.3, 0.3 ),
//	float3(1.0, 0.2, 0.2), // r
										   float3( 0.5, 0.2, 0.8 ),
										   float3( 0.2, 0.2, 1.0 ), // b
										   float3( 0.2, 0.3, 0.9 ),
										   float3( 0.2, 0.9, 0.2 ), // g
										   float3( 0.3, 0.5, 0.3 ),
										   float3( 0.3, 0.5, 0.3 ),
										   float3( 0.3, 0.5, 0.3 )
										   //float3(0.3, 0.5, 0.3)
									   );

	float3 sumColor = float3( 0.0 );
	float3 sumSpectrum = float3( 0.0 );

	const int tap = 4;
	const int samples = 9;

	float scale = 13.0; // bloom width
	const float weightScale = 2.3; // bloom strength

	for( int i = 0; i < samples; i++ )
	{
		//float t = ( ( float( 4 + ( i ) ) ) / ( float( samples ) - 1.0 ) );
		//float t = log2( float( i ) / ( float( samples ) - 1.0 ) );
		//float t = ( float( i ) / ( float( samples ) - 1.0 ) );

		//float3 so = spectrumoffset( t );
		float3 so = chromaticOffsets[ i ];
		float4 color = tex2D( samp0, st + float2( float( i ), 0 ) * rpWindowCoord.xy * scale );

		float weight = gaussFact[ i ];
		sumColor += color.rgb * ( so.rgb * weight * weightScale );
	}

#if 1
	for( int i = 1; i < samples; i++ )
	{
		//float t = ( ( float( 4 + ( i ) ) ) / ( float( samples ) - 1.0 ) );

		//float3 so = spectrumoffset( t );
		float3 so = chromaticOffsets[ i ];
		float4 color = tex2D( samp0, st + float2( float( -i ), 0 ) * rpWindowCoord.xy * scale );

		float weight = gaussFact[ i ];
		sumColor += color.rgb * ( so.rgb * weight * weightScale );
	}
#endif

	result.color = float4( sumColor, 1.0 );
	//result.color = float4( sumColor / float(samples), 1.0 );
	//result.color = float4( sumColor / sumSpectrum, 1.0 );
}
