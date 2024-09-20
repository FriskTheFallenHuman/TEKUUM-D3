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
#ifndef DEBUGGERMESSAGES_H_
#define DEBUGGERMESSAGES_H_

enum EDebuggerMessage
{
	DBMSG_UNKNOWN,
	DBMSG_CONNECT,
	DBMSG_CONNECTED,
	DBMSG_DISCONNECT,
	DBMSG_ADDBREAKPOINT,
	DBMSG_REMOVEBREAKPOINT,
	DBMSG_HITBREAKPOINT,
	DBMSG_RESUME,
	DBMSG_RESUMED,
	DBMSG_BREAK,
	DBMSG_PRINT,
	DBMSG_INSPECTVARIABLE,
	DBMSG_INSPECTCALLSTACK,
	DBMSG_INSPECTTHREADS,
	DBMSG_STEPOVER,
	DBMSG_STEPINTO,
	DBMSG_INSPECTSCRIPTS,
	DBMSG_EXECCOMMAND
};

#endif // DEBUGGER_MESSAGES_H_