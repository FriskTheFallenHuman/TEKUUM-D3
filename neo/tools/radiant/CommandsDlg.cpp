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

#include "precompiled.h"
#pragma hdrstop

#include "qe3.h"
#include "Radiant.h"
#include "CommandsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CCommandsDlg dialog

CCommandsDlg::CCommandsDlg( CWnd* pParent )
	: CDialogEx( CCommandsDlg::IDD, pParent )
{
}

void CCommandsDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_COMMANDS, m_lstCommands );
}

BEGIN_MESSAGE_MAP( CCommandsDlg, CDialogEx )
END_MESSAGE_MAP()

// CCommandsDlg message handlers

BOOL CCommandsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_lstCommands.SetTabStops( 120 );
	PopulateCommandList();
	return TRUE;
}

void CCommandsDlg::PopulateCommandList()
{
	int nCount = g_nCommandCount;
	CFile fileout;
	if( !fileout.Open( "commandlist.txt", CFile::modeCreate | CFile::modeWrite ) )
	{
		AfxMessageBox( "Failed to open commandlist.txt for writing." );
		return;
	}

	for( int n = 0; n < nCount; n++ )
	{
		CString strLine = FormatCommandLine( n );
		m_lstCommands.AddString( strLine );
		WriteCommandToFile( fileout, strLine );
	}

	fileout.Close();
}

CString CCommandsDlg::FormatCommandLine( int commandIndex )
{
	CString strKeys = GetCommandKeyString( commandIndex );
	CString strMod = GetCommandModifiersString( commandIndex );
	CString strLine;
	strLine.Format( "%s \t%s%s", g_Commands[commandIndex].m_strCommand, ( LPCTSTR )strMod, ( LPCTSTR )strKeys );
	return strLine;
}

CString CCommandsDlg::GetCommandKeyString( int commandIndex )
{
	char c = g_Commands[commandIndex].m_nKey;
	CString strKeys = CString( c );
	for( int k = 0; k < g_nKeyCount; k++ )
	{
		if( g_Keys[k].m_nVKKey == g_Commands[commandIndex].m_nKey )
		{
			strKeys = g_Keys[k].m_strName;
			break;
		}
	}
	return strKeys;
}

CString CCommandsDlg::GetCommandModifiersString( int commandIndex )
{
	CString strMod;
	if( g_Commands[commandIndex].m_nModifiers & RAD_SHIFT )
	{
		strMod = "Shift";
	}
	if( g_Commands[commandIndex].m_nModifiers & RAD_ALT )
	{
		strMod += ( strMod.IsEmpty() ? "" : " + " ) + CString( "Alt" );
	}
	if( g_Commands[commandIndex].m_nModifiers & RAD_CONTROL )
	{
		strMod += ( strMod.IsEmpty() ? "" : " + " ) + CString( "Control" );
	}
	if( !strMod.IsEmpty() )
	{
		strMod += " + ";
	}
	return strMod;
}

void CCommandsDlg::WriteCommandToFile( CFile& fileout, const CString& strLine )
{
	CString formattedLine;
	formattedLine.Format( "%s \t\t\t%s", ( LPCTSTR )strLine, "\r\n" );
	fileout.Write( formattedLine, formattedLine.GetLength() );
}