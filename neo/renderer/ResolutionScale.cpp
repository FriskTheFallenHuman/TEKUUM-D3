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
#include "RenderCommon.h"
#include "ResolutionScale.h"


idResolutionScale	resolutionScale;

static const float MINIMUM_RESOLUTION_SCALE = 0.5f;
static const float MAXIMUM_RESOLUTION_SCALE = 1.0f;

// RB: turned this off. It is only useful on mobile devices or consoles
idCVar rs_enable( "rs_enable", "0", CVAR_INTEGER, "Enable dynamic resolution scaling, 0 - off, 1 - horz only, 2 - vert only, 3 - both" );
// Rb end
idCVar rs_forceFractionX( "rs_forceFractionX", "0", CVAR_FLOAT, "Force a specific 0.0 to 1.0 horizontal resolution scale" );
idCVar rs_forceFractionY( "rs_forceFractionY", "0", CVAR_FLOAT, "Force a specific 0.0 to 1.0 vertical resolution scale" );
idCVar rs_showResolutionChanges( "rs_showResolutionChanges", "0", CVAR_INTEGER, "1 = Print whenever the resolution scale changes, 2 = always" );
idCVar rs_dropMilliseconds( "rs_dropMilliseconds", "15.0", CVAR_FLOAT, "Drop the resolution when GPU time exceeds this" );
idCVar rs_raiseMilliseconds( "rs_raiseMilliseconds", "13.0", CVAR_FLOAT, "Raise the resolution when GPU time is below this for several frames" );
idCVar rs_dropFraction( "rs_dropFraction", "0.11", CVAR_FLOAT, "Drop the resolution in increments of this" );
idCVar rs_raiseFraction( "rs_raiseFraction", "0.06", CVAR_FLOAT, "Raise the resolution in increments of this" );
idCVar rs_raiseFrames( "rs_raiseFrames", "5", CVAR_INTEGER, "Require this many frames below rs_raiseMilliseconds" );
idCVar rs_display( "rs_display", "0", CVAR_INTEGER, "0 - percentages, 1 - pixels per frame" );


/*
========================
idResolutionScale::idResolutionScale
========================
*/
idResolutionScale::idResolutionScale()
{
	dropMilliseconds = 15.0f;
	raiseMilliseconds = 13.0f;
	framesAboveRaise = 0;
	currentResolution = 1.0f;
}

/*
========================
idResolutionScale::InitForMap
========================
*/
void idResolutionScale::InitForMap( const char* mapName )
{
	dropMilliseconds = rs_dropMilliseconds.GetFloat();
	raiseMilliseconds = rs_raiseMilliseconds.GetFloat();
}

/*
========================
idResolutionScale::ResetToFullResolution
========================
*/
void idResolutionScale::ResetToFullResolution()
{
	currentResolution = 1.0f;
}

/*
========================
idResolutionScale::GetCurrentResolutionScale
========================
*/
void idResolutionScale::GetCurrentResolutionScale( float& x, float& y )
{
	assert( currentResolution >= MINIMUM_RESOLUTION_SCALE );
	assert( currentResolution <= MAXIMUM_RESOLUTION_SCALE );

	x = MAXIMUM_RESOLUTION_SCALE;
	y = MAXIMUM_RESOLUTION_SCALE;
	switch( rs_enable.GetInteger() )
	{
		case 0:
			return;
		case 1:
			x = currentResolution;
			break;
		case 2:
			y = currentResolution;
			break;
		case 3:
		{
			const float middle = ( MINIMUM_RESOLUTION_SCALE + MAXIMUM_RESOLUTION_SCALE ) * 0.5f;
			if( currentResolution >= middle )
			{
				// First scale horizontally from max to min
				x = MINIMUM_RESOLUTION_SCALE + ( currentResolution - middle ) * 2.0f;
			}
			else
			{
				// Then scale vertically from max to min
				x = MINIMUM_RESOLUTION_SCALE;
				y = MINIMUM_RESOLUTION_SCALE + ( currentResolution - MINIMUM_RESOLUTION_SCALE ) * 2.0f;
			}
			break;
		}
	}
	float forceFrac = rs_forceFractionX.GetFloat();
	if( forceFrac > 0.0f && forceFrac <= MAXIMUM_RESOLUTION_SCALE )
	{
		x = forceFrac;
	}
	forceFrac = rs_forceFractionY.GetFloat();
	if( forceFrac > 0.0f && forceFrac <= MAXIMUM_RESOLUTION_SCALE )
	{
		y = forceFrac;
	}
}

/*
========================
idResolutionScale::SetCurrentGPUFrameTime
========================
*/
void idResolutionScale::SetCurrentGPUFrameTime( int microseconds )
{
	float old = currentResolution;
	float milliseconds = microseconds * 0.001f;

	if( milliseconds > dropMilliseconds )
	{
		// We missed our target, so drop the resolution.
		// The target should be set conservatively so this does not
		// necessarily imply a missed VBL.
		//
		// we might consider making the drop in some way
		// proportional to how badly we missed
		currentResolution -= rs_dropFraction.GetFloat();
		if( currentResolution < MINIMUM_RESOLUTION_SCALE )
		{
			currentResolution = MINIMUM_RESOLUTION_SCALE;
		}
	}
	else if( milliseconds < raiseMilliseconds )
	{
		// We seem to have speed to spare, so increase the resolution
		// if we stay here consistantly.  The raise fraction should
		// be smaller than the drop fraction to avoid ping-ponging
		// back and forth.
		if( ++framesAboveRaise >= rs_raiseFrames.GetInteger() )
		{
			framesAboveRaise = 0;
			currentResolution += rs_raiseFraction.GetFloat();
			if( currentResolution > MAXIMUM_RESOLUTION_SCALE )
			{
				currentResolution = MAXIMUM_RESOLUTION_SCALE;
			}
		}
	}
	else
	{
		// we are inside the target range
		framesAboveRaise = 0;
	}

	if( rs_showResolutionChanges.GetInteger() > 1 ||
			( rs_showResolutionChanges.GetInteger() == 1 && currentResolution != old ) )
	{
		idLib::Printf( "GPU msec: %4.1f resolutionScale: %4.2f\n", milliseconds, currentResolution );
	}
}

/*
========================
idResolutionScale::GetConsoleText
========================
*/
void idResolutionScale::GetConsoleText( idStr& s )
{
	float x;
	float y;
	if( rs_enable.GetInteger() == 0 )
	{
		s = "rs-off";
		return;
	}
	GetCurrentResolutionScale( x, y );
	if( rs_display.GetInteger() > 0 )
	{
		x *= 1280.0f;
		y *= 720.0f;
		if( rs_enable.GetInteger() == 1 )
		{
			y = 1.0f;
		}
		else if( rs_enable.GetInteger() == 2 )
		{
			x = 1.0f;
		}
		s = va( "rs-pixels %i", idMath::Ftoi( x * y ) );
	}
	else
	{
		if( rs_enable.GetInteger() == 3 )
		{
			s = va( "%2i%%h,%2i%%v", idMath::Ftoi( 100.0f * x ), idMath::Ftoi( 100.0f * y ) );
		}
		else
		{
			s = va( "%2i%%%s", ( rs_enable.GetInteger() == 1 ) ? idMath::Ftoi( 100.0f * x ) : idMath::Ftoi( 100.0f * y ), ( rs_enable.GetInteger() == 1 ) ? "h" : "v" );
		}
	}
}
