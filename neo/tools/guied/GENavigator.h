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
#ifndef GENAVIGATOR_H_
#define GENAVIGATOR_H_

class rvGEWorkspace;
class idWindow;

class rvGENavigator
{
public:

	rvGENavigator( );

	bool	Create( HWND parent, bool visible );
	void	Show( bool visibile );

	void	Refresh();

	void	SetWorkspace( rvGEWorkspace* workspace );

	void	Update();
	void	UpdateSelections();

	HWND	GetWindow();

protected:

	void	AddWindow( idWindow* window );

	static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK ListWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT FAR PASCAL GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam );

	HWND			mWnd;
	HWND			mTree;
	HICON			mVisibleIcon;
	HICON			mVisibleIconDisabled;
	HICON			mScriptsIcon;
	HICON			mScriptsLightIcon;
	HICON			mCollapseIcon;
	HICON			mExpandIcon;
	rvGEWorkspace*	mWorkspace;
	WNDPROC			mListWndProc;
};

ID_INLINE HWND rvGENavigator::GetWindow()
{
	return mWnd;
}

#endif // GENAVIGATOR_H_