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

/*
========================
CommandLinks

The command system is not required for idLib, but we want to be able
to use the CONSOLE_COMMAND() macro inside idlib, so these must be here.
========================
*/
idCommandLink* CommandLinks( idCommandLink* cl )
{
	static idCommandLink* commandLinks = NULL;
	if( cl != NULL )
	{
		commandLinks = cl;
	}
	return commandLinks;
}


idCommandLink* commandLinks = NULL;

idCommandLink::idCommandLink( const char* cmdName, cmdFunction_t function,
							  const char* description, argCompletion_t argCompletion )
{
	next = CommandLinks();
	CommandLinks( this );
	cmdName_ = cmdName;
	function_ = function;
	description_ = description;
	argCompletion_ = argCompletion;
}

