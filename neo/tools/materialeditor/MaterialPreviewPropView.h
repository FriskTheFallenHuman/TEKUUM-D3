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

#include "../common/PropTree/PropTreeView.h"
#include "MaterialPreviewView.h"

// MaterialPreviewPropView view

class MaterialPreviewPropView : public CPropTreeView
{
	DECLARE_DYNCREATE( MaterialPreviewPropView )

protected:
	MaterialPreviewPropView();           // protected constructor used by dynamic creation
	virtual ~MaterialPreviewPropView();

public:
	virtual void OnDraw( CDC* pDC );    // overridden to draw this view

	afx_msg void OnPropertyChangeNotification( NMHDR* nmhdr, LRESULT* lresult );
	afx_msg void OnPropertyButtonClick( NMHDR* nmhdr, LRESULT* lresult );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

	void AddLight();
	void InitializePropTree();

	void RegisterPreviewView( MaterialPreviewView* view );

protected:

	int		numLights;

	MaterialPreviewView*	materialPreview;

	DECLARE_MESSAGE_MAP()
};
