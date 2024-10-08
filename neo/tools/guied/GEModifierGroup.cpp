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

#include "GEModifierGroup.h"

rvGEModifierGroup::rvGEModifierGroup( ) :
	rvGEModifier( "Group", NULL )
{
}

rvGEModifierGroup::~rvGEModifierGroup( )
{
	int i;

	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		delete mModifiers[i];
	}

	mModifiers.Clear( );
}

bool rvGEModifierGroup::Append( rvGEModifier* mod )
{
	// All modifiers must be the same type
	assert( !mModifiers.Num() || !idStr::Icmp( mod->GetName( ), mModifiers[0]->GetName( ) ) );

	if( !mModifiers.Num( ) )
	{
		mName = mod->GetName( );
	}

	mModifiers.Append( mod );
	return true;
}

bool rvGEModifierGroup::IsValid()
{
	int i;

	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		if( !mModifiers[i]->IsValid( ) )
		{
			return false;
		}
	}

	return true;
}

bool rvGEModifierGroup::Apply()
{
	int i;

	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		mModifiers[i]->Apply( );
	}

	return true;
}

bool rvGEModifierGroup::Undo()
{
	int i;

	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		mModifiers[i]->Undo( );
	}

	return true;
}

bool rvGEModifierGroup::CanMerge( rvGEModifier* mergebase )
{
	rvGEModifierGroup*	merge = ( rvGEModifierGroup* ) mergebase;
	int					i;

	if( mModifiers.Num() != merge->mModifiers.Num( ) )
	{
		return false;
	}

	// Double check the merge is possible
	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		if( mModifiers[i]->GetWindow() != merge->mModifiers[i]->GetWindow() )
		{
			return false;
		}

		if( idStr::Icmp( mModifiers[i]->GetName( ), merge->mModifiers[i]->GetName( ) ) )
		{
			return false;
		}

		if( !mModifiers[i]->CanMerge( merge->mModifiers[i] ) )
		{
			return false;
		}
	}

	return true;
}

bool rvGEModifierGroup::Merge( rvGEModifier* mergebase )
{
	rvGEModifierGroup*	merge = ( rvGEModifierGroup* ) mergebase;
	int					i;

	// Double check the merge is possible
	for( i = 0; i < mModifiers.Num(); i ++ )
	{
		mModifiers[i]->Merge( merge->mModifiers[i] );
	}

	return true;
}
