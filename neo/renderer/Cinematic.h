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

#ifndef __CINEMATIC_H__
#define __CINEMATIC_H__

/*
===============================================================================

	cinematic

	Multiple idCinematics can run simultaniously.
	A single idCinematic can be reused for multiple files if desired.

===============================================================================
*/

// cinematic states
typedef enum
{
	FMV_IDLE,
	FMV_PLAY,			// play
	FMV_EOF,			// all other conditions, i.e. stop/EOF/abort
	FMV_ID_BLT,
	FMV_ID_IDLE,
	FMV_LOOPED,
	FMV_ID_WAIT
} cinStatus_t;

class idImage;

// a cinematic stream generates an image buffer, which the caller will upload to a texture
typedef struct
{
	int					imageWidth;
	int					imageHeight;	// will be a power of 2
	idImage*			imageY;
	idImage*			imageCr;
	idImage*			imageCb;
	idImage*			image;
	int					status;
} cinData_t;

class idCinematic
{
public:
	// initialize cinematic play back data
	static void			InitCinematic();

	// shutdown cinematic play back data
	static void			ShutdownCinematic();

	// allocates and returns a private subclass that implements the methods
	// This should be used instead of new
	static idCinematic*	Alloc();

	// frees all allocated memory
	virtual				~idCinematic();

	// returns false if it failed to load
	virtual bool		InitFromFile( const char* qpath, bool looping );

	// returns the length of the animation in milliseconds
	virtual int			AnimationLength();

	// RB: let us know wether this video went EOF or is still active
	virtual bool        IsPlaying() const;
	// RB end

	// the pointers in cinData_t will remain valid until the next UpdateForTime() call
	virtual cinData_t	ImageForTime( int milliseconds );

	// closes the file and frees all allocated memory
	virtual void		Close();

	// sets the cinematic to start at that time (can be in the past)
	virtual void		ResetTime( int time );

	// gets the time the cinematic started
	virtual int			GetStartTime();

	virtual void		ExportToTGA( bool skipExisting = true );

	virtual float		GetFrameRate() const;
};

/*
===============================================

	Sound meter.

===============================================
*/

class idSndWindow : public idCinematic
{
public:

	idSndWindow()
	{
		showWaveform = false;
	}
	~idSndWindow() {}

	bool				InitFromFile( const char* qpath, bool looping );
	cinData_t			ImageForTime( int milliseconds );
	int					AnimationLength();

private:
	bool				showWaveform;
};

#endif /* !__CINEMATIC_H__ */
