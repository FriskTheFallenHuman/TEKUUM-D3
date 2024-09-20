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

// CWaitDlg dialog

class CWaitDlg : public CDialogEx
{
public:
	CWaitDlg( CWnd* pParent = NULL, const char* msg = "Wait..." );   // standard constructor
	~CWaitDlg();
	void SetText( const char* msg, float percent = 0.0f, bool append = false );
	void AllowCancel( bool enable );
	bool CancelPressed();

	enum { IDD = IDD_DLG_WAIT };
	CString	waitStr;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

private:
	idStr	text;
	bool	cancelPressed;
	CProgressCtrl m_progress;
};