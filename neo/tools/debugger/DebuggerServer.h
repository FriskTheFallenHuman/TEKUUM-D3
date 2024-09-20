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
#ifndef DEBUGGERSERVER_H_
#define DEBUGGERSERVER_H_

#ifndef DEBUGGERMESSAGES_H_
	#include "DebuggerMessages.h"
#endif

#ifndef DEBUGGERBREAKPOINT_H_
	#include "DebuggerBreakpoint.h"
#endif

#include "../framework/Game.h"
class idInterpreter;
class idProgram;

class function_t;
typedef struct prstack_s prstack_t;

class rvDebuggerServer
{
public:

	rvDebuggerServer( );
	~rvDebuggerServer( );

	bool		Initialize();
	void		Shutdown();

	bool		ProcessMessages();

	bool		IsConnected();

	void		CheckBreakpoints( idInterpreter* interpreter, idProgram* program, int instructionPointer );

	void		Print( const char* text );

	void		OSPathToRelativePath( const char* osPath, idStr& qpath );

	bool		GameSuspended();
private:

	void		ClearBreakpoints();

	void		Break( idInterpreter* interpreter, idProgram* program, int instructionPointer );
	void		Resume();

	void		SendMessage( EDebuggerMessage dbmsg );
	void		SendPacket( void* data, int datasize );

	// Message handlers
	void		HandleAddBreakpoint( idBitMsg* msg );
	void		HandleRemoveBreakpoint( idBitMsg* msg );
	void		HandleResume( idBitMsg* msg );
	void		HandleInspectVariable( idBitMsg* msg );
	void		HandleInspectCallstack( idBitMsg* msg );
	void		HandleInspectThreads( idBitMsg* msg );
	void		HandleInspectScripts( idBitMsg* msg );
	void		HandleExecCommand( idBitMsg* msg );
	////

	bool							mConnected;
	netadr_t						mClientAdr;
	idPort							mPort;
	idList<rvDebuggerBreakpoint*>	mBreakpoints;
	CRITICAL_SECTION				mCriticalSection;

	HANDLE							mGameThread;
	bool							mBreak;

	bool							mBreakNext;
	bool							mBreakStepOver;
	bool							mBreakStepInto;
	int								mBreakStepOverDepth;
	const function_t*				mBreakStepOverFunc1;
	const function_t*				mBreakStepOverFunc2;
	idProgram*						mBreakProgram;
	int								mBreakInstructionPointer;
	idInterpreter*					mBreakInterpreter;

	idStr							mLastStatementFile;
	int								mLastStatementLine;
	uintptr_t						mGameDLLHandle;
	idStrList						mScriptFileList;

};

/*
================
rvDebuggerServer::IsConnected
================
*/
ID_INLINE bool rvDebuggerServer::IsConnected()
{
	return mConnected;
}

/*
================
rvDebuggerServer::SendPacket
================
*/
ID_INLINE void rvDebuggerServer::SendPacket( void* data, int size )
{
	mPort.SendPacket( mClientAdr, data, size );
}

/*
================
rvDebuggerServer::GameSuspended
================
*/
ID_INLINE bool rvDebuggerServer::GameSuspended()
{
	return mBreak;
}

#endif // DEBUGGERSERVER_H_
