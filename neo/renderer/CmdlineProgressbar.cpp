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
#include "CmdlineProgressbar.h"


void CommandlineProgressBar::Start()
{
	// restore the original resolution, same as "vid_restart"
	glConfig.nativeScreenWidth = sysWidth;
	glConfig.nativeScreenHeight = sysHeight;
	R_SetNewMode( false );

	common->Printf( "0%%  10   20   30   40   50   60   70   80   90   100%%\n" );
	common->Printf( "|----|----|----|----|----|----|----|----|----|----|\n" );

	session->UpdateScreen( false );
}

void CommandlineProgressBar::Increment( bool updateScreen )
{
	if( ( count + 1 ) >= nextTicCount )
	{
		if( updateScreen )
		{
			// restore the original resolution, same as "vid_restart"
			glConfig.nativeScreenWidth = sysWidth;
			glConfig.nativeScreenHeight = sysHeight;
			R_SetNewMode( false );

			// resize frame buffers (this triggers SwapBuffers)
			tr.SwapCommandBuffers( NULL, NULL, NULL, NULL, NULL, NULL );
		}

		size_t ticsNeeded = ( size_t )( ( ( double )( count + 1 ) / expectedCount ) * 50.0 );

		do
		{
			common->Printf( "*" );
		}
		while( ++tics < ticsNeeded );

		nextTicCount = ( size_t )( ( tics / 50.0 ) * expectedCount );
		if( count == ( expectedCount - 1 ) )
		{
			if( tics < 51 )
			{
				common->Printf( "*" );
			}
			common->Printf( "\n" );
		}

		if( updateScreen )
		{
			session->UpdateScreen( false );

			// swap front / back buffers
			tr.SwapCommandBuffers( NULL, NULL, NULL, NULL, NULL, NULL );
		}
	}

	count++;
}

void CommandlineProgressBar::Reset()
{
	count = 0;
	tics = 0;
	nextTicCount = 0;
}

void CommandlineProgressBar::Reset( int expected )
{
	expectedCount = expected;
	count = 0;
	tics = 0;
	nextTicCount = 0;
}
