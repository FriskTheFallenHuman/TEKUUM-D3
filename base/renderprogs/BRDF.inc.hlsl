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

// Normal Distribution Function ( NDF ) or D( h )
// GGX ( Trowbridge-Reitz )
half Distribution_GGX( half hdotN, half alpha )
{
	// alpha is assumed to be roughness^2
	float a2 = alpha * alpha;
	//float tmp = ( hdotN * hdotN ) * ( a2 - 1.0 ) + 1.0;
	float tmp = ( hdotN * a2 - hdotN ) * hdotN + 1.0;

	return ( a2 / ( PI * tmp * tmp ) );
}

half Distribution_GGX_Disney( half hdotN, half alphaG )
{
	float a2 = alphaG * alphaG;
	float tmp = ( hdotN * hdotN ) * ( a2 - 1.0 ) + 1.0;
	//tmp *= tmp;

	return ( a2 / ( PI * tmp ) );
}

half Distribution_GGX_1886( half hdotN, half alpha )
{
	// alpha is assumed to be roughness^2
	return ( alpha / ( PI * pow( hdotN * hdotN * ( alpha - 1.0 ) + 1.0, 2.0 ) ) );
}

// Fresnel term F( v, h )
// Fnone( v, h ) = F(0°) = specularColor
half3 Fresnel_Schlick( half3 specularColor, half vDotN )
{
	return specularColor + ( 1.0 - specularColor ) * pow( 1.0 - vDotN, 5.0 );
}

// Fresnel term that takes roughness into account so rough non-metal surfaces aren't too shiny [Lagarde11]
half3 Fresnel_SchlickRoughness( half3 specularColor, half vDotN, half roughness )
{
	return specularColor + ( max( half3( 1.0  - roughness ), specularColor ) - specularColor ) * pow( 1.0 - vDotN, 5.0 );
}

// Sebastien Lagarde proposes an empirical approach to derive the specular occlusion term from the diffuse occlusion term in [Lagarde14].
// The result does not have any physical basis but produces visually pleasant results.
// See Sebastien Lagarde and Charles de Rousiers. 2014. Moving Frostbite to PBR.
float ComputeSpecularAO( float vDotN, float ao, float roughness )
{
	return clamp( pow( vDotN + ao, exp2( -16.0 * roughness - 1.0 ) ) - 1.0 + ao, 0.0, 1.0 );
}

// Visibility term G( l, v, h )
// Very similar to Marmoset Toolbag 2 and gives almost the same results as Smith GGX
float Visibility_Schlick( half vdotN, half ldotN, float alpha )
{
	float k = alpha * 0.5;

	float schlickL = ( ldotN * ( 1.0 - k ) + k );
	float schlickV = ( vdotN * ( 1.0 - k ) + k );

	return ( 0.25 / ( schlickL * schlickV ) );
	//return ( ( schlickL * schlickV ) / ( 4.0 * vdotN * ldotN ) );
}

// see s2013_pbs_rad_notes.pdf
// Crafting a Next-Gen Material Pipeline for The Order: 1886
// this visibility function also provides some sort of back lighting
float Visibility_SmithGGX( half vdotN, half ldotN, float alpha )
{
	// alpha is already roughness^2

	float V1 = ldotN + sqrt( alpha + ( 1.0 - alpha ) * ldotN * ldotN );
	float V2 = vdotN + sqrt( alpha + ( 1.0 - alpha ) * vdotN * vdotN );

	// RB: avoid too bright spots
	return ( 1.0 / max( V1 * V2, 0.15 ) );
}

// HACK calculate roughness from D3 gloss maps
float EstimateLegacyRoughness( float3 specMapSRGB )
{
	float Y = dot( LUMINANCE_SRGB.rgb, specMapSRGB );

	//float glossiness = clamp( 1.0 - specMapSRGB.r, 0.0, 0.98 );
	float glossiness = clamp( pow( Y, 1.0 / 2.0 ), 0.0, 0.98 );

	float roughness = 1.0 - glossiness;

	return roughness;
}

// Environment BRDF approximations
// see s2013_pbs_black_ops_2_notes.pdf
/*
half a1vf( half g )
{
	return ( 0.25 * g + 0.75 );
}

half a004( half g, half vdotN )
{
	float t = min( 0.475 * g, exp2( -9.28 * vdotN ) );
	return ( t + 0.0275 ) * g + 0.015;
}

half a0r( half g, half vdotN )
{
	return ( ( a004( g, vdotN ) - a1vf( g ) * 0.04 ) / 0.96 );
}

float3 EnvironmentBRDF( half g, half vdotN, float3 rf0 )
{
	float4 t = float4( 1.0 / 0.96, 0.475, ( 0.0275 - 0.25 * 0.04 ) / 0.96, 0.25 );
	t *= float4( g, g, g, g );
	t += float4( 0.0, 0.0, ( 0.015 - 0.75 * 0.04 ) / 0.96, 0.75 );
	half a0 = t.x * min( t.y, exp2( -9.28 * vdotN ) ) + t.z;
	half a1 = t.w;

	return saturate( a0 + rf0 * ( a1 - a0 ) );
}


half3 EnvironmentBRDFApprox( half roughness, half vdotN, half3 specularColor )
{
	const half4 c0 = half4( -1, -0.0275, -0.572, 0.022 );
	const half4 c1 = half4( 1, 0.0425, 1.04, -0.04 );

	half4 r = roughness * c0 + c1;
	half a004 = min( r.x * r.x, exp2( -9.28 * vdotN ) ) * r.x + r.y;
	half2 AB = half2( -1.04, 1.04 ) * a004 + r.zw;

	return specularColor * AB.x + AB.y;
}
*/



