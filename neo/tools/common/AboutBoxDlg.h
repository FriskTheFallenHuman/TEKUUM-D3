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

#ifndef ABOUTBOXDLG_H
#define ABOUTBOXDLG_H

#include "../../sys/win32/rc/resource.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg( UINT nIDTemplate, CWnd* pParent = nullptr );

	void SetDialogTitle( const CString& title );

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();

	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strTitle;
};

#endif /* !ABOUTBOXDLG_H */