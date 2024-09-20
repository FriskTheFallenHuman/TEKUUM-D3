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
#include "MapInfo.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

// CMapInfo dialog

CMapInfo::CMapInfo( CWnd* pParent )
	: CDialogEx( CMapInfo::IDD, pParent )
{
	m_nNet = 0;
	m_nTotalBrushes = 0;
	m_nTotalEntities = 0;
}

void CMapInfo::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_ENTITIES, m_lstEntity );
	DDX_Text( pDX, IDC_EDIT_NET, m_nNet );
	DDX_Text( pDX, IDC_EDIT_TOTALBRUSHES, m_nTotalBrushes );
	DDX_Text( pDX, IDC_EDIT_TOTALENTITIES, m_nTotalEntities );
}

BEGIN_MESSAGE_MAP( CMapInfo, CDialogEx )
END_MESSAGE_MAP()

// CMapInfo message handlers

BOOL CMapInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nTotalBrushes = 0;
	m_nTotalEntities = 0;
	m_nNet = 0;
	for( idEditorBrush* pBrush = active_brushes.next ; pBrush != &active_brushes ; pBrush = pBrush->next )
	{
		m_nTotalBrushes++;
		if( pBrush->owner == world_entity )
		{
			m_nNet++;
		}
	}

	CMapStringToPtr mapEntity;

	intptr_t nValue = 0;
	for( idEditorEntity* pEntity = entities.next ; pEntity != &entities ; pEntity = pEntity->next )
	{
		m_nTotalEntities++;
		nValue = 0;
		mapEntity.Lookup( pEntity->eclass->name, reinterpret_cast<void*&>( nValue ) );
		nValue++ ;
		mapEntity.SetAt( pEntity->eclass->name, reinterpret_cast<void*>( nValue ) );
	}

	m_lstEntity.ResetContent();
	m_lstEntity.SetTabStops( 96 );
	CString strKey;
	POSITION pos = mapEntity.GetStartPosition();
	while( pos )
	{
		mapEntity.GetNextAssoc( pos, strKey, reinterpret_cast<void*&>( nValue ) );
		CString strList;
		strList.Format( "%s\t%i", strKey.GetString(), nValue );
		m_lstEntity.AddString( strList );
	}

	UpdateData( FALSE );

	return TRUE;
}
