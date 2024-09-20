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

#ifndef __STAGING_VK__
#define __STAGING_VK__

/*
===========================================================================

idVulkanStagingManager

===========================================================================
*/

struct stagingBuffer_t
{
	stagingBuffer_t() :
		submitted( false ),
		commandBuffer( VK_NULL_HANDLE ),
		buffer( VK_NULL_HANDLE ),
		fence( VK_NULL_HANDLE ),
		offset( 0 ),
		data( NULL ) {}

	bool				submitted;
	VkCommandBuffer		commandBuffer;
	VkBuffer			buffer;
	VkFence				fence;
	VkDeviceSize		offset;
	byte* 				data;

	int stagedCommands;
};

class idVulkanStagingManager
{
public:
	idVulkanStagingManager();
	~idVulkanStagingManager();

	void			Init();
	void			Shutdown();

	byte* 			Stage( const int size, const int alignment, VkCommandBuffer& commandBuffer, VkBuffer& buffer, int& bufferOffset );
	void			Flush();

private:
	void			Wait( stagingBuffer_t& stage );

private:
	int				maxBufferSize;
	int				currentBuffer;
	byte* 			mappedData;
	VkDeviceMemory	memory;
	VkCommandPool	commandPool;

	stagingBuffer_t buffers[ NUM_FRAME_DATA ];
};

extern idVulkanStagingManager stagingManager;

#endif