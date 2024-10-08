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
#include "afxcmn.h"
#include "afxwin.h"


// CPreviewDlg dialog

struct CommentedItem
{
	idStr Name;
	idStr Path;
	idStr Comments;
};

class CPreviewDlg : public CDialogEx
{
public:
	enum { MODELS, GUIS, SOUNDS, MATERIALS, SCRIPTS, SOUNDPARENT, WAVES, PARTICLES, MODELPARENT, GUIPARENT, COMMENTED, SKINS };

	CPreviewDlg( CWnd* pParent = NULL );   // standard constructor
	virtual ~CPreviewDlg();
	void SetOnlyFilter( bool gOnly, idStr filter );
	void SetMode( int mode, const char* preSelect = NULL );
	void RebuildTree( const char* data );
	void SetDisablePreview( bool b )
	{
		disablePreview = b;
	}

	idStr mediaName;
	int returnCode;

	bool Waiting();
	void SetModal();

	enum { IDD = IDD_DIALOG_PREVIEW };
private:
	DECLARE_DYNAMIC( CPreviewDlg )

	CTreeCtrl treeMedia;
	CEdit editInfo;
	HTREEITEM commentItem;
	CImageList m_image;
	CBitmap m_bitmap;
	idGLDrawable m_testDrawable;
	idGLDrawableMaterial m_drawMaterial;
	idGLDrawableModel m_drawModel;
	idGLWidget wndPreview;
	idHashTable<HTREEITEM> quickTree;
	idList<CommentedItem> items;
	virtual BOOL OnInitDialog();
	int currentMode;
	void AddCommentedItems();
	idStr data;
	bool disablePreview;
	bool onlyFilter;
	idStr strOnlyFilter;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support
	void BuildTree();
	void AddStrList( const char* root, const idStrList& list, int type );
	void AddSounds( bool rootItems );
	void AddMaterials( bool rootItems );
	void AddParticles( bool rootItems );
	void AddSkins( bool rootItems );

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTvnSelchangedTreeMedia( NMHDR* pNMHDR, LRESULT* pResult );
	virtual BOOL Create( LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL );
protected:
	virtual void OnCancel();
	virtual void OnOK();
	virtual void OnShowWindow( BOOL bShow, UINT status );
public:
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedPreviewGui();
};
