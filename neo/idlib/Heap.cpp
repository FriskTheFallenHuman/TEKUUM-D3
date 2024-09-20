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

//===============================================================
//
//	memory allocation all in one place
//
//===============================================================
#include <stdlib.h>
#undef new

/*
==================
Mem_Alloc16
==================
*/
// RB: 64 bit fixes, changed int to size_t
void* Mem_Alloc16( const size_t size )
// RB end
{
	if( !size )
	{
		return NULL;
	}
	const size_t paddedSize = ( size + 15 ) & ~15;
#ifdef _WIN32
	// this should work with MSVC and mingw, as long as __MSVCRT_VERSION__ >= 0x0700
	return _aligned_malloc( paddedSize, 16 );
#else // not _WIN32
	// DG: the POSIX solution for linux etc
	void* ret;
	posix_memalign( &ret, 16, paddedSize );
	return ret;
	// DG end
#endif // _WIN32
}

/*
==================
Mem_Free16
==================
*/
void Mem_Free16( void* ptr )
{
	if( ptr == NULL )
	{
		return;
	}
#ifdef _WIN32
	_aligned_free( ptr );
#else // not _WIN32
	// DG: Linux/POSIX compatibility
	// can use normal free() for aligned memory
	free( ptr );
	// DG end
#endif // _WIN32
}

/*
==================
Mem_ClearedAlloc
==================
*/
void* Mem_ClearedAlloc( const size_t size )
{
	void* mem = Mem_Alloc( size );
	SIMDProcessor->Memset( mem, 0, size );
	return mem;
}

/*
==================
Mem_CopyString
==================
*/
char* Mem_CopyString( const char* in )
{
	char* out = ( char* )Mem_Alloc( strlen( in ) + 1 );
	strcpy( out, in );
	return out;
}

