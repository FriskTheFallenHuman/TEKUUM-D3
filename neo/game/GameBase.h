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

#ifndef __GAMEBASE_H__
#define	__GAMEBASE_H__

// default scripts
#define SCRIPT_DEFAULTDEFS		"script/doom_defs.script"
#define SCRIPT_DEFAULT			"script/doom_main.script"
#define SCRIPT_DEFAULTFUNC		"doom_main"

#define LAGO_IMG_WIDTH			64
#define LAGO_IMG_HEIGHT			64
#define LAGO_WIDTH				64
#define LAGO_HEIGHT				44
#define LAGO_MATERIAL			"textures/sfx/lagometer"
#define LAGO_IMAGE				"textures/sfx/lagometer.tga"

// if set to 1 the server sends the client PVS with snapshots and the client compares against what it sees
#ifndef ASYNC_WRITE_PVS
	#define ASYNC_WRITE_PVS 0
#endif

// the "gameversion" client command will print this plus compile date
#define	GAME_VERSION			"baseDOOM-1"

#define	MAX_CLIENTS				32
#define	GENTITYNUM_BITS			12
#define	MAX_GENTITIES			(1<<GENTITYNUM_BITS)
#define	ENTITYNUM_NONE			(MAX_GENTITIES-1)
#define	ENTITYNUM_WORLD			(MAX_GENTITIES-2)
#define	ENTITYNUM_MAX_NORMAL	(MAX_GENTITIES-2)

#define MAX_GAME_MESSAGE_SIZE	8192
#define MAX_ENTITY_STATE_SIZE	512
#define ENTITY_PVS_SIZE			((MAX_GENTITIES+31)>>5)

// content masks
#define	MASK_ALL				(-1)
#define	MASK_SOLID				(CONTENTS_SOLID)
#define	MASK_MONSTERSOLID		(CONTENTS_SOLID|CONTENTS_MONSTERCLIP|CONTENTS_BODY)
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define	MASK_WATER				(CONTENTS_WATER)
#define	MASK_OPAQUE				(CONTENTS_OPAQUE)
#define	MASK_SHOT_RENDERMODEL	(CONTENTS_SOLID|CONTENTS_RENDERMODEL)
#define	MASK_SHOT_BOUNDINGBOX	(CONTENTS_SOLID|CONTENTS_BODY)

#define DEFAULT_GRAVITY_STRING	"1066"

extern void gameError( const char* fmt, ... );

#endif
