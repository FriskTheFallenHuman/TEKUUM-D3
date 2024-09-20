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

#include "mediapreviewdlg.h"

// CEditViewDlg dialog

class CEditViewDlg : public CDialogEx
{

	DECLARE_DYNAMIC( CEditViewDlg )

public:
	enum { MATERIALS, GUIS };
	CEditViewDlg( CWnd* pParent = nullptr );   // standard constructor
	virtual ~CEditViewDlg();

	void SetMode( int _mode )
	{
		mode = _mode;
	}

	void SetMaterialInfo( const char* name, const char* file, int line );
	void SetGuiInfo( const char* name );
	void UpdateEditPreview();

	// Dialog Data
	enum { IDD = IDD_DIALOG_EDITVIEW };

protected:
	CFindReplaceDialog* findDlg;
	CMediaPreviewDlg mediaPreview;
	int mode;
	idStr fileName;
	idStr matName;
	idStr editText;
	idStr findStr;
	int line;
	CEdit editInfo;

	void ShowFindDlg();

	virtual void DoDataExchange( CDataExchange* pDX ) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog() override;
	afx_msg void OnDestroy();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnBnClickedButtonGoto();
	virtual BOOL PreTranslateMessage( MSG* pMsg ) override;
	afx_msg LRESULT OnFindDialogMessage( WPARAM wParam, LPARAM lParam );
};
