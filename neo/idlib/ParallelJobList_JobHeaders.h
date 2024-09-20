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
#ifndef __PARALLELJOBLIST_JOBHEADERS_H__
#define __PARALLELJOBLIST_JOBHEADERS_H__

/*
================================================================================================

	Minimum set of headers needed to compile the code for a job.

================================================================================================
*/

#include "sys/sys_defines.h"

#include <stddef.h>					// for offsetof
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// RB: added <stdint.h> for missing uintptr_t
#include <stdint.h>

// RB begin
#if defined(__MINGW32__)
	//#include <sal.h> 	// RB: missing __analysis_assume
	#include <malloc.h> // DG: _alloca16 needs that

	#ifndef __analysis_assume
		#define __analysis_assume( x )
	#endif

#elif defined(__linux__)
	#include <malloc.h> // DG: _alloca16 needs that
	#include <signal.h>
	// RB end
	// Yamagi begin
#elif defined(__FreeBSD__)
	#include <signal.h>
#endif
// Yamagi end

#ifdef _MSC_VER
	#include <intrin.h>
	#pragma warning( disable : 4100 )	// unreferenced formal parameter
	#pragma warning( disable : 4127 )	// conditional expression is constant
#endif



#include "sys/sys_assert.h"
#include "sys/sys_types.h"
#include "sys/sys_intrinsics.h"
#include "math/Math.h"
#include "ParallelJobList.h"

#if _MSC_VER >= 1600
	#undef NULL
	#define NULL 0
#endif

#endif // !__PARALLELJOBLIST_JOBHEADERS_H__
