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

#include "../../renderer/RenderCommon.h"
#include "../../sys/win32/win_local.h"
#include <io.h>

#include "../../sys/win32/rc/resource.h"

#include "GEApp.h"

rvGEApp		gApp;

/*
================
GUIEditorInit

Start the gui editor
================
*/
void GUIEditorInit()
{
	InitAfx();

	gApp.Initialize();
}

/*
================
GUIEditorShutdown
================
*/
void GUIEditorShutdown()
{
}

/*
================
GUIEditorHandleMessage

Handle translator messages
================
*/
bool GUIEditorHandleMessage( void* msg )
{
	if( !gApp.IsActive( ) )
	{
		return false;
	}

	return gApp.TranslateAccelerator( reinterpret_cast<LPMSG>( msg ) );
}

/*
================
GUIEditorRun

Run a frame
================
*/
static int sysMsgTime = 0; // DG: only used by GUIEditorRun(); no reason to put this into Win32Vars_t
void GUIEditorRun()
{
	MSG			msg;

	// pump the message loop
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( !GetMessage( &msg, NULL, 0, 0 ) )
		{
			common->Quit();
		}

		// save the msg time, because wndprocs don't have access to the timestamp
		if( sysMsgTime && sysMsgTime > ( int )msg.time )
		{
		}
		else
		{
			sysMsgTime = msg.time;
		}

		if( gApp.TranslateAccelerator( &msg ) )
		{
			continue;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	gApp.RunFrame( );

	// The GUI editor runs too hot so we need to slow it down a bit.
	Sleep( 1 );
}

/*
================
StringFromVec4

Returns a clean string version of the given vec4
================
*/
const char* StringFromVec4( idVec4& v )
{
	return va( "%s,%s,%s,%s",
			   idStr::FloatArrayToString( &v[0], 1, 8 ),
			   idStr::FloatArrayToString( &v[1], 1, 8 ),
			   idStr::FloatArrayToString( &v[2], 1, 8 ),
			   idStr::FloatArrayToString( &v[3], 1, 8 ) );
}

/*
================
IsExpression

Returns true if the given string is an expression
================
*/
bool IsExpression( const char* s )
{
	idParser src( s, strlen( s ), "",
				  LEXFL_ALLOWMULTICHARLITERALS		|
				  LEXFL_NOSTRINGCONCAT				|
				  LEXFL_ALLOWBACKSLASHSTRINGCONCAT	|
				  LEXFL_NOFATALERRORS );

	idToken token;
	bool	needComma = false;
	bool	needNumber = false;
	while( src.ReadToken( &token ) )
	{
		switch( token.type )
		{
			case TT_NUMBER:
				needComma = true;
				needNumber = false;
				break;

			case TT_PUNCTUATION:
				if( needNumber )
				{
					return true;
				}
				if( token[0] == ',' )
				{
					if( !needComma )
					{
						return true;
					}

					needComma = false;
					break;
				}

				if( needComma )
				{
					return true;
				}

				if( token[0] == '-' )
				{
					needNumber = true;
				}
				break;

			default:
				return true;
		}
	}

	return false;
}
