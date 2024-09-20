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

#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

// This is for "official" HDMI 3D support with with the left eye above the right and a guard band in the middle
// Some displays which don't support this can still do stereo 3D by packing 2 eyes into a single (mono-sized) buffer
enum hdmi3DState_t
{
	HDMI3D_NOT_SUPPORTED,	// The TV doesn't support it
	HDMI3D_NOT_ENABLED,		// The TV supports it, but the user disabled it
	HDMI3D_NOT_ACTIVE,		// The TV supports it, and the user enabled it, but it's not active
	HDMI3D_ACTIVE
};


//================================================================================================
// class idRenderContext
//================================================================================================
class idRenderContext
{
public:
	idRenderContext() : depthHackValue( 0.0f ), weaponDepthHackValue( 1.0f ) {}
	virtual			~idRenderContext() {};

	void			InitContext();
	void			Shutdown();
	void			PrepareSwap();
	void			SwapBuffers( bool forceVsync = false );
	void			SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] );

	hdmi3DState_t	GetHDMI3DState();
	void			EnableHDMI3D( const bool enable );

	// Back End Rendering
	void			ExecuteBackendCommands( const emptyCommand_t* cmds );
	void			Clear( float r, float g, float b, float a );
	void			InitGraphicsAPIWrapper();

	// Debug Tools
	void			RenderDebugTools( drawSurf_t** drawSurfs, int numDrawSurfs );

	void			SetWrapperContext( const wrapperContext_t& context );
	void			SetWrapperConfig( const wrapperConfig_t& config );

	// Texture Resolves
	void			ResolveTargetColor( idImage* image );
	void			ResolveTargetDepth( idImage* image );
	void			ResolveTargetColor( idImage* image, int srcMinX, int srcMinY, int srcMaxX, int srcMaxY, int dstX, int dstY );
	void			ResolveTargetDepth( idImage* image, int srcMinX, int srcMinY, int srcMaxX, int srcMaxY, int dstX, int dstY );

	void			SetDepthHackValue( float depth )
	{
		depthHackValue = depth;
	}
	float			GetDepthHackValue() const
	{
		return depthHackValue;
	}
	void			SetWeaponDepthHackValue( float depth )
	{
		weaponDepthHackValue = depth;
	}
	float			GetWeaponDepthHackValue() const
	{
		return weaponDepthHackValue;
	}

	uint64_t			GetGPUFrameMicroSec() const
	{
		return GPUFrameMicroSec;
	}

private:
	float			depthHackValue;
	float			weaponDepthHackValue;
	uint64_t			GPUFrameMicroSec;
};

extern idRenderContext rRenderContext;

#endif	// !__RENDERCONTEXT_H__
