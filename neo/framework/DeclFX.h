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

#ifndef __DECLFX_H__
#define __DECLFX_H__

/*
===============================================================================

	idDeclFX

===============================================================================
*/

enum
{
	FX_LIGHT,
	FX_PARTICLE,
	FX_DECAL,
	FX_MODEL,
	FX_SOUND,
	FX_SHAKE,
	FX_ATTACHLIGHT,
	FX_ATTACHENTITY,
	FX_LAUNCH,
	FX_SHOCKWAVE
};

//
// single fx structure
//
typedef struct
{
	int						type;
	int						sibling;

	idStr					data;
	idStr					name;
	idStr					fire;

	float					delay;
	float					duration;
	float					restart;
	float					size;
	float					fadeInTime;
	float					fadeOutTime;
	float					shakeTime;
	float					shakeAmplitude;
	float					shakeDistance;
	float					shakeImpulse;
	float					lightRadius;
	float					rotate;
	float					random1;
	float					random2;

	idVec3					lightColor;
	idVec3					offset;
	idMat3					axis;

	bool					soundStarted;
	bool					shakeStarted;
	bool					shakeFalloff;
	bool					shakeIgnoreMaster;
	bool					bindParticles;
	bool					explicitAxis;
	bool					noshadows;
	bool					particleTrackVelocity;
	bool					trackOrigin;
} idFXSingleAction;

//
// grouped fx structures
//
class idDeclFX : public idDecl
{
public:
	virtual size_t			Size() const;
	virtual const char* 	DefaultDefinition() const;
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion );
	virtual void			FreeData();
	virtual void			Print() const;
	virtual void			List() const;

	idList<idFXSingleAction>events;
	idStr					joint;

private:
	void					ParseSingleFXAction( idLexer& src, idFXSingleAction& FXAction );
};

#endif /* !__DECLFX_H__ */
