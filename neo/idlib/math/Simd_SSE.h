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

#ifndef __MATH_SIMD_SSE_H__
#define __MATH_SIMD_SSE_H__

/*
===============================================================================

	SSE implementation of idSIMDProcessor

===============================================================================
*/

#if defined(USE_INTRINSICS_SSE)

class idSIMD_SSE : public idSIMD_Generic
{
public:
	virtual const char* VPCALL GetName() const;

	virtual void VPCALL BlendJoints( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );
	virtual void VPCALL BlendJointsFast( idJointQuat* joints, const idJointQuat* blendJoints, const float lerp, const int* index, const int numJoints );
	virtual void VPCALL ConvertJointQuatsToJointMats( idJointMat* jointMats, const idJointQuat* jointQuats, const int numJoints );
	virtual void VPCALL ConvertJointMatsToJointQuats( idJointQuat* jointQuats, const idJointMat* jointMats, const int numJoints );
	virtual void VPCALL TransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
	virtual void VPCALL UntransformJoints( idJointMat* jointMats, const int* parents, const int firstJoint, const int lastJoint );
};

#endif

#endif /* !__MATH_SIMD_SSE_H__ */
