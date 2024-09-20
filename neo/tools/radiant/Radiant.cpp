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

#include "qe3.h"
#include "radiant.h"
#include "MainFrm.h"
#include "lightdlg.h"

#include <process.h>    // for _beginthreadex and _endthreadex
#include <ddeml.h>  // for MSGF_DDEMGR

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

idCVar radiant_entityMode( "radiant_entityMode", "0", CVAR_TOOL | CVAR_ARCHIVE, "" );

BEGIN_MESSAGE_MAP( CRadiantApp, CWinAppEx )
	ON_COMMAND( ID_HELP, &CRadiantApp::OnAppHelp )
	ON_COMMAND( ID_HELP_ABOUT, &CRadiantApp::OnAppAbout )
	// Standard file based document commands
	ON_COMMAND( ID_FILE_NEW, &CWinAppEx::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, &CWinAppEx::OnFileOpen )
END_MESSAGE_MAP()

/*
================
CRadiantApp::CRadiantApp
================
*/
CRadiantApp::CRadiantApp() noexcept
{
	SetAppID( _T( "Radiant.AppID.NoVersion" ) );
}

CRadiantApp theApp;

/*
================
CRadiantApp::InitInstance
================
*/
BOOL CRadiantApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if( !AfxOleInit() )
	{
		return FALSE;
	}

	EnableTaskbarInteraction( FALSE );

	AfxEnableControlContainer();

	// AfxInitRichEdit2() is required to use RichEdit control
	AfxInitRichEdit2();

	// Change the registry key under which our settings are stored.
	SetRegistryKey( EDITOR_REGISTRY_KEY );

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// create main MDI Frame window
	g_PrefsDlg.LoadPrefs();

	glEnableClientState( GL_VERTEX_ARRAY );

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL,
			RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CFrameWnd* pFrame = new CMainFrame;
	if( !pFrame )
	{
		return FALSE;
	}

	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame( IDR_MAINFRAME,
					   WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
					   nullptr );

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow( SW_SHOW );
	pFrame->UpdateWindow();

	return TRUE;
}

/*
================
CRadiantApp::ExitInstance
================
*/
int CRadiantApp::ExitInstance()
{
	common->Shutdown();
	g_pParentWnd = NULL;
	ExitProcess( 0 );
	return CWinAppEx::ExitInstance();
}

/*
================
CRadiantApp::OnIdle
================
*/
BOOL CRadiantApp::OnIdle( LONG lCount )
{
	if( g_pParentWnd )
	{
		g_pParentWnd->RoutineProcessing();
	}
	return FALSE;
	//return CWinAppEx::OnIdle(lCount);
}

/*
================
CRadiantApp::OnAppHelp
================
*/
void CRadiantApp::OnAppHelp()
{
	ShellExecute( m_pMainWnd->GetSafeHwnd(), "open", "https://iddevnet.dhewm3.org/doom3/index.html", NULL, NULL, SW_SHOW );
}

/*
================
CRadiantApp::Run
================
*/
int CRadiantApp::Run()
{
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	lastFrameTime = std::chrono::steady_clock::now();

#if _MSC_VER >= 1300
	MSG* msg = AfxGetCurrentMessage();			// TODO Robert fix me!!
#else
	MSG* msg = &m_msgCur;
#endif

	// phase1: check to see if we can do idle work
	while( bIdle &&	!::PeekMessage( msg, NULL, NULL, NULL, PM_NOREMOVE ) )
	{
		// call OnIdle while in bIdle state
		if( !OnIdle( lIdleCount++ ) )
		{
			bIdle = FALSE; // assume "no idle" state
		}
	}

	// phase2: pump messages while available
	do
	{
		// pump message, but quit on WM_QUIT
		if( !PumpMessage() )
		{
			return ExitInstance();
		}

		// reset "no idle" state after pumping "normal" message
		if( IsIdleMessage( msg ) )
		{
			bIdle = TRUE;
			lIdleCount = 0;
		}

	}
	while( ::PeekMessage( msg, NULL, NULL, NULL, PM_NOREMOVE ) );

	return 0;
}

class CAboutRadiantDlg : public CAboutDlg
{
public:
	CAboutRadiantDlg();
	virtual BOOL OnInitDialog();
};

CAboutRadiantDlg::CAboutRadiantDlg() : CAboutDlg( IDD_ABOUT )
{
	SetDialogTitle( _T( "About idStudio" ) );
}

BOOL CAboutRadiantDlg::OnInitDialog()
{
	CAboutDlg::OnInitDialog();

	CString buffer;
	buffer.Format( "idStudio Build: %i\n%s\nCopyright 2004, 2011 Id Software, Inc\n\n", BUILD_NUMBER, ID__DATE__ );
	SetDlgItemText( IDC_ABOUT_TEXT, buffer );

	return TRUE;
}

/*
================
CRadiantApp::OnAppAbout
================
*/
void CRadiantApp::OnAppAbout()
{
	CAboutRadiantDlg aboutDlg;
	aboutDlg.DoModal();
}