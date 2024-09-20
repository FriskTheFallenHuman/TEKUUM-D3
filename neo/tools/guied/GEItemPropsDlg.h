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
#ifndef GEITEMPROPSDLG_H_
#define GEITEMPROPSDLG_H_

//bool GEItemPropsDlg_DoModal	( HWND parent, idWindow* window, idDict& outDict );

class rvGEItemProps
{
public:
	rvGEItemProps();
	~rvGEItemProps();

	bool	Create( HWND parent, bool visible );
	void	Show( bool visible );
	void	Update( void );

	HWND	GetWindow( void );
	void	SetWorkspace( rvGEWorkspace* workspace );

protected:

	static int CALLBACK WndProc( HWND hWnd, UINT msg, LPARAM lParam );

	HWND				mWnd;
	HWND				mDlg;
	rvGEWindowWrapper*	mWrapper;
	rvGEWorkspace*		mWorkspace;

	idDict				mDict;
	PROPSHEETHEADER		propsh;
	PROPSHEETPAGE		propsp[4];
};

ID_INLINE HWND rvGEItemProps::GetWindow( void )
{
	return mWnd;
}

#endif // GEITEMPROPSDLG_H_