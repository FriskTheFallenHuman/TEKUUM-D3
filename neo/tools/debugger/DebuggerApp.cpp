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

#include "precompiled.h"
#pragma hdrstop


#include "../../sys/win32/rc/resource.h"
#include "DebuggerApp.h"

/*
================
rvDebuggerApp::rvDebuggerApp
================
*/
rvDebuggerApp::rvDebuggerApp( )  //:
//mOptions ( "Software\\id Software\\DOOM3\\Tools\\Debugger" )
{
	mInstance		= NULL;
	mDebuggerWindow = NULL;
	mAccelerators   = NULL;
}

/*
================
rvDebuggerApp::~rvDebuggerApp
================
*/
rvDebuggerApp::~rvDebuggerApp( )
{
	if( mAccelerators )
	{
		DestroyAcceleratorTable( mAccelerators );
	}
}

/*
================
rvDebuggerApp::Initialize

Initializes the debugger application by creating the debugger window
================
*/
bool rvDebuggerApp::Initialize( HINSTANCE instance )
{
	INITCOMMONCONTROLSEX ex;
	ex.dwICC = ICC_USEREX_CLASSES | ICC_LISTVIEW_CLASSES | ICC_WIN95_CLASSES;
	ex.dwSize = sizeof( INITCOMMONCONTROLSEX );

	mInstance = instance;

	mOptions.Load( );

	mDebuggerWindow = new rvDebuggerWindow;

	if( !mDebuggerWindow->Create( instance ) )
	{
		delete mDebuggerWindow;
		return false;
	}

	// Initialize the network connection for the debugger
	if( !mClient.Initialize( ) )
	{
		return false;
	}

	mAccelerators = LoadAccelerators( mInstance, MAKEINTRESOURCE( IDR_DBG_ACCELERATORS ) );

	return true;
}

/*
================
rvDebuggerApp::ProcessWindowMessages

Process windows messages
================
*/
bool rvDebuggerApp::ProcessWindowMessages()
{
	MSG	msg;

	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( !GetMessage( &msg, NULL, 0, 0 ) )
		{
			return false;
		}

		if( !TranslateAccelerator( &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return true;
}

/*
================
rvDebuggerApp::TranslateAccelerator

Translate any accelerators destined for this window
================
*/
bool rvDebuggerApp::TranslateAccelerator( LPMSG msg )
{
	if( mDebuggerWindow && ::TranslateAccelerator( mDebuggerWindow->GetWindow(), mAccelerators, msg ) )
	{
		return true;
	}

	return false;
}

/*
================
rvDebuggerApp::Run

Main Loop for the debugger application
================
*/
int rvDebuggerApp::Run()
{
	// Main message loop:
	while( ProcessWindowMessages( ) )
	{
		mClient.ProcessMessages( );

		Sleep( 0 );
	}

	mClient.Shutdown( );
	mOptions.Save( );

	delete mDebuggerWindow;

	return 1;
}
