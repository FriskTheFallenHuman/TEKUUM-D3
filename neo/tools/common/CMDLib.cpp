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

#include "CMDLib.h"

#define PATHSEPERATOR   '/'

/*
================
Q_filelength
================
*/
int Q_filelength( FILE* f )
{
	int		pos;
	int		end;

	pos = ftell( f );
	fseek( f, 0, SEEK_END );
	end = ftell( f );
	fseek( f, pos, SEEK_SET );

	return end;
}

/*
==============
LoadFile
==============
*/
int LoadFile( const char* filename, void** bufferptr )
{
	FILE*	f;
	int    length;
	void*    buffer;

	*bufferptr = NULL;

	if( filename == NULL || strlen( filename ) == 0 )
	{
		return -1;
	}

	f = fopen( filename, "rb" );
	if( !f )
	{
		return -1;
	}
	length = Q_filelength( f );
	buffer = Mem_ClearedAlloc( length + 1 );
	( ( char* )buffer )[length] = 0;
	if( ( int )fread( buffer, 1, length, f ) != length )
	{
		idLib::Error( "File read failure" );
	}
	fclose( f );

	*bufferptr = buffer;
	return length;
}

/*
==============
DefaultExtension
==============
*/
void DefaultExtension( char* path, char* extension )
{
	char*    src;
	//
	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	//
	src = path + strlen( path ) - 1;

	while( *src != PATHSEPERATOR && src != path )
	{
		if( *src == '.' )
		{
			return;    // it has an extension
		}
		src--;
	}

	strcat( path, extension );
}

/*
==============
DefaultPath
==============
*/
void DefaultPath( char* path, char* basepath )
{
	char    temp[128];

	if( path[0] == PATHSEPERATOR )
	{
		return;    // absolute path location
	}
	strcpy( temp, path );
	strcpy( path, basepath );
	strcat( path, temp );
}

/*
==============
StripFilename
==============
*/
void StripFilename( char* path )
{
	int             length;

	length = strlen( path ) - 1;
	while( length > 0 && path[length] != PATHSEPERATOR )
	{
		length--;
	}
	path[length] = 0;
}

/*
==============
StripExtension
==============
*/
void StripExtension( char* path )
{
	int             length;

	length = strlen( path ) - 1;
	while( length > 0 && path[length] != '.' )
	{
		length--;
		if( path[length] == '/' )
		{
			return;    // no extension
		}
	}
	if( length )
	{
		path[length] = 0;
	}
}
