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

#ifndef __CMDLINE_PROGRESSBAR_H__
#define __CMDLINE_PROGRESSBAR_H__

// CommandlineProgressBar draws a nice progressbar in the console like you would get with boost
class CommandlineProgressBar
{
private:
	size_t tics = 0;
	size_t nextTicCount = 0;
	int	count = 0;
	int expectedCount = 0;

	int sysWidth = 1280;
	int sysHeight = 720;

public:
	CommandlineProgressBar( int _expectedCount, int width, int height )
	{
		expectedCount = _expectedCount;
		sysWidth = width;
		sysHeight = height;
	}

	void Start();
	void Increment( bool updateScreen );

	void Reset();
	void Reset( int expected );
};

#endif /* !__CMDLINE_PROGRESSBAR_H__ */
