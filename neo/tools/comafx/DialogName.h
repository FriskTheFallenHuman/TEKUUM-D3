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

#ifndef __DIALOGNAME_H__
#define __DIALOGNAME_H__

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000
// NameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DialogName dialog

class DialogName : public CDialog
{
	CString m_strCaption;
// Construction
public:
	DialogName( const char* pName, CWnd* pParent = NULL ); // standard constructor

// Dialog Data
	//{{AFX_DATA(DialogName)
	enum { IDD = IDD_NEWNAME };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogName)
protected:
	virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DialogName)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* !__DIALOGNAME_H__ */
