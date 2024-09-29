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

#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

#include "sys/sys_defines.h"
#include "sys/sys_builddefines.h"
#include "sys/sys_includes.h"
#include "sys/sys_assert.h"
#include "sys/sys_types.h"
#include "sys/sys_intrinsics.h"
#include "sys/sys_threading.h"

//-----------------------------------------------------

#define ID_TIME_T time_t // Signed because -1 means "File not found" and we don't want that to compare > than any other time

// non-portable system services
#include "../sys/sys_public.h"

// id lib
#include "../idlib/Lib.h"

#include "sys/sys_filesystem.h"

// framework
#include "../framework/BuildVersion.h"
#include "../framework/BuildDefines.h"
#include "../framework/Licensee.h"
#include "../framework/CmdSystem.h"
#include "../framework/CVarSystem.h"
#include "../framework/Common.h"
// DG: needed for idFile_InZip in File.h
#include "../framework/Unzip.h"
// DG end
#include "../framework/File.h"
#include "../framework/FileSystem.h"
#include "../framework/UsercmdGen.h"

// decls
#include "../framework/TokenParser.h"
#include "../framework/DeclManager.h"
#include "../framework/DeclTable.h"
#include "../framework/DeclSkin.h"
#include "../framework/DeclEntityDef.h"
#include "../framework/DeclFX.h"
#include "../framework/DeclParticle.h"
#include "../framework/DeclAF.h"
#include "../framework/DeclPDA.h"

// We have expression parsing and evaluation code in multiple places:
// materials, sound shaders, and guis. We should unify them.
const int MAX_EXPRESSION_OPS = 4096;
const int MAX_EXPRESSION_REGISTERS = 4096;

// renderer
// everything that is needed by the backend needs
// to be double buffered to allow it to run in
// parallel on a dual cpu machine
const uint32_t NUM_FRAME_DATA = 2;

#if defined(USE_VULKAN)
	#include "../renderer/Vulkan/qvk.h"
#else
	// RB: replaced QGL with GLEW
	#include <GL/glew.h>
	// RB end
#endif
#include "../renderer/Cinematic.h"
#include "../renderer/Material.h"
#include "../renderer/BufferObject.h"
#include "../renderer/VertexCache.h"
#include "../renderer/Model.h"
#include "../renderer/ModelManager.h"
#include "../renderer/RenderSystem.h"
#include "../renderer/RenderWorld.h"
#include "../renderer/DeviceContext.h"

// sound engine
#include "../sound/sound.h"

// asynchronous networking
#include "../framework/async/NetworkSystem.h"

// user interfaces
#include "../ui/ListGUI.h"
#include "../ui/UserInterface.h"

// collision detection system
#include "../cm/CollisionModel.h"

// AAS files and manager
#include "../aas/AASFile.h"
#include "../aas/AASFileManager.h"

// MayaImport
#include "../MayaImport/maya_main.h"

// game
#include "../framework/Game.h"

//-----------------------------------------------------

#ifndef _D3SDK

	#ifdef GAME_DLL

		#if defined(_D3XP)
			#include "../d3xp/Game_local.h"
		#else
			#include "../game/Game_local.h"
		#endif

	#else

		#include "../framework/DemoChecksum.h"

		// framework
		#include "../framework/Compressor.h"
		#include "../framework/EventLoop.h"
		#include "../framework/KeyInput.h"
		#include "../framework/EditField.h"
		#include "../framework/DebugGraph.h"
		#include "../framework/Console.h"
		#include "../framework/DemoFile.h"
		#include "../framework/Session.h"

		// asynchronous networking
		#include "../framework/async/AsyncNetwork.h"

		// The editor entry points are always declared, but may just be
		// stubbed out on non-windows platforms.
		#ifdef ID_ALLOW_TOOLS
			#include "../tools/edit_public.h"
		#endif

		// Compilers for map, model, video etc. processing.
		#if defined( ID_ALLOW_CMD_TOOLS ) || defined( ID_ALLOW_TOOLS )
			#include "../tools/compilers/compiler_public.h"
		#endif

	#endif /* !GAME_DLL */

#endif /* !_D3SDK */

//-----------------------------------------------------
/*
#undef min
#undef max
#include <algorithm>	// for min / max / swap
*/

#endif /* !__PRECOMPILED_H__ */
