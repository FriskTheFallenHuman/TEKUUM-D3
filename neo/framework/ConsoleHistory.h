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
#ifndef __CONSOLEHISTORY_H__
#define __CONSOLEHISTORY_H__

/*

This should behave like the windows command prompt, with the addition
of a persistant history file.

Note that commands bound to keys do not go through the console history.

*/

class idConsoleHistory
{
public:
	idConsoleHistory() :
		upPoint( 0 ),
		downPoint( 0 ),
		returnLine( 0 ),
		numHistory( 0 )
	{
		ClearHistory();
	}

	void	LoadHistoryFile();

	// the line should not have a \n
	// Empty lines are never added to the history.
	// If the command is the same as the last returned history line, nothing is changed.
	void	AddToHistory( const char* line, bool writeHistoryFile = true );

	// the string will not have a \n
	// Returns an empty string if there is nothing to retrieve in that
	// direction.
	idStr	RetrieveFromHistory( bool backward );

	// console commands
	void	PrintHistory();
	void	ClearHistory();

private:
	int		upPoint;	// command to be retrieved with next up-arrow
	int		downPoint;	// command to be retrieved with next down-arrow
	int		returnLine;	// last returned by RetrieveFromHistory()
	int		numHistory;

	static const int COMMAND_HISTORY = 64;
	idArray<idStr, COMMAND_HISTORY>	historyLines;

	compile_time_assert( CONST_ISPOWEROFTWO( COMMAND_HISTORY ) );	// we use the binary 'and' operator for wrapping
};

extern idConsoleHistory consoleHistory;

#endif // !__CONSOLEHISTORY_H__
