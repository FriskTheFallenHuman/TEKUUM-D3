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

#ifndef GEPROPERTYPAGE_H_
#define GEPROPERTYPAGE_H_

class rvGEPropertyPage
{
public:

	rvGEPropertyPage( );

	virtual bool	Init()
	{
		return true;
	}
	virtual bool	Apply()
	{
		return true;
	}
	virtual bool	SetActive()
	{
		return true;
	}
	virtual bool	KillActive()
	{
		return true;
	}
	virtual int		HandleMessage( UINT msg, WPARAM wParam, LPARAM lParam );

	static INT_PTR CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

protected:

	HWND		mPage;
};

#endif // GEPROPERTYPAGE_H_