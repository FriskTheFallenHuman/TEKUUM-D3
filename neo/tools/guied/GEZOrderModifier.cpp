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
#include "GEZOrderModifier.h"


rvGEZOrderModifier::rvGEZOrderModifier( const char* name, idWindow* window, EZOrderChange change ) :
	rvGEModifier( name, window )
{
	int			count;
	int			index;
	idWindow*	parent;

	parent = window->GetParent( );
	if( !parent )
	{
		return;
	}

	count = parent->GetChildCount( );
	index = parent->GetChildIndex( mWindow );

	if( index + 1 >= count )
	{
		mUndoBefore = NULL;
	}
	else
	{
		mUndoBefore = parent->GetChild( index + 1 );
	}

	switch( change )
	{
		case ZO_FORWARD:
			index += 2;
			break;

		case ZO_BACKWARD:
			if( index == 0 )
			{
				index = 1;
			}
			else
			{
				index -= 1;
			}
			break;

		case ZO_BACK:
			index = 0;
			break;

		case ZO_FRONT:
			index = count;
			break;
	}

	if( index >= count )
	{
		mBefore = NULL;
	}
	else
	{
		mBefore = parent->GetChild( index );
	}
}

bool rvGEZOrderModifier::Apply()
{
	idWindow* parent;

	parent = mWindow->GetParent( );

	parent->RemoveChild( mWindow );
	parent->InsertChild( mWindow, mBefore );

	return true;
}

bool rvGEZOrderModifier::Undo()
{
	idWindow* parent;

	parent = mWindow->GetParent( );

	parent->RemoveChild( mWindow );
	parent->InsertChild( mWindow, mUndoBefore );

	return true;
}

bool rvGEZOrderModifier::IsValid()
{
	if( !mWindow->GetParent( ) )
	{
		return false;
	}

	return true;
}
