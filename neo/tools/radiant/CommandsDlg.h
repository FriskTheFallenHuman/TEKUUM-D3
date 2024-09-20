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

// CCommandsDlg dialog

class CCommandsDlg : public CDialogEx
{
public:
	CCommandsDlg( CWnd* Parent = nullptr );   // standard constructor

	// Dialog Data
	enum { IDD = IDD_DLG_COMMANDLIST };
	CListBox m_lstCommands;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void PopulateCommandList();
	CString FormatCommandLine( int commandIndex );
	CString GetCommandKeyString( int commandIndex );
	CString GetCommandModifiersString( int commandIndex );
	void WriteCommandToFile( CFile& fileout, const CString& strLine );
};