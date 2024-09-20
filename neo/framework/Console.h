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

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

enum justify_t
{
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER_LEFT,
	JUSTIFY_CENTER_RIGHT
};

class idOverlayHandle
{
	friend class idConsoleLocal;
public:
	idOverlayHandle() : index( -1 ), time( 0 ) {}
private:
	int		index;
	int		time;
};

/*
===============================================================================

	The console is strictly for development and advanced users. It should
	never be used to convey actual game information to the user, which should
	always be done through a GUI.

	The force options are for the editor console display window, which
	doesn't respond to pull up / pull down

===============================================================================
*/

class idConsole
{
public:
	virtual			~idConsole() {}

	virtual void	Init() = 0;
	virtual void	Shutdown() = 0;

	virtual bool	ProcessEvent( const sysEvent_t* event, bool forceAccept ) = 0;

	// the system code can release the mouse pointer when the console is active
	virtual bool	Active() = 0;

	// clear the timers on any recent prints that are displayed in the notify lines
	virtual void	ClearNotifyLines() = 0;

	// force console open
	virtual void	Open() = 0;

	// some console commands, like timeDemo, will force the console closed before they start
	virtual void	Close() = 0;

	virtual void	Draw( bool forceFullScreen ) = 0;
	virtual void	Print( const char* text ) = 0;

	virtual void	PrintOverlay( idOverlayHandle& handle, justify_t justify, VERIFY_FORMAT_STRING const char* text, ... ) = 0;

	virtual idDebugGraph* 	CreateGraph( int numItems ) = 0;
	virtual void			DestroyGraph( idDebugGraph* graph ) = 0;
};

extern idConsole* 	console;	// statically initialized to an idConsoleLocal

#endif /* !__CONSOLE_H__ */
