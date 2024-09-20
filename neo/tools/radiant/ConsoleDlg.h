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

#pragma once

// CConsoleDlg dialog

class CConsoleDlg : public CDialogEx
{
	DECLARE_DYNCREATE( CConsoleDlg )

public:
	CConsoleDlg( CWnd* pParent = NULL );   // standard constructor
	virtual ~CConsoleDlg();

	enum { IDD = IDD_DIALOG_CONSOLE };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit editConsole;
	CEdit editInput;
	void AddText( const char* msg );
	void SetConsoleText( const idStr& text );
	void ExecuteCommand( const idStr& cmd = "" );

	idStr consoleStr;
	idStrList consoleHistory;
	idStr currentCommand;
	int currentHistoryPosition;
	bool saveCurrentCommand;

	afx_msg void OnSize( UINT nType, int cx, int cy );
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );
};
