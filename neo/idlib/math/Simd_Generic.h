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

#ifndef __MATH_SIMD_GENERIC_H__
#define __MATH_SIMD_GENERIC_H__

/*
===============================================================================

	Generic implementation of idSIMDProcessor

===============================================================================
*/

class idSIMD_Generic : public idSIMDProcessor
{
public:
	virtual const char* VPCALL GetName() const;

	virtual void VPCALL MinMax( float& min,			float& max,				const float* src,		const int count );
	virtual	void VPCALL MinMax( idVec2& min,		idVec2& max,			const idVec2* src,		const int count );
	virtual void VPCALL MinMax( idVec3& min,		idVec3& max,			const idVec3* src,		const int count );
	virtual	void VPCALL MinMax( idVec3& min,		idVec3& max,			const idDrawVert* src,	const int count );
	virtual	void VPCALL MinMax( idVec3& min,		idVec3& max,			const idDrawVert* src,	const triIndex_t* indexes,		const int count );

	virtual void VPCALL Memcpy( void* dst,			const void* src,		const int count );
	virtual void VPCALL Memset( void* dst,			const int val,			const int count );

	virtual void VPCALL BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );
	virtual void VPCALL BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );
	virtual void VPCALL ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );
	virtual void VPCALL ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );
	virtual void VPCALL TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
	virtual void VPCALL UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );

	virtual void VPCALL UpSamplePCMTo44kHz( float* dest, const short* pcm, const int numSamples, const int kHz, const int numChannels );
	virtual void VPCALL UpSampleOGGTo44kHz( float* dest, const float* const* ogg, const int numSamples, const int kHz, const int numChannels );
	virtual void VPCALL MixSoundTwoSpeakerMono( float* mixBuffer, const float* samples, const int numSamples, const float lastV[2], const float currentV[2] );
	virtual void VPCALL MixSoundTwoSpeakerStereo( float* mixBuffer, const float* samples, const int numSamples, const float lastV[2], const float currentV[2] );
	virtual void VPCALL MixSoundSixSpeakerMono( float* mixBuffer, const float* samples, const int numSamples, const float lastV[6], const float currentV[6] );
	virtual void VPCALL MixSoundSixSpeakerStereo( float* mixBuffer, const float* samples, const int numSamples, const float lastV[6], const float currentV[6] );
	virtual void VPCALL MixedSoundToSamples( short* samples, const float* mixBuffer, const int numSamples );
};

#endif /* !__MATH_SIMD_GENERIC_H__ */
