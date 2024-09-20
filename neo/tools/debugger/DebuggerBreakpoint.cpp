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
#include "DebuggerBreakpoint.h"

int rvDebuggerBreakpoint::mNextID = 1;

rvDebuggerBreakpoint::rvDebuggerBreakpoint( const char* filename, int linenumber, int id, bool onceOnly )
{
	mFilename = filename;
	mLineNumber = linenumber;
	mEnabled = true;
	mOnceOnly = onceOnly;

	if( id == -1 )
	{
		mID = mNextID++;
	}
	else
	{
		mID = id;
	}
}

rvDebuggerBreakpoint::rvDebuggerBreakpoint( rvDebuggerBreakpoint& bp )
{
	mFilename = bp.mFilename;
	mEnabled = bp.mEnabled;
	mLineNumber = bp.mLineNumber;
}

rvDebuggerBreakpoint::~rvDebuggerBreakpoint()
{
}
