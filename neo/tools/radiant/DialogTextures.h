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

#include "../common/GLWidget.h"

// CDialogTextures dialog

class CDialogTextures : public CDialogEx
{
// Construction
public:
	enum { NONE, TEXTURES, MATERIALS, MODELS, SCRIPTS, SOUNDS, SOUNDPARENT, GUIS, PARTICLES, FX, NUMIDS };
	static const char* TypeNames[NUMIDS];
	CDialogTextures( CWnd* pParent = NULL );   // standard constructor
	void OnCancel();
	void CollapseEditor();
	void SelectCurrentItem( bool collapse, const char* name, int id );

	enum { IDD = IDD_DIALOG_TEXTURELIST };
	CButton	m_chkHideRoot;
	CButton	m_btnRefresh;
	CButton	m_btnLoad;
	idGLWidget m_wndPreview;
	CImageList m_treeimageList;
	CBitmap m_treebitmap;
	CTreeCtrl	m_treeTextures;

	idGLDrawable m_testDrawable;
	idGLDrawableMaterial m_drawMaterial;
	idGLDrawableModel m_drawModel;
	const idMaterial* editMaterial;
	idStr editGui;
	idStr currentFile;
	idStr mediaName;
	bool setTexture;
	bool ignoreCollapse;
	int mode;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

protected:
	void addStrList( const char* root, const idStrList& list, int id );
	void addScripts( bool rootItems );
	void addModels( bool rootItems );
	void addMaterials( bool rootItems );
	void addSounds( bool rootItems );
	void addGuis( bool rootItems );
	void addFXs( bool rootItems );
	void addParticles( bool rootItems );
	void BuildTree();
	void CollapseChildren( HTREEITEM parent );
	const char* buildItemName( HTREEITEM item, const char* rootName );
	bool loadTree( HTREEITEM item, const idStr& name, CWaitDlg* dlg );
	HTREEITEM findItem( const char* name, HTREEITEM item, HTREEITEM* foundItem );

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLoad();
	afx_msg void OnRefresh();
	afx_msg void OnClickTreeTextures( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnSelchangedTreeTextures( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnDblclkTreeTextures( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnPreview();
	afx_msg void OnMaterialEdit();
	afx_msg void OnMaterialInfo();
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnCheckHideroot();

	DECLARE_MESSAGE_MAP()

	idHashTable<HTREEITEM>	quickTree;
	idStr					itemName;

public:
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnNMRclickTreeTextures( NMHDR* pNMHDR, LRESULT* pResult );
};