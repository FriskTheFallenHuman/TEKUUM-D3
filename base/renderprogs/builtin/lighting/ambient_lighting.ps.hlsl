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
uniform sampler2D samp0 : register(s0); // texture 1 is the per-surface normal map
uniform sampler2D samp1 : register(s1); // texture 3 is the per-surface specular or roughness/metallic/AO mixer map
uniform sampler2D samp2 : register(s2); // texture 2 is the per-surface baseColor map 
uniform sampler2D samp3 : register(s3); // texture 3 is the BRDF LUT
uniform sampler2D samp4 : register(s4); // texture 4 is SSAO

struct PS_IN {
	half4 position	: VPOS;
	half4 texcoord0	: TEXCOORD0_centroid;
	half4 texcoord1	: TEXCOORD1_centroid;
//	half4 texcoord2	: TEXCOORD2_centroid;
//	half4 texcoord3	: TEXCOORD3_centroid;
	half4 texcoord4	: TEXCOORD4_centroid;
	half4 texcoord5	: TEXCOORD5_centroid;
	half4 texcoord6	: TEXCOORD6_centroid;
	half4 color		: COLOR0;
};

struct PS_OUT {
	half4 color : COLOR;
};
// *INDENT-ON*

void main( PS_IN fragment, out PS_OUT result )
{
	half4 bumpMap =			tex2D( samp0, fragment.texcoord1.xy );
//	half4 lightFalloff =	idtex2Dproj( samp1, fragment.texcoord2 );
//	half4 lightProj	=		idtex2Dproj( samp2, fragment.texcoord3 );
	half4 YCoCG =			tex2D( samp2, fragment.texcoord4.xy );
	half4 specMap =			sRGBAToLinearRGBA( tex2D( samp1, fragment.texcoord5.xy ) );

	half3 lightVector = normalize( fragment.texcoord0.xyz );
	half3 diffuseMap = sRGBToLinearRGB( ConvertYCoCgToRGB( YCoCG ) );

	half3 localNormal;
#if defined(USE_NORMAL_FMT_RGB8)
	localNormal.xy = bumpMap.rg - 0.5;
#else
	localNormal.xy = bumpMap.wy - 0.5;
#endif
	localNormal.z = sqrt( abs( dot( localNormal.xy, localNormal.xy ) - 0.25 ) );
	localNormal = normalize( localNormal );

	const half specularPower = 10.0f;
	half hDotN = dot3( normalize( fragment.texcoord6.xyz ), localNormal );
	// RB: added abs
	half3 specularContribution = _half3( pow( abs( hDotN ), specularPower ) );

	half3 diffuseColor = diffuseMap * ( rpDiffuseModifier.xyz * 0.5f );
	half3 specularColor = specMap.xyz * specularContribution * ( rpSpecularModifier.xyz );

	float halfLdotN = 1.0f;
#if defined(USE_HALF_LAMBERT)
	// RB: http://developer.valvesoftware.com/wiki/Half_Lambert
	halfLdotN = dot3( localNormal, lightVector ) * 0.5 + 0.5;
	halfLdotN *= halfLdotN;
#endif

	// traditional very dark Lambert light model used in Doom 3
	float ldotN = dot3( localNormal, lightVector );

	half3 lightColor = sRGBToLinearRGB( rpAmbientColor.rgb );

	half rim =  1.0f - saturate( hDotN );
	half rimPower = 8.0;
	half3 rimColor = sRGBToLinearRGB( half3( 0.125 ) * 1.2 ) * lightColor * pow( rim, rimPower );

	//result.color.rgb = localNormal.xyz * 0.5 + 0.5;
	result.color.xyz = ( ( diffuseColor + specularColor ) * halfLdotN * lightColor + rimColor ) * fragment.color.rgb;
	//result.color = ( ( diffuseColor + specularColor ) * halfLdotN * lightColor + rimColor ) * fragment.color.rgba;
	result.color.w = fragment.color.a;
}
