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

#include "Game_local.h"

/*
================
idWorldspawn

Worldspawn class.  Each map has one worldspawn which handles global spawnargs.
Every map should have exactly one worldspawn.
================
*/
CLASS_DECLARATION( idEntity, idWorldspawn )
EVENT( EV_Remove,				idWorldspawn::Event_Remove )
EVENT( EV_SafeRemove,			idWorldspawn::Event_Remove )
END_CLASS

/*
================
idWorldspawn::Spawn
================
*/
void idWorldspawn::Spawn()
{
	idStr				scriptname;
	idThread*			thread;
	const function_t*	func;
	const idKeyValue*	kv;

	assert( gameLocal.world == NULL );
	gameLocal.world = this;

	g_gravity.SetFloat( spawnArgs.GetFloat( "gravity", va( "%f", DEFAULT_GRAVITY ) ) );

	// disable stamina on hell levels
	if( spawnArgs.GetBool( "no_stamina" ) )
	{
		pm_stamina.SetFloat( 0.0f );
	}

	// load script
	scriptname = gameLocal.GetMapName();
	scriptname.SetFileExtension( ".script" );
	if( fileSystem->ReadFile( scriptname, NULL, NULL ) > 0 )
	{
		gameLocal.program.CompileFile( scriptname );

		// call the main function by default
		func = gameLocal.program.FindFunction( "main" );
		if( func != NULL )
		{
			thread = new idThread( func );
			thread->DelayedStart( 0 );
		}
	}

	// call any functions specified in worldspawn
	kv = spawnArgs.MatchPrefix( "call" );
	while( kv != NULL )
	{
		func = gameLocal.program.FindFunction( kv->GetValue() );
		if( func == NULL )
		{
			gameLocal.Error( "Function '%s' not found in script for '%s' key on worldspawn", kv->GetValue().c_str(), kv->GetKey().c_str() );
		}

		thread = new idThread( func );
		thread->DelayedStart( 0 );
		kv = spawnArgs.MatchPrefix( "call", kv );
	}
}

/*
=================
idWorldspawn::Save
=================
*/
void idWorldspawn::Save( idRestoreGame* savefile )
{
}

/*
=================
idWorldspawn::Restore
=================
*/
void idWorldspawn::Restore( idRestoreGame* savefile )
{
	assert( gameLocal.world == this );

	g_gravity.SetFloat( spawnArgs.GetFloat( "gravity", va( "%f", DEFAULT_GRAVITY ) ) );

	// disable stamina on hell levels
	if( spawnArgs.GetBool( "no_stamina" ) )
	{
		pm_stamina.SetFloat( 0.0f );
	}
}

/*
================
idWorldspawn::~idWorldspawn
================
*/
idWorldspawn::~idWorldspawn()
{
	if( gameLocal.world == this )
	{
		gameLocal.world = NULL;
	}
}

/*
================
idWorldspawn::Event_Remove
================
*/
void idWorldspawn::Event_Remove()
{
	gameLocal.Error( "Tried to remove world" );
}
