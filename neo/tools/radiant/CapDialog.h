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

// CCapDialog dialog

class CCapDialog : public CDialogEx
{
public:
	enum {BEVEL = 0, ENDCAP, IBEVEL, IENDCAP};
	CCapDialog( CWnd* pParent = NULL );   // standard constructor

	int getCapType()
	{
		return m_nCap;
	};

	enum { IDD = IDD_DIALOG_CAP };
	int		m_nCap;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};