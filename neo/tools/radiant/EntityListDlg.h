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

// CEntityListDlg dialog

class CEntityListDlg : public CDialogEx
{
public:
	CEntityListDlg( CWnd* pParent = NULL );   // standard constructor
	void UpdateList();
	static void ShowDialog();

	enum { IDD = IDD_DLG_ENTITYLIST };
	CListCtrl	m_lstEntity;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

protected:
	afx_msg void OnSelect();
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand( UINT nID,  LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	CListBox listEntities;
	afx_msg void OnLbnSelchangeListEntities();
	afx_msg void OnLbnDblclkListEntities();
};