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
#ifndef SYS_INCLUDES_H
#define SYS_INCLUDES_H

// Include the various platform specific header files (windows.h, etc)

/*
================================================================================================

	Windows

================================================================================================
*/

// RB: windows specific stuff should only be set on Windows
#if defined(_WIN32)

	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// prevent auto literal to string conversion

	#ifndef _D3SDK
		#ifndef GAME_DLL

			#ifndef WINVER
				#define WINVER				0x501
			#endif

			#ifdef ID_ALLOW_TOOLS
				// (hopefully) suppress "warning C4996: 'MBCS_Support_Deprecated_In_MFC':
				//   MBCS support in MFC is deprecated and may be removed in a future version of MFC."
				#define NO_WARN_MBCS_MFC_DEPRECATION

				#include <afxwin.h>

				#include "../tools/comafx/framework.h"
				#include "../tools/comafx/pch.h"

				// scaling factor based on DPI (dpi/96.0f, so 1.0 by default); implemented in win_main.cpp
				float Win_GetWindowScalingFactor( HWND window );
			#endif

			// RB begin
			#if !defined(_WINSOCKAPI_)
				#include <winsock2.h>
			#endif
			// RB end

			#include <mmsystem.h>
			#include <mmreg.h>

			#define DIRECTINPUT_VERSION  0x0800			// was 0x0700 with the old mssdk

			#include <dinput.h>

		#endif /* !GAME_DLL */
	#endif /* !_D3SDK */

	// DG: intrinsics for GCC
	#if defined(__GNUC__) && defined(__SSE2__)
		#include <emmintrin.h>

		// TODO: else: alternative implementations?
	#endif
	// DG end

	#ifdef _MSC_VER
		#include <intrin.h>			// needed for intrinsics like _mm_setzero_si28

		#pragma warning(disable : 4100)				// unreferenced formal parameter
		#pragma warning(disable : 4127)				// conditional expression is constant
		#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
		#pragma warning(disable : 4267)				// RB 'initializing': conversion from 'size_t' to 'int', possible loss of data
		#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
		#pragma warning(disable : 4996)				// unsafe string operations
	#endif // _MSC_VER

	#include <windows.h>						// for gl.h

#elif defined(__linux__) || defined(__FreeBSD__)

	#include <signal.h>
	#include <pthread.h>

#endif // #if defined(_WIN32)
// RB end

#include <stdlib.h>							// no malloc.h on mac or unix
#undef FindText								// fix namespace pollution


/*
================================================================================================

	Common Include Files

================================================================================================
*/

#if !defined( _DEBUG ) && !defined( NDEBUG )
	// don't generate asserts
	#define NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <typeinfo>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <memory>
// RB: added <stdint.h> for missing uintptr_t with MinGW
#include <stdint.h>
// RB end
// Yamagi: <stddef.h> for ptrdiff_t on FreeBSD
#include <stddef.h>
// Yamagi end
//#define FLT_EPSILON 1.19209290E-07F
#include <cfloat>
#include <chrono>
#include <thread>
#include <algorithm>

//-----------------------------------------------------

// Hacked stuff we may want to consider implementing later
class idScopedGlobalHeap
{
};

#endif // SYS_INCLUDES_H
