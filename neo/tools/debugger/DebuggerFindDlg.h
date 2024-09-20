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
#ifndef DEBUGGERFINDDLG_H_
#define DEBUGGERFINDDLG_H_

class rvDebuggerWindow;

class rvDebuggerFindDlg
{
public:

	rvDebuggerFindDlg( );

	bool	DoModal( rvDebuggerWindow* window );

	const char*		GetFindText();

protected:

	HWND	mWnd;

private:

	static char		mFindText[ 256 ];

	static INT_PTR	CALLBACK DlgProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
};

ID_INLINE const char* rvDebuggerFindDlg::GetFindText()
{
	return mFindText;
}

#endif // DEBUGGERFINDDLG_H_