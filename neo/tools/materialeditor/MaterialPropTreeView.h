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
#include "MaterialView.h"

#include "../common/registryoptions.h"
#include "MaterialDef.h"


/**
* View that displays material and stage properties and allows the user to edit the properties.
*/
class MaterialPropTreeView : public CPropTreeView, public MaterialView
{

public:
	virtual				~MaterialPropTreeView();

	void				SetPropertyListType( int listType, int stageNum = -1 );

	void				LoadSettings();
	void				SaveSettings();

	//Material Interface
	virtual void		MV_OnMaterialChange( MaterialDoc* pMaterial );

protected:
	MaterialPropTreeView();
	DECLARE_DYNCREATE( MaterialPropTreeView )

	afx_msg void		OnPropertyChangeNotification( NMHDR* nmhdr, LRESULT* lresult );
	afx_msg void		OnPropertyItemExpanding( NMHDR* nmhdr, LRESULT* lresult );
	DECLARE_MESSAGE_MAP()

	MaterialDef*		FindDefForTreeID( UINT treeID );
	void				RefreshProperties();

protected:

	MaterialDoc*		currentMaterial;
	int					currentListType;
	int					currentStage;
	MaterialDefList*	currentPropDefs;
	rvRegistryOptions	registry;
	bool				internalChange;
};
