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

#include "../../sys/win32/win_local.h"

#include "MaterialEditor.h"
#include "MEMainFrame.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

MEMainFrame* meMainFrame = NULL;

CFont* materialEditorFont = NULL;

/**
* Initializes the material editor tool.
*/
void MaterialEditorInit()
{

	InitPropTree( win32.hInstance );

	com_editors = EDITOR_MATERIAL;


	InitAfx();

	InitCommonControls();

	// Initialize OLE libraries
	if( !AfxOleInit() )
	{
		return;
	}
	AfxEnableControlContainer();

	NONCLIENTMETRICS info;
	info.cbSize = sizeof( info );

	::SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof( info ), &info, 0 );

	LOGFONT lf;
	memset( &lf, 0, sizeof( LOGFONT ) );

	CWindowDC dc( NULL );
	lf.lfCharSet = ( BYTE )GetTextCharsetInfo( dc.GetSafeHdc(), NULL, 0 );

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	// check if we should use system font
	_tcscpy( lf.lfFaceName, info.lfMenuFont.lfFaceName );

	materialEditorFont = new CFont;
	materialEditorFont->CreateFontIndirect( &lf );


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	meMainFrame = new MEMainFrame;

	// create and load the frame with its resources
	meMainFrame->LoadFrame( IDR_ME_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL );


	// hide the doom window by default
	::ShowWindow( win32.hWnd, SW_HIDE );

	// The one and only window has been initialized, so show and update it
	meMainFrame->ShowWindow( SW_SHOW );
	meMainFrame->UpdateWindow();
}

/**
* Called every frame by the doom engine to allow the material editor to process messages.
*/
void MaterialEditorRun()
{

	MSG* msg = AfxGetCurrentMessage();

	while( ::PeekMessage( msg, NULL, NULL, NULL, PM_NOREMOVE ) )
	{
		// pump message
		if( !AfxGetApp()->PumpMessage() )
		{
		}
	}
}

/**
* Called by the doom engine when the material editor needs to be destroyed.
*/
void MaterialEditorShutdown()
{

	delete meMainFrame;

	delete materialEditorFont;

	meMainFrame = NULL;
}

/**
* Allows the doom engine to reflect console output to the material editors console.
*/
void MaterialEditorPrintConsole( const char* msg )
{
	//meMainFrame can be null when starting immedeatly from commandline.
	if( meMainFrame && com_editors & EDITOR_MATERIAL )
	{
		meMainFrame->PrintConsoleMessage( msg );
	}
}

/**
* Returns the handle to the main Material Editor Window
*/
HWND GetMaterialEditorWindow()
{
	return meMainFrame->GetSafeHwnd();
}