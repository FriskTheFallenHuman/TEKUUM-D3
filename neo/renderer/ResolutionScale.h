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
#ifndef __RESOLUTIONSCALE_H__
#define __RESOLUTIONSCALE_H__

class idResolutionScale
{
public:
	idResolutionScale();

	void	InitForMap( const char* mapName );

	// Returns a float from 0.5 to 1.0, representing
	// the estimated resolution downscale needed to
	// maintain the target framerate.
	void	GetCurrentResolutionScale( float& x, float& y );

	// This should be called after any discontinuous
	// view movement or force texture loading to prevent
	// the unusual frames from causing an excessively
	// low dynamic resolution.
	void	ResetToFullResolution();

	// Systems that don't have accurate GPU timing can pass 0
	// to this, which will effectively disable resolution scaling.
	void	SetCurrentGPUFrameTime( int microseconds );

	// return console display text
	void	GetConsoleText( idStr& s );

private:
	float			dropMilliseconds;
	float			raiseMilliseconds;
	int				framesAboveRaise;
	float			currentResolution;
};

extern idResolutionScale resolutionScale;

#endif // __RESOLUTIONSCALE_H__
