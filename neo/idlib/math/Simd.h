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

#ifndef __MATH_SIMD_H__
#define __MATH_SIMD_H__

/*
===============================================================================

	Single Instruction Multiple Data (SIMD)

	For optimal use data should be aligned on a 16 byte boundary.
	All idSIMDProcessor routines are thread safe.

===============================================================================
*/

class idSIMD
{
public:
	static void			Init();
	static void			InitProcessor( const char* module, bool forceGeneric );
	static void			Shutdown();
	static void			Test_f( const class idCmdArgs& args );
};


/*
===============================================================================

	virtual base class for different SIMD processors

===============================================================================
*/

// RB begin
#ifdef _WIN32
	#define VPCALL __fastcall
#else
	#define VPCALL
#endif
// RB end

class idVec2;
class idVec3;
class idVec4;
class idVec5;
class idVec6;
class idVecX;
class idMat2;
class idMat3;
class idMat4;
class idMat5;
class idMat6;
class idMatX;
class idPlane;
class idDrawVert;
class idJointQuat;
class idJointMat;
struct dominantTri_t;

const int MIXBUFFER_SAMPLES = 4096;

typedef enum
{
	SPEAKER_LEFT = 0,
	SPEAKER_RIGHT,
	SPEAKER_CENTER,
	SPEAKER_LFE,
	SPEAKER_BACKLEFT,
	SPEAKER_BACKRIGHT
} speakerLabel;


class idSIMDProcessor
{
public:
	idSIMDProcessor()
	{
		cpuid = CPUID_NONE;
	}

	cpuid_t							cpuid;

	virtual ~idSIMDProcessor() {}                   // SRS - Added virtual destructor

	virtual const char* VPCALL		GetName() const = 0;

	virtual	void VPCALL MinMax( float& min,			float& max,				const float* src,		const int count ) = 0;
	virtual	void VPCALL MinMax( idVec2& min,		idVec2& max,			const idVec2* src,		const int count ) = 0;
	virtual	void VPCALL MinMax( idVec3& min,		idVec3& max,			const idVec3* src,		const int count ) = 0;
	virtual	void VPCALL MinMax( idVec3& min,		idVec3& max,			const idDrawVert* src,	const int count ) = 0;
	virtual	void VPCALL MinMax( idVec3& min,		idVec3& max,			const idDrawVert* src,	const triIndex_t* indexes,		const int count ) = 0;

	virtual void VPCALL Memcpy( void* dst,			const void* src,		const int count ) = 0;
	virtual void VPCALL Memset( void* dst,			const int val,			const int count ) = 0;

	// animation
	virtual void VPCALL BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints ) = 0;
	virtual void VPCALL BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints ) = 0;
	virtual void VPCALL ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints ) = 0;
	virtual void VPCALL ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints ) = 0;
	virtual void VPCALL TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint ) = 0;
	virtual void VPCALL UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint ) = 0;

	// sound mixing
	virtual void VPCALL UpSamplePCMTo44kHz( float* dest, const short* pcm, const int numSamples, const int kHz, const int numChannels ) = 0;
	virtual void VPCALL UpSampleOGGTo44kHz( float* dest, const float* const* ogg, const int numSamples, const int kHz, const int numChannels ) = 0;
	virtual void VPCALL MixSoundTwoSpeakerMono( float* mixBuffer, const float* samples, const int numSamples, const float lastV[2], const float currentV[2] ) = 0;
	virtual void VPCALL MixSoundTwoSpeakerStereo( float* mixBuffer, const float* samples, const int numSamples, const float lastV[2], const float currentV[2] ) = 0;
	virtual void VPCALL MixSoundSixSpeakerMono( float* mixBuffer, const float* samples, const int numSamples, const float lastV[6], const float currentV[6] ) = 0;
	virtual void VPCALL MixSoundSixSpeakerStereo( float* mixBuffer, const float* samples, const int numSamples, const float lastV[6], const float currentV[6] ) = 0;
	virtual void VPCALL MixedSoundToSamples( short* samples, const float* mixBuffer, const int numSamples ) = 0;
};

// pointer to SIMD processor
extern idSIMDProcessor* SIMDProcessor;

#endif /* !__MATH_SIMD_H__ */
