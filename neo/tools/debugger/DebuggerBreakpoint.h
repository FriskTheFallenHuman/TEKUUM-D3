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
#ifndef DEBUGGERBREAKPOINT_H_
#define DEBUGGERBREAKPOINT_H_

class rvDebuggerBreakpoint
{
public:

	rvDebuggerBreakpoint( const char* filename, int linenumber, int id = -1, bool onceOnly = false );
	rvDebuggerBreakpoint( rvDebuggerBreakpoint& bp );
	~rvDebuggerBreakpoint();

	const char*		GetFilename();
	int				GetLineNumber();
	int				GetID();
	bool			GetOnceOnly();

protected:

	bool	mEnabled;
	bool	mOnceOnly;
	int		mID;
	int		mLineNumber;
	idStr	mFilename;
private:

	static int	mNextID;
};

ID_INLINE const char* rvDebuggerBreakpoint::GetFilename()
{
	return mFilename;
}

ID_INLINE int rvDebuggerBreakpoint::GetLineNumber()
{
	return mLineNumber;
}

ID_INLINE int rvDebuggerBreakpoint::GetID()
{
	return mID;
}

ID_INLINE bool rvDebuggerBreakpoint::GetOnceOnly()
{
	return mOnceOnly;
}

#endif // DEBUGGERBREAKPOINT_H_
