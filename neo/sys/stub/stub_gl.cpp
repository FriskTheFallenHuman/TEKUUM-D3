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

#include "../../renderer/tr_local.h"

void GLimp_WakeBackEnd( void* a ) {};
void GLimp_EnableLogging( bool ) {};
void GLimp_FrontEndSleep() {};
void GLimp_ActivateContext() {};
void GLimp_DeactivateContext() {};
bool GLimp_SpawnRenderThread( void ( *a )() )
{
	return false;
};

static void StubFunction() {};
GLExtension_t GLimp_ExtensionPointer( const char* a )
{
	return StubFunction;
};

bool GLimp_Init( glimpParms_t a )
{
	return true;
};
void GLimp_SetGamma( unsigned short* a, unsigned short* b, unsigned short* c ) {};
void GLimp_Shutdown() {};
void GLimp_SwapBuffers() {};
void* GLimp_BackEndSleep()
{
	return 0;
};
