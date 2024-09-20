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
#include "GEKeyValueModifier.h"

rvGEKeyValueModifier::rvGEKeyValueModifier( const char* name, idWindow* window, const char* key, const char* value ) :
	rvGEModifier( name, window ),
	mKey( key ),
	mValue( value )
{
	mUndoValue = mWrapper->GetStateDict().GetString( mKey );
}

bool rvGEKeyValueModifier::Apply()
{
	if( mValue.Length( ) )
	{
		mWrapper->SetStateKey( mKey, mValue );
	}
	else
	{
		mWrapper->DeleteStateKey( mKey );
	}

	return true;
}

bool rvGEKeyValueModifier::Undo()
{
	mWrapper->SetStateKey( mKey, mValue );

	return true;
}

bool rvGEKeyValueModifier::Merge( rvGEModifier* mergebase )
{
	rvGEKeyValueModifier* merge = ( rvGEKeyValueModifier* ) mergebase;

	mValue = merge->mValue;

	return true;
}
