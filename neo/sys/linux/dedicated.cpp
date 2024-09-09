/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#include "../../renderer/tr_local.h"
#include "../posix/posix_public.h"
#include "local.h"

/*
==========
input
==========
*/

void Sys_InitInput() { }

void Sys_ShutdownInput() { }

void Sys_GrabMouseCursor( bool ) { }

int Sys_PollMouseInputEvents()
{
	return 0;
}

void Sys_EndMouseInputEvents() { }

int Sys_ReturnMouseInputEvent( const int n, int& action, int& value )
{
	return 0;
}

int Sys_PollKeyboardInputEvents()
{
	return 0;
}

void Sys_EndKeyboardInputEvents() { }

int Sys_ReturnKeyboardInputEvent( const int n, int& action, bool& state )
{
	return 0;
}

unsigned char Sys_MapCharForKey( int key )
{
	return ( unsigned char )key;
}

/*
==========
GL
==========
*/

void GLimp_EnableLogging( bool enable ) { }

bool GLimp_Init( glimpParms_t a )
{
	return true;
}

void GLimp_SetGamma( unsigned short red[256],
					 unsigned short green[256],
					 unsigned short blue[256] ) { }

void GLimp_Shutdown() { }

void GLimp_SwapBuffers() { }

void GLimp_DeactivateContext() { }

void GLimp_ActivateContext() { }

bool GLimp_SetScreenParms( glimpParms_t parms )
{
	return true;
}

