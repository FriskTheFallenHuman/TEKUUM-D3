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
#ifndef DEBUGGERSCRIPT_H_
#define DEBUGGERSCRIPT_H_

class idProgram;
class idUserInterfaceLocal;

class rvDebuggerScript
{
public:

	rvDebuggerScript();
	~rvDebuggerScript();

	bool	Load( const char* filename );
	bool	Reload();

	const char*		GetFilename();
	const char*		GetContents();
	idProgram*		GetProgram();
#if 0// Test code
	idProgram&		GetProgram();
#endif

	bool			IsLineCode( int linenumber );
	bool			IsFileModified( bool updateTime = false );

protected:
	void			Unload();

	idProgram*				mProgram;
	idUserInterfaceLocal*	mInterface;
	char*					mContents;
	idStr					mFilename;
	ID_TIME_T				mModifiedTime;
};

ID_INLINE const char* rvDebuggerScript::GetFilename()
{
	return mFilename;
}

ID_INLINE const char* rvDebuggerScript::GetContents()
{
	return mContents ? mContents : "";
}

ID_INLINE idProgram* rvDebuggerScript::GetProgram()
{
	return mProgram;
}


#endif // DEBUGGERSCRIPT_H_