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

#include "AboutBoxDlg.h"

CAboutDlg::CAboutDlg( UINT nIDTemplate, CWnd* pParent /*=nullptr*/ )
	: CDialog( nIDTemplate, pParent )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
	ON_COMMAND( IDOK, &CAboutDlg::OnOK )
END_MESSAGE_MAP()

void CAboutDlg::OnOK()
{
	EndDialog( IDOK );
}

void CAboutDlg::SetDialogTitle( const CString& title )
{
	m_strTitle = title;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( !m_strTitle.IsEmpty() )
	{
		SetWindowText( m_strTitle );
	}

	CString buffer;
	buffer.Format( "Renderer:\t%s", glGetString( GL_RENDERER ) );
	SetDlgItemText( IDC_ABOUT_GLRENDERER, buffer );

	buffer.Format( "Version:\t\t%s", glGetString( GL_VERSION ) );
	SetDlgItemText( IDC_ABOUT_GLVERSION, buffer );

	buffer.Format( "Vendor:\t\t%s", glGetString( GL_VENDOR ) );
	SetDlgItemText( IDC_ABOUT_GLVENDOR, buffer );

	const GLubyte* extensions = glGetString( GL_EXTENSIONS );
	if( extensions )
	{
		CString extStr = ( char* )extensions;
		CListBox* pListBox = ( CListBox* )GetDlgItem( IDC_ABOUT_GLEXTENSIONS );

		int start = 0;
		int end;
		while( ( end = extStr.Find( ' ', start ) ) != -1 )
		{
			pListBox->AddString( extStr.Mid( start, end - start ) );
			start = end + 1;
		}
		pListBox->AddString( extStr.Mid( start ) );
	}
	else
	{
		SetDlgItemText( IDC_ABOUT_GLEXTENSIONS, "No extensions found." );
	}

	return TRUE;
}