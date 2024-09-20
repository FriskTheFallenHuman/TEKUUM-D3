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

// RB: missing __analysis_assume
#if defined(__MINGW32__)
	#include <sal.h>
#endif
// RB end

#include "ParallelJobList_JobHeaders.h"

#ifdef _WIN32
	#include <windows.h> // for DebugBreak
#else // POSIX for raise()
	#include <signal.h>
#endif

/*
================================================================================================

	Software Cache

================================================================================================
*/

uint32_t globalDmaTag;

bool SpursEmulationAssertFailed( const char* filename, int line, const char* expression )
{
	static bool halt = true;
	if( halt )
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
	return true;
}
