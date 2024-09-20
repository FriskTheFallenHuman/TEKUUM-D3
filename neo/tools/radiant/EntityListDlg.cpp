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
#include "EntityListDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CEntityListDlg g_EntityListDlg;

// CEntityListDlg dialog

void CEntityListDlg::ShowDialog()
{
	if( g_EntityListDlg.GetSafeHwnd() == NULL )
	{
		g_EntityListDlg.Create( IDD_DLG_ENTITYLIST );
	}
	g_EntityListDlg.UpdateList();
	g_EntityListDlg.ShowWindow( SW_SHOW );

}

CEntityListDlg::CEntityListDlg( CWnd* pParent )
	: CDialogEx( CEntityListDlg::IDD, pParent )
{
}


void CEntityListDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_ENTITY, m_lstEntity );
	DDX_Control( pDX, IDC_LIST_ENTITIES, listEntities );
}

BEGIN_MESSAGE_MAP( CEntityListDlg, CDialogEx )
	ON_BN_CLICKED( IDC_SELECT, OnSelect )
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE( IDC_LIST_ENTITIES, OnLbnSelchangeListEntities )
	ON_LBN_DBLCLK( IDC_LIST_ENTITIES, OnLbnDblclkListEntities )
END_MESSAGE_MAP()

// CEntityListDlg message handlers

void CEntityListDlg::OnSelect()
{
	int index = listEntities.GetCurSel();
	if( index != LB_ERR )
	{
		idEditorEntity* ent = reinterpret_cast<idEditorEntity*>( listEntities.GetItemDataPtr( index ) );
		if( ent )
		{
			Select_Deselect();
			Select_Brush( ent->brushes.onext );
		}
	}
	Sys_UpdateWindows( W_ALL );
}

void CEntityListDlg::UpdateList()
{
	listEntities.ResetContent();
	for( idEditorEntity* pEntity = entities.next ; pEntity != &entities ; pEntity = pEntity->next )
	{
		int index = listEntities.AddString( pEntity->epairs.GetString( "name" ) );
		if( index != LB_ERR )
		{
			listEntities.SetItemDataPtr( index, ( void* )pEntity );
		}
	}
}

void CEntityListDlg::OnSysCommand( UINT nID,  LPARAM lParam )
{
	if( nID == SC_CLOSE )
	{
		DestroyWindow();
	}
}

void CEntityListDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CEntityListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateList();

	CRect rct;
	m_lstEntity.GetClientRect( rct );
	m_lstEntity.InsertColumn( 0, "Key", LVCFMT_LEFT, rct.Width() / 2 );
	m_lstEntity.InsertColumn( 1, "Value", LVCFMT_LEFT, rct.Width() / 2 );
	m_lstEntity.DeleteColumn( 2 );
	UpdateData( FALSE );

	return TRUE;
}

void CEntityListDlg::OnClose()
{
	DestroyWindow();
}

void CEntityListDlg::OnLbnSelchangeListEntities()
{
	int index = listEntities.GetCurSel();
	if( index != LB_ERR )
	{
		m_lstEntity.DeleteAllItems();
		idEditorEntity* pEntity = reinterpret_cast<idEditorEntity*>( listEntities.GetItemDataPtr( index ) );
		if( pEntity )
		{
			int count = pEntity->epairs.GetNumKeyVals();
			for( int i = 0; i < count; i++ )
			{
				int nParent = m_lstEntity.InsertItem( 0, pEntity->epairs.GetKeyVal( i )->GetKey() );
				m_lstEntity.SetItem( nParent, 1, LVIF_TEXT, pEntity->epairs.GetKeyVal( i )->GetValue(), 0, 0, 0, ( LPARAM )( pEntity ) );
			}
		}
	}
}

void CEntityListDlg::OnLbnDblclkListEntities()
{
	OnSelect();
}
