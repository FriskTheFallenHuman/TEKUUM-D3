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

#include "MEOptions.h"

/**
* Constructor for MEOptions.
*/
MEOptions::MEOptions( )
{

	registry.Init( "Software\\id Software\\DOOM3\\Tools\\MaterialEditor" );

	materialTreeWidth = 0;
	stageWidth = 0;
	previewPropertiesWidth = 0;
	materialEditHeight = 0;
	materialPropHeadingWidth = 0;
	previewPropHeadingWidth = 0;

}

/**
* Destructor for MEOptions.
*/
MEOptions::~MEOptions()
{
}

/**
* Saves the material editor options to the registry.
*/
bool MEOptions::Save()
{

	registry.SetFloat( "materialTreeWidth", materialTreeWidth );
	registry.SetFloat( "stageWidth", stageWidth );
	registry.SetFloat( "previewPropertiesWidth", previewPropertiesWidth );
	registry.SetFloat( "materialEditHeight", materialEditHeight );
	registry.SetFloat( "materialPropHeadingWidth", materialPropHeadingWidth );
	registry.SetFloat( "previewPropHeadingWidth", previewPropHeadingWidth );

	return registry.Save();
}

/**
* Loads the material editor options from the registry.
*/
bool MEOptions::Load()
{

	if( !registry.Load() )
	{
		return false;
	}

	materialTreeWidth = ( int )registry.GetFloat( "materialTreeWidth" );
	stageWidth = ( int )registry.GetFloat( "stageWidth" );
	previewPropertiesWidth = ( int )registry.GetFloat( "previewPropertiesWidth" );
	materialEditHeight = ( int )registry.GetFloat( "materialEditHeight" );
	materialPropHeadingWidth = ( int )registry.GetFloat( "materialPropHeadingWidth" );
	previewPropHeadingWidth = ( int )registry.GetFloat( "previewPropHeadingWidth" );

	return true;

}