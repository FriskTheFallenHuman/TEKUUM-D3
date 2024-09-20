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

/*
===============================================================================

	Preprocessor settings for compiling different versions.

===============================================================================
*/

// memory debugging
//#define ID_REDIRECT_NEWDELETE
//#define ID_DEBUG_MEMORY
//#define ID_DEBUG_UNINITIALIZED_MEMORY

// if enabled, the console won't toggle upon ~, unless you start the binary with +set com_allowConsole 1
// Ctrl+Alt+~ will always toggle the console no matter what
#ifndef ID_CONSOLE_LOCK
	#define ID_CONSOLE_LOCK 0
#endif

// useful for network debugging, turns off 'LAN' checks, all IPs are classified 'internet'
#ifndef ID_NOLANADDRESS
	#define ID_NOLANADDRESS 0
#endif

// let .dds be loaded from FS without altering pure state. only for developement.
#ifndef ID_PURE_ALLOWDDS
	#define ID_PURE_ALLOWDDS 0
#endif

// build an exe with no CVAR_CHEAT controls
#ifndef ID_ALLOW_CHEATS
	#define ID_ALLOW_CHEATS 0
#endif

// fake a pure client. useful to connect an all-debug client to a server
#ifndef ID_FAKE_PURE
	#define ID_FAKE_PURE 0
#endif

// verify checksums in clientinfo traffic
// NOTE: this makes the network protocol incompatible
#ifndef ID_CLIENTINFO_TAGS
	#define ID_CLIENTINFO_TAGS 0
#endif

// for win32 this is defined in preprocessor settings so that MFC can be
// compiled out.
//#define ID_DEDICATED

// don't define ID_ALLOW_TOOLS when we don't want tool code in the executable. - DG: defined in cmake now
#if 0 // defined( _WIN32 ) && defined(_MFC_VER) && !defined( ID_DEDICATED )
	#define	ID_ALLOW_TOOLS
#endif

#define ID_ENFORCE_KEY 0

#ifndef ID_ENFORCE_KEY
	#if !defined( ID_DEDICATED )
		#define ID_ENFORCE_KEY 1
	#else
		#define ID_ENFORCE_KEY 0
	#endif
#endif

#ifndef ID_ENFORCE_KEY_CLIENT
	#if ID_ENFORCE_KEY
		#define ID_ENFORCE_KEY_CLIENT 1
	#else
		#define ID_ENFORCE_KEY_CLIENT 0
	#endif
#endif

// async network

/*
DOOM III gold:	33
1.1 beta patch:	34
1.1 patch:		35
1.2 XP:			36-39
1.3 patch:		40
1.3.1:			41
dhewm3			42
KROOM3 WIP		43
*/
#define ASYNC_PROTOCOL_MINOR	(43)
#define ASYNC_PROTOCOL_VERSION	(( ASYNC_PROTOCOL_MAJOR << 16 ) + ASYNC_PROTOCOL_MINOR)

#define MAX_ASYNC_CLIENTS		(32)

#define MAX_USERCMD_BACKUP		(256)
#define MAX_USERCMD_DUPLICATION	(25)
#define MAX_USERCMD_RELAY		(10)

// index 0 is hardcoded to be the idnet master
// which leaves 4 to user customization
#define MAX_MASTER_SERVERS		(5)

#define MAX_NICKLEN				(32)

// max number of servers that will be scanned for at a single IP address
#define MAX_SERVER_PORTS		(8)

// special game init ids
#define GAME_INIT_ID_INVALID	(-1)
#define GAME_INIT_ID_MAP_LOAD	(-2)