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

#include "precompiled.h"
#pragma hdrstop

const int SMALLEST_NON_DENORMAL					= 1 << IEEE_FLT_MANTISSA_BITS;
const int NAN_VALUE								= 0x7f800000;

const float	idMath::PI				= 3.14159265358979323846f;
const float	idMath::TWO_PI			= 2.0f * PI;
const float	idMath::HALF_PI			= 0.5f * PI;
const float	idMath::ONEFOURTH_PI	= 0.25f * PI;
const float idMath::ONEOVER_PI		= 1.0f / idMath::PI;
const float idMath::ONEOVER_TWOPI	= 1.0f / idMath::TWO_PI;
const float idMath::E				= 2.71828182845904523536f;
const float idMath::SQRT_TWO		= 1.41421356237309504880f;
const float idMath::SQRT_THREE		= 1.73205080756887729352f;
const float	idMath::SQRT_1OVER2		= 0.70710678118654752440f;
const float	idMath::SQRT_1OVER3		= 0.57735026918962576450f;
const float	idMath::M_DEG2RAD		= PI / 180.0f;
const float	idMath::M_RAD2DEG		= 180.0f / PI;
const float	idMath::M_SEC2MS		= 1000.0f;
const float	idMath::M_MS2SEC		= 0.001f;
const float	idMath::INFINITUM		= 1e30f;
const float idMath::FLOAT_EPSILON		= 1.192092896e-07f;
const float idMath::FLT_SMALLEST_NON_DENORMAL	= * reinterpret_cast< const float* >( & SMALLEST_NON_DENORMAL );	// 1.1754944e-038f

#if defined(USE_INTRINSICS_SSE)
const __m128 idMath::SIMD_SP_zero				= { 0.0f, 0.0f, 0.0f, 0.0f };
const __m128 idMath::SIMD_SP_255				= { 255.0f, 255.0f, 255.0f, 255.0f };
const __m128 idMath::SIMD_SP_min_char			= { -128.0f, -128.0f, -128.0f, -128.0f };
const __m128 idMath::SIMD_SP_max_char			= { 127.0f, 127.0f, 127.0f, 127.0f };
const __m128 idMath::SIMD_SP_min_short			= { -32768.0f, -32768.0f, -32768.0f, -32768.0f };
const __m128 idMath::SIMD_SP_max_short			= { 32767.0f, 32767.0f, 32767.0f, 32767.0f };
const __m128 idMath::SIMD_SP_smallestNonDenorm	= { FLT_SMALLEST_NON_DENORMAL, FLT_SMALLEST_NON_DENORMAL, FLT_SMALLEST_NON_DENORMAL, FLT_SMALLEST_NON_DENORMAL };
const __m128 idMath::SIMD_SP_tiny				= { 1e-4f, 1e-4f, 1e-4f, 1e-4f };
const __m128 idMath::SIMD_SP_rsqrt_c0			= { 3.0f, 3.0f, 3.0f, 3.0f };
const __m128 idMath::SIMD_SP_rsqrt_c1			= { -0.5f, -0.5f, -0.5f, -0.5f };
#endif

bool		idMath::initialized		= false;
dword		idMath::iSqrt[SQRT_TABLE_SIZE];		// inverse square root lookup table

/*
===============
idMath::Init
===============
*/
void idMath::Init()
{
	union _flint fi, fo;

	for( int i = 0; i < SQRT_TABLE_SIZE; i++ )
	{
		fi.i	 = ( ( EXP_BIAS - 1 ) << EXP_POS ) | ( i << LOOKUP_POS );
		fo.f	 = ( float )( 1.0 / sqrt( fi.f ) );
		iSqrt[i] = ( ( dword )( ( ( fo.i + ( 1 << ( SEED_POS - 2 ) ) ) >> SEED_POS ) & 0xFF ) ) << SEED_POS;
	}

	iSqrt[SQRT_TABLE_SIZE / 2] = ( ( dword )( 0xFF ) ) << ( SEED_POS );

	initialized = true;
}

/*
================
idMath::FloatToBits
================
*/
int idMath::FloatToBits( float f, int exponentBits, int mantissaBits )
{
	int i, sign, exponent, mantissa, value;

	assert( exponentBits >= 2 && exponentBits <= 8 );
	assert( mantissaBits >= 2 && mantissaBits <= 23 );

	int maxBits = ( ( ( 1 << ( exponentBits - 1 ) ) - 1 ) << mantissaBits ) | ( ( 1 << mantissaBits ) - 1 );
	int minBits = ( ( ( 1 <<   exponentBits ) - 2 ) << mantissaBits ) | 1;

	float max = BitsToFloat( maxBits, exponentBits, mantissaBits );
	float min = BitsToFloat( minBits, exponentBits, mantissaBits );

	if( f >= 0.0f )
	{
		if( f >= max )
		{
			return maxBits;
		}
		else if( f <= min )
		{
			return minBits;
		}
	}
	else
	{
		if( f <= -max )
		{
			return ( maxBits | ( 1 << ( exponentBits + mantissaBits ) ) );
		}
		else if( f >= -min )
		{
			return ( minBits | ( 1 << ( exponentBits + mantissaBits ) ) );
		}
	}

	exponentBits--;
	i = *reinterpret_cast<int*>( &f );
	sign = ( i >> IEEE_FLT_SIGN_BIT ) & 1;
	exponent = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	mantissa = i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 );
	value = sign << ( 1 + exponentBits + mantissaBits );
	value |= ( ( INT32_SIGNBITSET( exponent ) << exponentBits ) | ( abs( exponent ) & ( ( 1 << exponentBits ) - 1 ) ) ) << mantissaBits;
	value |= mantissa >> ( IEEE_FLT_MANTISSA_BITS - mantissaBits );
	return value;
}

/*
================
idMath::BitsToFloat
================
*/
float idMath::BitsToFloat( int i, int exponentBits, int mantissaBits )
{
	static int exponentSign[2] = { 1, -1 };
	int sign, exponent, mantissa, value;

	assert( exponentBits >= 2 && exponentBits <= 8 );
	assert( mantissaBits >= 2 && mantissaBits <= 23 );

	exponentBits--;
	sign = i >> ( 1 + exponentBits + mantissaBits );
	exponent = ( ( i >> mantissaBits ) & ( ( 1 << exponentBits ) - 1 ) ) * exponentSign[( i >> ( exponentBits + mantissaBits ) ) & 1];
	mantissa = ( i & ( ( 1 << mantissaBits ) - 1 ) ) << ( IEEE_FLT_MANTISSA_BITS - mantissaBits );
	value = sign << IEEE_FLT_SIGN_BIT | ( exponent + IEEE_FLT_EXPONENT_BIAS ) << IEEE_FLT_MANTISSA_BITS | mantissa;
	return *reinterpret_cast<float*>( &value );
}
