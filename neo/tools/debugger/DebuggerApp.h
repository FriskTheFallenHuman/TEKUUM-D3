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
#ifndef DEBUGGERAPP_H_
#define DEBUGGERAPP_H_

#include "../../sys/win32/win_local.h"
//#include "../../framework/sync/Msg.h"

#ifndef REGISTRYOPTIONS_H_
	#include "../common/RegistryOptions.h"
#endif

#ifndef DEBUGGERWINDOW_H_
	#include "DebuggerWindow.h"
#endif

#ifndef DEBUGGERMESSAGES_H_
	#include "DebuggerMessages.h"
#endif

#ifndef DEBUGGERCLIENT_H_
	#include "DebuggerClient.h"
#endif

// These were changed to static by ID so to make it easy we just throw them
// in this header
// we need a lot to be able to list all threads in mars_city1
const int MAX_MSGLEN = 8600;

class rvDebuggerApp
{
public:

	rvDebuggerApp( );
	~rvDebuggerApp();

	bool				Initialize( HINSTANCE hInstance );
	int					Run();

	rvRegistryOptions&	GetOptions();
	rvDebuggerClient&	GetClient();
	rvDebuggerWindow&	GetWindow();

	HINSTANCE			GetInstance();

	bool				TranslateAccelerator( LPMSG msg );

protected:

	rvRegistryOptions	mOptions;
	rvDebuggerWindow*	mDebuggerWindow;
	HINSTANCE			mInstance;
	rvDebuggerClient	mClient;
	HACCEL				mAccelerators;

private:

	bool	ProcessNetMessages();
	bool	ProcessWindowMessages();
};

ID_INLINE HINSTANCE rvDebuggerApp::GetInstance()
{
	return mInstance;
}

ID_INLINE rvDebuggerClient& rvDebuggerApp::GetClient()
{
	return mClient;
}

ID_INLINE rvRegistryOptions& rvDebuggerApp::GetOptions()
{
	return mOptions;
}

ID_INLINE rvDebuggerWindow& rvDebuggerApp::GetWindow()
{
	assert( mDebuggerWindow );
	return *mDebuggerWindow;
}

extern rvDebuggerApp gDebuggerApp;

#endif // DEBUGGERAPP_H_
