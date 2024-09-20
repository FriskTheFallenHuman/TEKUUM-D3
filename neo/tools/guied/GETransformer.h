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

#ifndef GETRANSFORMER_H_
#define GETRANSFORMER_H_

class rvGETransformer
{
public:

	rvGETransformer( );

	bool		Create( HWND parent, bool visible );
	void		Show( bool show );

	void		SetWorkspace( rvGEWorkspace* workspace );
	void		Update();

	bool		GetLockAspect();
	HWND		GetWindow();
	rvGEWorkspace* 	GetWorkspace()
	{
		return mWorkspace;
	}
protected:

	HWND			mWnd;
	HWND			mDlg;
	rvGEWorkspace*	mWorkspace;
	idWindow*		mRelative;

private:

	static LRESULT CALLBACK		WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static INT_PTR CALLBACK		DlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT FAR PASCAL	GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam );
};

ID_INLINE HWND rvGETransformer::GetWindow()
{
	return mWnd;
}

#endif // GETRANSFORMER_H_