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

#include "entitydlg.h"
#include "ConsoleDlg.h"
#include "TabsDlg.h"


// CInspectorDialog dialog

class CInspectorDialog : public CTabsDlg
{
	//DECLARE_DYNAMIC(CInspectorDialog)
public:
	CInspectorDialog( CWnd* pParent = NULL );   // standard constructor
	virtual ~CInspectorDialog();

	enum { IDD = IDD_DIALOG_INSPECTORS };

protected:
	bool initialized;
	unsigned int dockedTabs;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void AssignModel();
	void SetMode( int mode, bool updateTabs = true );
	void UpdateEntitySel( const eclass_t* ent );
	void UpdateSelectedEntity();
	void FillClassList();
	bool GetSelectAllCriteria( idStr& key, idStr& val );
	void SetDockedTabs( bool docked, int ID );

	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSizing( UINT nSide, LPRECT lpRect );
	afx_msg void OnMoving( UINT nSide, LPRECT lpRect );
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage( MSG* pMsg );

	CTabCtrl		tabInspector;
	//idGLConsoleWidget	consoleWnd;
	CConsoleDlg		consoleWnd;
	CNewTexWnd		texWnd;
	CDialogTextures mediaDlg;
	CEntityDlg		entityDlg;

	int prevMode;
};

extern CInspectorDialog* g_Inspectors;