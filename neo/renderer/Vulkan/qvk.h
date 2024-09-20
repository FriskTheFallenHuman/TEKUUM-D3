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

#ifndef __QVK_H__
#define __QVK_H__

#if defined( USE_VULKAN )

#if defined(VK_USE_PLATFORM_WIN32_KHR)  //_WIN32
	#include <Windows.h>
#endif

#define USE_AMD_ALLOCATOR

#include <vulkan/vulkan.h>

#if defined( USE_AMD_ALLOCATOR )
	#include "vma.h"
#endif

#define ID_VK_CHECK( x ) { \
	VkResult ret = x; \
	if ( ret != VK_SUCCESS ) idLib::FatalError( "VK: %s - %s", VK_ErrorToString( ret ), #x ); \
}

#define ID_VK_VALIDATE( x, msg ) { \
	if ( !( x ) ) idLib::FatalError( "VK: %s - %s", msg, #x ); \
}

const char* VK_ErrorToString( VkResult result );


static const int MAX_DESC_SETS				= 16384;
static const int MAX_DESC_UNIFORM_BUFFERS	= 8192;
static const int MAX_DESC_IMAGE_SAMPLERS	= 12384;
static const int MAX_DESC_SET_WRITES		= 32;
static const int MAX_DESC_SET_UNIFORMS		= 48;
static const int MAX_IMAGE_PARMS			= 16;
static const int MAX_UBO_PARMS				= 2;
static const int NUM_TIMESTAMP_QUERIES		= 32;

// VK_EXT_debug_marker
extern PFN_vkDebugMarkerSetObjectTagEXT		qvkDebugMarkerSetObjectTagEXT;
extern PFN_vkDebugMarkerSetObjectNameEXT	qvkDebugMarkerSetObjectNameEXT;
extern PFN_vkCmdDebugMarkerBeginEXT			qvkCmdDebugMarkerBeginEXT;
extern PFN_vkCmdDebugMarkerEndEXT			qvkCmdDebugMarkerEndEXT;
extern PFN_vkCmdDebugMarkerInsertEXT		qvkCmdDebugMarkerInsertEXT;

// VK_EXT_debug_utils
extern PFN_vkQueueBeginDebugUtilsLabelEXT	qvkQueueBeginDebugUtilsLabelEXT;
extern PFN_vkQueueEndDebugUtilsLabelEXT		qvkQueueEndDebugUtilsLabelEXT;
extern PFN_vkCmdBeginDebugUtilsLabelEXT		qvkCmdBeginDebugUtilsLabelEXT;
extern PFN_vkCmdEndDebugUtilsLabelEXT		qvkCmdEndDebugUtilsLabelEXT;
extern PFN_vkCmdInsertDebugUtilsLabelEXT	qvkCmdInsertDebugUtilsLabelEXT;

#endif

#endif
