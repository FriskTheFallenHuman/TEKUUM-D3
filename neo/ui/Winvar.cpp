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

#include "Window.h"
#include "Winvar.h"
#include "UserInterfaceLocal.h"

idWinVar::idWinVar()
{
	guiDict = NULL;
	name = NULL;
	eval = true;
}

idWinVar::~idWinVar()
{
	delete[] name;
	name = NULL;
	guiDict = NULL;
}

void idWinVar::SetGuiInfo( idDict* gd, const char* _name )
{
	guiDict = gd;
	SetName( _name );
}


void idWinVar::Init( const char* _name, idWindow* win )
{
	idStr key = _name;
	guiDict = NULL;
	int len = key.Length();
	if( len > 5 && key[0] == 'g' && key[1] == 'u' && key[2] == 'i' && key[3] == ':' )
	{
		key = key.Right( len - VAR_GUIPREFIX_LEN );
		SetGuiInfo( win->GetGui()->GetStateDict(), key );
		win->AddUpdateVar( this );
	}
	else
	{
		Set( _name );
	}
}

void idMultiWinVar::Set( const char* val )
{
	for( int i = 0; i < Num(); i++ )
	{
		( *this )[i]->Set( val );
	}
}

void idMultiWinVar::Update()
{
	for( int i = 0; i < Num(); i++ )
	{
		( *this )[i]->Update();
	}
}

void idMultiWinVar::SetGuiInfo( idDict* dict )
{
	for( int i = 0; i < Num(); i++ )
	{
		( *this )[i]->SetGuiInfo( dict, ( *this )[i]->c_str() );
	}
}

