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

#ifndef GEOPTIONS_H_
#define GEOPTIONS_H_

#ifndef REGISTRYOPTIONS_H_
	#include "../common/registryoptions.h"
#endif

class idRectangle;

class rvGEOptions
{
public:

	static const int MAX_MRU_SIZE = rvRegistryOptions::MAX_MRU_SIZE;

	rvGEOptions();

	void			Init();

	// Write the options to the registery
	bool			Save();

	// Read the options from the registry
	bool			Load();

	void			SetSelectionColor( idVec4& color );
	void			SetSelectionColor( COLORREF color );
	void			SetGridColor( idVec4& color );
	void			SetGridColor( COLORREF color );
	void			SetGridWidth( int width );
	void			SetGridHeight( int height );
	void			SetGridVisible( bool vis );
	void			SetGridSnap( bool snap );
	void			SetLastOptionsPage( int page );
	void			SetItemPropertiesVisible( bool vis );
	void			SetScriptsVisible( bool vis );
	void			SetNavigatorVisible( bool vis );
	void			SetPropertiesVisible( bool vis );
	void			SetTransformerVisible( bool vis );
	void			SetIgnoreDesktopSelect( bool ignore );
	void			SetStatusBarVisible( bool vis );

	void			AddRecentFile( const char* filename );
	int				GetRecentFileCount();
	const char*		GetRecentFile( int index );

	idVec4&			GetGridColor();
	int				GetGridWidth();
	int				GetGridHeight();
	bool			GetGridVisible();
	bool			GetGridSnap();
	int				GetLastOptionsPage();
	idVec4&			GetWorkspaceColor();
	bool			GetNavigatorVisible();
	bool			GetTransformerVisible();
	bool			GetPropertiesVisible();
	bool			GetItemPropertiesVisible();
	bool			GetScriptsVisible();
	idVec4&			GetSelectionColor();
	COLORREF*		GetCustomColors();
	bool			GetIgnoreDesktopSelect();
	bool			GetStatusBarVisible();

	void			SetWindowPlacement( const char* name, HWND hwnd );
	bool			GetWindowPlacement( const char* name, HWND hwnd );

	void			SnapRectToGrid( idRectangle& rect, bool snapLeft = true, bool snapTop = true, bool snapWidth = true, bool snapHeight = true );

protected:

	void				ConvertColor( COLORREF src, idVec4& dest );
	void				SetModified( bool mod );

	bool				mModified;
	int					mLastOptionsPage;

	idVec4				mGridColor;
	int					mGridWidth;
	int					mGridHeight;
	bool				mGridSnap;
	bool				mGridVisible;

	idVec4				mWorkspaceColor;
	idVec4				mSelectionColor;

	bool				mNavigatorVisible;
	bool				mItemPropertiesVisible;
	bool				mPropertiesVisible;
	bool				mTransformerVisible;
	bool				mScriptsVisible;
	bool				mStatusBarVisible;
	bool				mIgnoreDesktopSelect;

	idList<idStr>		mRecentFiles;

	COLORREF			mCustomColors[16];

	rvRegistryOptions	mRegistry;
};

ID_INLINE void rvGEOptions::SetModified( bool mod )
{
	mModified = mod;
}

ID_INLINE void rvGEOptions::ConvertColor( COLORREF src, idVec4& dest )
{
	dest[0] = ( float )GetRValue( src ) / 255.0f;
	dest[1] = ( float )GetGValue( src ) / 255.0f;
	dest[2] = ( float )GetBValue( src ) / 255.0f;
	dest[3] = 1.0f;
}

ID_INLINE void rvGEOptions::SetGridWidth( int width )
{
	mGridWidth = width;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetGridHeight( int height )
{
	mGridHeight = height;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetGridSnap( bool snap )
{
	mGridSnap = snap;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetGridVisible( bool vis )
{
	mGridVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetStatusBarVisible( bool vis )
{
	mStatusBarVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetGridColor( COLORREF color )
{
	ConvertColor( color, mGridColor );
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetSelectionColor( idVec4& color )
{
	VectorCopy( color, mSelectionColor );
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetSelectionColor( COLORREF color )
{
	ConvertColor( color, mSelectionColor );
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetGridColor( idVec4& color )
{
	VectorCopy( color, mGridColor );
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetNavigatorVisible( bool vis )
{
	mNavigatorVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetScriptsVisible( bool vis )
{
	mScriptsVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetItemPropertiesVisible( bool vis )
{
	mItemPropertiesVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetPropertiesVisible( bool vis )
{
	mPropertiesVisible = vis;
	SetModified( true );
}

ID_INLINE void rvGEOptions::SetTransformerVisible( bool vis )
{
	mTransformerVisible = vis;
	SetModified( true );
}

ID_INLINE idVec4& rvGEOptions::GetGridColor()
{
	return mGridColor;
}

ID_INLINE int rvGEOptions::GetGridWidth()
{
	return mGridWidth;
}

ID_INLINE int rvGEOptions::GetGridHeight()
{
	return mGridHeight;
}

ID_INLINE bool rvGEOptions::GetGridVisible()
{
	return mGridVisible;
}

ID_INLINE bool rvGEOptions::GetGridSnap()
{
	return mGridSnap;
}

ID_INLINE idVec4& rvGEOptions::GetWorkspaceColor()
{
	return mWorkspaceColor;
}

ID_INLINE int rvGEOptions::GetLastOptionsPage()
{
	return mLastOptionsPage;
}

ID_INLINE void rvGEOptions::SetLastOptionsPage( int page )
{
	mLastOptionsPage = page;
}

ID_INLINE bool rvGEOptions::GetNavigatorVisible()
{
	return mNavigatorVisible;
}

ID_INLINE bool rvGEOptions::GetPropertiesVisible()
{
	return mPropertiesVisible;
}

ID_INLINE bool rvGEOptions::GetScriptsVisible()
{
	return mScriptsVisible;
}

ID_INLINE bool rvGEOptions::GetItemPropertiesVisible()
{
	return mItemPropertiesVisible;
}

ID_INLINE bool rvGEOptions::GetTransformerVisible()
{
	return mTransformerVisible;
}

ID_INLINE bool rvGEOptions::GetStatusBarVisible()
{
	return mStatusBarVisible;
}

ID_INLINE idVec4& rvGEOptions::GetSelectionColor()
{
	return mSelectionColor;
}

ID_INLINE COLORREF* rvGEOptions::GetCustomColors()
{
	return mCustomColors;
}

ID_INLINE void rvGEOptions::SetIgnoreDesktopSelect( bool ignore )
{
	mIgnoreDesktopSelect = ignore;
}

ID_INLINE bool rvGEOptions::GetIgnoreDesktopSelect()
{
	return mIgnoreDesktopSelect;
}

ID_INLINE void rvGEOptions::SetWindowPlacement( const char* name, HWND hwnd )
{
	mRegistry.SetWindowPlacement( name, hwnd );
}

ID_INLINE bool rvGEOptions::GetWindowPlacement( const char* name, HWND hwnd )
{
	return mRegistry.GetWindowPlacement( name, hwnd );
}

ID_INLINE void rvGEOptions::AddRecentFile( const char* filename )
{
	mRegistry.AddRecentFile( filename );
}

ID_INLINE int rvGEOptions::GetRecentFileCount()
{
	return mRegistry.GetRecentFileCount( );
}

ID_INLINE const char* rvGEOptions::GetRecentFile( int index )
{
	return mRegistry.GetRecentFile( index );
}

#endif // _GEOPTIONS_H_
