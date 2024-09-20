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

#include "GEApp.h"
#include "GEMoveModifier.h"

rvGEMoveModifier::rvGEMoveModifier( const char* name, idWindow* window, float x, float y ) :
	rvGEModifier( name, window )
{
	mOldRect = mWrapper->GetClientRect( );

	mNewRect[0] = mOldRect[0] + x;
	mNewRect[1] = mOldRect[1] + y;
	mNewRect[2] = mOldRect[2];
	mNewRect[3] = mOldRect[3];
}

bool rvGEMoveModifier::Merge( rvGEModifier* mergebase )
{
	rvGEMoveModifier* merge = ( rvGEMoveModifier* ) mergebase;

	mNewRect = merge->mNewRect;

	return true;
}

bool rvGEMoveModifier::Apply()
{
	mWrapper->SetRect( mNewRect );

	return true;
}

bool rvGEMoveModifier::Undo()
{
	mWrapper->SetRect( mOldRect );

	return true;
}

bool rvGEMoveModifier::IsValid()
{
	if( !mWindow->GetParent( ) )
	{
		return false;
	}

	return true;
}
