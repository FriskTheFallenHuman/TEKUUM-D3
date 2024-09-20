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
uniform sampler2D samp3 : register(s3); // texture 4 is the light falloff texture
uniform sampler2D samp4 : register(s4); // texture 5 is the light projection texture

struct PS_IN {
	half4 position	: VPOS;
	half4 texcoord1	: TEXCOORD1_centroid;
	half4 texcoord2	: TEXCOORD2_centroid;
	half4 texcoord3	: TEXCOORD3_centroid;
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
	half4 lightFalloff =	idtex2Dproj( samp3, fragment.texcoord2 );
	half4 lightProj	=		idtex2Dproj( samp4, fragment.texcoord3 );
	half4 YCoCG =			tex2D( samp2, fragment.texcoord4.xy );
	half4 specMapSRGB =		tex2D( samp1, fragment.texcoord5.xy );
	half4 specMap =			sRGBAToLinearRGBA( specMapSRGB );

	const half3 ambientLightVector = half3( 0.5f, 9.5f - 0.385f, 0.8925f );
	half3 lightVector = normalize( ambientLightVector );
	half3 diffuseMap = sRGBToLinearRGB( ConvertYCoCgToRGB( YCoCG ) );

	half3 localNormal;
	// RB begin
#if defined(USE_NORMAL_FMT_RGB8)
	localNormal.xy = bumpMap.rg - 0.5;
#else
	localNormal.xy = bumpMap.wy - 0.5;
#endif
	// RB end
	localNormal.z = sqrt( abs( dot( localNormal.xy, localNormal.xy ) - 0.25 ) );
	localNormal = normalize( localNormal );

	// traditional very dark Lambert light model used in Doom 3
	half ldotN = saturate( dot3( localNormal, lightVector ) );

#if defined(USE_HALF_LAMBERT)
	// RB: http://developer.valvesoftware.com/wiki/Half_Lambert
	half halfLdotN = dot3( localNormal, lightVector ) * 0.5 + 0.5;
	halfLdotN *= halfLdotN;

	half lambert = halfLdotN;
#else
	half lambert = ldotN;
#endif

	const half specularPower = 10.0f;
	half hDotN = dot3( normalize( fragment.texcoord6.xyz ), localNormal );
	// RB: added abs
	half3 specularContribution = _half3( pow( abs( hDotN ), specularPower ) );

	half3 diffuseColor = diffuseMap * ( rpDiffuseModifier.xyz );
	half3 specularColor = specMap.xyz * specularContribution * ( rpSpecularModifier.xyz );
	half3 lightColor = sRGBToLinearRGB( lightProj.xyz * lightFalloff.xyz );

	result.color.xyz = ( diffuseColor + specularColor ) * lightColor * fragment.color.xyz;
	result.color.w = 1.0;
}
