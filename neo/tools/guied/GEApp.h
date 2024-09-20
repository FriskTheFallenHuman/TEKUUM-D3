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

#ifndef GEAPP_H_
#define GEAPP_H_

#include "../../sys/win32/win_local.h"

#include "../../ui/Window.h"
#include "../../ui/UserInterfaceLocal.h"

#ifndef GEOPTIONS_H_
	#include "GEOptions.h"
#endif // GEOPTIONS_H_

#ifndef GEWINDOWWRAPPER_H_
	#include "GEWindowWrapper.h"
#endif // GEWINDOWWRAPPER_H_

#ifndef GEWORKSPACE_H_
	#include "GEWorkspace.h"
#endif // GEWORKSPACE_H_

#ifndef GENAVIGATOR_H_
	#include "GENavigator.h"
#endif // GENAVIGATOR_H_

#ifndef GEPROPERTIES_H_
	#include "GEProperties.h"
#endif // GEPROPERTIES_H_

#ifndef GETRANSFORMER_H_
	#include "GETransformer.h"
#endif // GETRANSFORMER_H_

#ifndef GESTATUSBAR_H_
	#include "GEStatusBar.h"
#endif // GESTATUSBAR_H_

#ifndef GEITEMPROPS_H_
	#include "GEItemPropsDlg.h"
#endif

// Utility functions
const char* StringFromVec4( idVec4& vec );
bool		IsExpression( const char* s );


class rvGEViewer;

class rvGEApp
{
public:

	rvGEApp( );
	~rvGEApp( );

	bool				Initialize();
	void				RunFrame();
//	bool				Uninitialize			();

	bool				TranslateAccelerator( LPMSG msg );

	rvGEWorkspace*		GetActiveWorkspace( HWND* retwnd = NULL );
	rvGENavigator&		GetNavigator();
	rvGEProperties&		GetProperties();
	rvGETransformer&	GetTransformer();
	rvGEOptions&		GetOptions();
	rvGEItemProps&		GetItemProperties();
	HINSTANCE			GetInstance();
	HWND				GetMDIFrame();
	HWND				GetMDIClient();
	rvGEStatusBar&		GetStatusBar();
	HWND				GetScriptWindow();
	void				SetScriptWindow( HWND hWnd )
	{
		mScripts = hWnd;
	}

	bool				OpenFile( const char* filename );
	bool				SaveFile( const char* filename );
	bool				NewFile();

	bool				IsActive();

	void				CloseViewer();

	int					ToolWindowActivate( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

	int					MessageBox( const char* text, int flags );

	bool				ApplyProperties( idDict* dict, bool keyShortcut = false );

	// helper bool to not exit if user chooses to not do so during quit dialogs
	static bool				mDontExit;

protected:

	int						HandleCommand( WPARAM wParam, LPARAM lParam );
	int						HandleInitMenu( WPARAM wParam, LPARAM lParam );

	void					HandleCommandSave( rvGEWorkspace* workspace, const char* filename );

	bool					InitRecentFiles();
	void					UpdateRecentFiles();

	HWND					mMDIFrame;
	HWND					mMDIClient;
	HINSTANCE				mInstance;
	rvGEOptions				mOptions;
	HACCEL					mAccelerators;
	rvGENavigator			mNavigator;
	rvGETransformer			mTransformer;
	rvGEStatusBar			mStatusBar;
	rvGEProperties			mProperties;
	rvGEItemProps			mItemProperties;
	HWND					mScripts;

	HMENU					mRecentFileMenu;
	int						mRecentFileInsertPos;

	rvGEViewer*				mViewer;

	idList<rvGEWorkspace*>	mWorkspaces;
	idList<HWND>			mToolWindows;

private:

	static LRESULT CALLBACK	FrameWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK	MDIChildProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};

ID_INLINE bool rvGEApp::IsActive()
{
	return mMDIFrame ? true : false;
}

ID_INLINE rvGENavigator& rvGEApp::GetNavigator()
{
	return mNavigator;
}

ID_INLINE rvGEProperties& rvGEApp::GetProperties()
{
	return mProperties;
}

ID_INLINE rvGETransformer& rvGEApp::GetTransformer()
{
	return mTransformer;
}

ID_INLINE HWND rvGEApp::GetScriptWindow( void )
{
	return mScripts;
}

ID_INLINE rvGEItemProps& rvGEApp::GetItemProperties( void )
{
	return mItemProperties;
}

ID_INLINE rvGEOptions& rvGEApp::GetOptions()
{
	return mOptions;
}

ID_INLINE HINSTANCE rvGEApp::GetInstance()
{
	return mInstance;
}

ID_INLINE rvGEStatusBar& rvGEApp::GetStatusBar()
{
	return mStatusBar;
}

ID_INLINE HWND rvGEApp::GetMDIFrame()
{
	return mMDIFrame;
}

ID_INLINE HWND rvGEApp::GetMDIClient()
{
	return mMDIClient;
}


extern rvGEApp gApp;

#endif // GEAPP_H_
