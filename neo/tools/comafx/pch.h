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

#ifndef __PCH_H__
#define __PCH_H__

void InitAfx();

// tool tips
typedef struct toolTip_s
{
	int id;
	char* tip;
} toolTip_t;

INT_PTR DefaultOnToolHitTest( const toolTip_t* toolTips, const CDialog* dialog, CPoint point, TOOLINFO* pTI );
BOOL DefaultOnToolTipNotify( const toolTip_t* toolTips, UINT id, NMHDR* pNMHDR, LRESULT* pResult );

// edit control
bool EditControlEnterHit( CEdit* edit );
float EditVerifyFloat( CEdit* edit, bool allowNegative = true );
float EditSpinFloat( CEdit* edit, bool up );

// combo box
int SetSafeComboBoxSelection( CComboBox* combo, const char* string, int skip );
int GetSafeComboBoxSelection( CComboBox* combo, CString& string, int skip );
int UnsetSafeComboBoxSelection( CComboBox* combo, CString& string );

#endif /* !__PCH_H__ */
