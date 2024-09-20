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

// CFindTextureDlg dialog

void HideInfoDialog();
void ShowInfoDialog( const char* pText );

class CFindTextureDlg : public CDialogEx
{

public:
	CFindTextureDlg( CWnd* pParent = nullptr );   // standard constructor

	enum { IDD = IDD_DIALOG_FINDREPLACE };

	static  void updateTextures( const char* p );
	static void show();
	static bool isOpen();
	static void setFindStr( const char* p );
	static void setReplaceStr( const char* p );

protected:
	virtual void DoDataExchange( CDataExchange* pDX ) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bSelectedOnly;
	CString m_strFind;
	CString m_strReplace;
	BOOL m_bForce;
	BOOL m_bLive;

	afx_msg void OnBtnApply();
	afx_msg void OnOK();
	afx_msg  void OnCancel();
	afx_msg void OnSetfocusEditFind();
	afx_msg void OnSetfocusEditReplace();
};