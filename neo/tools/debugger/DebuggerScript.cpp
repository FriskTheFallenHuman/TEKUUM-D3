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

#if defined( ID_ALLOW_TOOLS )
	#include "DebuggerApp.h"
#endif
#include "DebuggerScript.h"
#include "../../ui/Window.h"
#include "../../ui/UserInterfaceLocal.h"

/*
================
rvDebuggerScript::rvDebuggerScript
================
*/
rvDebuggerScript::rvDebuggerScript()
{
	mContents  = NULL;
	mProgram   = NULL;
	mInterface = NULL;
}

/*
================
rvDebuggerScript::~rvDebuggerScript
================
*/
rvDebuggerScript::~rvDebuggerScript()
{
	Unload( );
}


/*
================
rvDebuggerScript::Unload

Unload the script from memory
================
*/
void rvDebuggerScript::Unload()
{
	delete[] mContents;

	if( mInterface )
	{
		delete mInterface;
	}

	mContents  = NULL;
	mProgram   = NULL;
	mInterface = NULL;
}

/*
================
rvDebuggerScript::Load

Loads the debugger script and attempts to compile it using the method
appropriate for the file being loaded.  If the script cant be compiled
the loading of the script fails
================
*/
bool rvDebuggerScript::Load( const char* filename )
{
	void* buffer;
	int	  size;

	// Unload the script before reloading it
	Unload( );

	// Cache the filename used to load the script
	mFilename = filename;

	// Read in the file
	size = fileSystem->ReadFile( filename, &buffer, &mModifiedTime );
	if( buffer == NULL )
	{
		return false;
	}

	// Copy the buffer over
	mContents = new char [ size + 1 ];
	memcpy( mContents, buffer, size );
	mContents[size] = 0;

	// Cleanup
	fileSystem->FreeFile( buffer );

	return true;
}

/*
================
rvDebuggerScript::Reload

Reload the contents of the script
================
*/
bool rvDebuggerScript::Reload()
{
	return Load( mFilename );
}

/*
================
rvDebuggerScript::IsValidLine

Determines whether or not the given line number within the script is a valid line of code
================
*/
bool rvDebuggerScript::IsLineCode( int linenumber )
{
	//we let server decide.
	return true;
}

/*
================
rvDebuggerScript::IsFileModified

Determines whether or not the file loaded for this script has been modified since
it was loaded.
================
*/
bool rvDebuggerScript::IsFileModified( bool updateTime )
{
	ID_TIME_T	t;
	bool	result = false;

	// Grab the filetime and shut the file down
	fileSystem->ReadFile( mFilename, NULL, &t );

	// Has the file been modified?
	if( t > mModifiedTime )
	{
		result = true;
	}

	// If updateTime is true then we will update the modified time
	// stored in the script with the files current modified time
	if( updateTime )
	{
		mModifiedTime = t;
	}

	return result;
}
