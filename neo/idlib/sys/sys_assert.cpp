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

#ifndef _WIN32
	#include <signal.h> // for raise()
#endif

/*
================================================================================================
Contains the AssertMacro implementation.
================================================================================================
*/

idCVar com_assertOutOfDebugger( "com_assertOutOfDebugger", "0", CVAR_BOOL, "by default, do not assert while not running under the debugger" );

struct skippedAssertion_t
{
	skippedAssertion_t() :
		file( NULL ),
		line( -1 )
	{
	}
	const char* 	file;
	int				line;
};
static idStaticList< skippedAssertion_t, 20 > skippedAssertions;

/*
========================
AssertFailed
========================
*/
bool AssertFailed( const char* file, int line, const char* expression )
{
	// Set this to true to skip ALL assertions, including ones YOU CAUSE!
	static volatile bool skipAllAssertions = false;
	if( skipAllAssertions )
	{
		return false;
	}

	// Set this to true to skip ONLY this assertion
	static volatile bool skipThisAssertion = false;
	skipThisAssertion = false;

	for( int i = 0; i < skippedAssertions.Num(); i++ )
	{
		if( skippedAssertions[i].file == file && skippedAssertions[i].line == line )
		{
			skipThisAssertion = true;
			// Set breakpoint here to re-enable
			if( !skipThisAssertion )
			{
				skippedAssertions.RemoveIndexFast( i );
			}
			return false;
		}
	}

	idLib::Warning( "ASSERTION FAILED! %s(%d): '%s'", file, line, expression );

// RB begin
#ifdef _WIN32
	if( IsDebuggerPresent() || com_assertOutOfDebugger.GetBool() )
#else
	//if( com_assertOutOfDebugger.GetBool() )
#endif
// RB end
	{
#ifdef _WIN32
#ifdef _MSC_VER
		__debugbreak();
#else
		// DG: mingw support
		DebugBreak();
#endif
#else // not _WIN32
		// DG: POSIX support
		raise( SIGTRAP );
		// DG: end
#endif // _WIN32
	}

	if( skipThisAssertion )
	{
		skippedAssertion_t* skipped = skippedAssertions.Alloc();
		skipped->file = file;
		skipped->line = line;
	}

	return true;
}

