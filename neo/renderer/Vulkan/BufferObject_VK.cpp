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

#include "../RenderCommon.h"
#include "../RenderBackend.h"
#include "../BufferObject.h"
#include "Staging_VK.h"

extern idCVar r_showBuffers;

/*
========================
UnbindBufferObjects
========================
*/
void UnbindBufferObjects()
{

}

/*
================================================================================================

idVertexBuffer

================================================================================================
*/

/*
========================
idVertexBuffer::idVertexBuffer
========================
*/
idVertexBuffer::idVertexBuffer()
{
	SetUnmapped();
}

/*
========================
idVertexBuffer::AllocBufferObject
========================
*/
bool idVertexBuffer::AllocBufferObject( const void* data, int allocSize, bufferUsageType_t _usage )
{
	assert( apiObject == VK_NULL_HANDLE );
	assert_16_byte_aligned( data );

	if( allocSize <= 0 )
	{
		idLib::Error( "idVertexBuffer::AllocBufferObject: allocSize = %i", allocSize );
	}

	size = allocSize;
	usage = _usage;

	bool allocationFailed = false;

	int numBytes = GetAllocedSize();

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.size = numBytes;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if( usage == BU_STATIC )
	{
		bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

#if defined( USE_AMD_ALLOCATOR )
	VmaAllocationCreateInfo vmaReq = {};
	if( usage == BU_STATIC )
	{
		vmaReq.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	}
	else if( usage == BU_DYNAMIC )
	{
#if defined(__APPLE__)
		// SRS - VMA_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_ONLY;
#else
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
#endif
		vmaReq.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	}

	ID_VK_CHECK( vmaCreateBuffer( vmaAllocator, &bufferCreateInfo, &vmaReq, &apiObject, &vmaAllocation, &allocation ) );

#else
	VkResult ret = vkCreateBuffer( vkcontext.device, &bufferCreateInfo, NULL, &apiObject );
	assert( ret == VK_SUCCESS );

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements( vkcontext.device, apiObject, &memoryRequirements );

#if defined(__APPLE__)
	// SRS - VULKAN_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_ONLY;
#else
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_TO_GPU;
#endif

	allocation = vulkanAllocator.Allocate(
					 memoryRequirements.size,
					 memoryRequirements.alignment,
					 memoryRequirements.memoryTypeBits,
					 memUsage,
					 VULKAN_ALLOCATION_TYPE_BUFFER );

	ID_VK_CHECK( vkBindBufferMemory( vkcontext.device, apiObject, allocation.deviceMemory, allocation.offset ) );
#endif

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "vertex buffer alloc %p, (%i bytes)\n", this, GetSize() );
	}

	// copy the data
	if( data != NULL )
	{
		Update( data, allocSize );
	}

	return !allocationFailed;
}

/*
========================
idVertexBuffer::FreeBufferObject
========================
*/
void idVertexBuffer::FreeBufferObject()
{
	if( IsMapped() )
	{
		UnmapBuffer();
	}

	// if this is a sub-allocation inside a larger buffer, don't actually free anything.
	if( OwnsBuffer() == false )
	{
		ClearWithoutFreeing();
		return;
	}

	if( apiObject == VK_NULL_HANDLE )
	{
		return;
	}

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "vertex buffer free %p, (%i bytes)\n", this, GetSize() );
	}

	if( apiObject != VK_NULL_HANDLE )
	{
#if defined( USE_AMD_ALLOCATOR )
		vmaDestroyBuffer( vmaAllocator, apiObject, vmaAllocation );
		apiObject = VK_NULL_HANDLE;
		allocation = VmaAllocationInfo();
		vmaAllocation = NULL;
#else
		vulkanAllocator.Free( allocation );
		vkDestroyBuffer( vkcontext.device, apiObject, NULL );
		apiObject = VK_NULL_HANDLE;
		allocation = vulkanAllocation_t();
#endif
	}

	ClearWithoutFreeing();
}

/*
========================
idVertexBuffer::Update
========================
*/
void idVertexBuffer::Update( const void* data, int size, int offset ) const
{
	assert( apiObject != VK_NULL_HANDLE );
	assert_16_byte_aligned( data );
	assert( ( GetOffset() & 15 ) == 0 );

	if( size > GetSize() )
	{
		idLib::FatalError( "idVertexBuffer::Update: size overrun, %i > %i\n", size, GetSize() );
	}

	if( usage == BU_DYNAMIC )
	{
		CopyBuffer(
#if defined( USE_AMD_ALLOCATOR )
			( byte* )allocation.pMappedData + GetOffset() + offset,
#else
			allocation.data + GetOffset() + offset,
#endif
			( const byte* )data, size );
	}
	else
	{
		VkBuffer stageBuffer;
		VkCommandBuffer commandBuffer;
		int stageOffset = 0;
		byte* stageData = stagingManager.Stage( size, 1, commandBuffer, stageBuffer, stageOffset );

		memcpy( stageData, data, size );

		VkBufferCopy bufferCopy = {};
		bufferCopy.srcOffset = stageOffset;
		bufferCopy.dstOffset = GetOffset() + offset;
		bufferCopy.size = size;

		vkCmdCopyBuffer( commandBuffer, stageBuffer, apiObject, 1, &bufferCopy );
	}
}

/*
========================
idVertexBuffer::MapBuffer
========================
*/
void* idVertexBuffer::MapBuffer( bufferMapType_t mapType )
{
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idVertexBuffer::MapBuffer: Cannot map a buffer marked as BU_STATIC." );
	}

#if defined( USE_AMD_ALLOCATOR )
	void* buffer = ( byte* )allocation.pMappedData + GetOffset();
#else
	void* buffer = allocation.data + GetOffset();
#endif

	SetMapped();

	if( buffer == NULL )
	{
		idLib::FatalError( "idVertexBuffer::MapBuffer: failed" );
	}
	return buffer;
}

/*
========================
idVertexBuffer::UnmapBuffer
========================
*/
void idVertexBuffer::UnmapBuffer()
{
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idVertexBuffer::UnmapBuffer: Cannot unmap a buffer marked as BU_STATIC." );
	}

	SetUnmapped();
}

/*
========================
idVertexBuffer::ClearWithoutFreeing
========================
*/
void idVertexBuffer::ClearWithoutFreeing()
{
	size = 0;
	offsetInOtherBuffer = OWNS_BUFFER_FLAG;
	apiObject = VK_NULL_HANDLE;
#if defined( USE_AMD_ALLOCATOR )
	allocation = VmaAllocationInfo();
	vmaAllocation = NULL;
#else
	allocation.deviceMemory = VK_NULL_HANDLE;
#endif
}

/*
================================================================================================

idIndexBuffer

================================================================================================
*/

/*
========================
idIndexBuffer::idIndexBuffer
========================
*/
idIndexBuffer::idIndexBuffer()
{
	SetUnmapped();
}

/*
========================
idIndexBuffer::AllocBufferObject
========================
*/
bool idIndexBuffer::AllocBufferObject( const void* data, int allocSize, bufferUsageType_t _usage )
{
	assert( apiObject == VK_NULL_HANDLE );
	assert_16_byte_aligned( data );

	if( allocSize <= 0 )
	{
		idLib::Error( "idIndexBuffer::AllocBufferObject: allocSize = %i", allocSize );
	}

	size = allocSize;
	usage = _usage;

	bool allocationFailed = false;

	int numBytes = GetAllocedSize();

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.size = numBytes;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	if( usage == BU_STATIC )
	{
		bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

#if defined( USE_AMD_ALLOCATOR )
	VmaAllocationCreateInfo vmaReq = {};
	if( usage == BU_STATIC )
	{
		vmaReq.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	}
	else if( usage == BU_DYNAMIC )
	{
#if defined(__APPLE__)
		// SRS - VMA_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_ONLY;
#else
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
#endif
		vmaReq.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	}

	ID_VK_CHECK( vmaCreateBuffer( vmaAllocator, &bufferCreateInfo, &vmaReq, &apiObject, &vmaAllocation, &allocation ) );

#else
	VkResult ret = vkCreateBuffer( vkcontext.device, &bufferCreateInfo, NULL, &apiObject );
	assert( ret == VK_SUCCESS );

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements( vkcontext.device, apiObject, &memoryRequirements );

#if defined(__APPLE__)
	// SRS - VULKAN_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_ONLY;
#else
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_TO_GPU;
#endif

	allocation = vulkanAllocator.Allocate(
					 memoryRequirements.size,
					 memoryRequirements.alignment,
					 memoryRequirements.memoryTypeBits,
					 memUsage,
					 VULKAN_ALLOCATION_TYPE_BUFFER );

	ID_VK_CHECK( vkBindBufferMemory( vkcontext.device, apiObject, allocation.deviceMemory, allocation.offset ) );
#endif

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "index buffer alloc %p, (%i bytes)\n", this, GetSize() );
	}

	// copy the data
	if( data != NULL )
	{
		Update( data, allocSize );
	}

	return !allocationFailed;
}

/*
========================
idIndexBuffer::FreeBufferObject
========================
*/
void idIndexBuffer::FreeBufferObject()
{
	if( IsMapped() )
	{
		UnmapBuffer();
	}

	// if this is a sub-allocation inside a larger buffer, don't actually free anything.
	if( OwnsBuffer() == false )
	{
		ClearWithoutFreeing();
		return;
	}

	if( apiObject == VK_NULL_HANDLE )
	{
		return;
	}

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "index buffer free %p, (%i bytes)\n", this, GetSize() );
	}

	if( apiObject != VK_NULL_HANDLE )
	{
#if defined( USE_AMD_ALLOCATOR )
		vmaDestroyBuffer( vmaAllocator, apiObject, vmaAllocation );
		apiObject = VK_NULL_HANDLE;
		allocation = VmaAllocationInfo();
		vmaAllocation = NULL;
#else
		vulkanAllocator.Free( allocation );
		vkDestroyBuffer( vkcontext.device, apiObject, NULL );
		apiObject = VK_NULL_HANDLE;
		allocation = vulkanAllocation_t();
#endif
	}

	ClearWithoutFreeing();
}

/*
========================
idIndexBuffer::Update
========================
*/
void idIndexBuffer::Update( const void* data, int size, int offset ) const
{
	assert( apiObject != VK_NULL_HANDLE );
	assert_16_byte_aligned( data );
	assert( ( GetOffset() & 15 ) == 0 );

	if( size > GetSize() )
	{
		idLib::FatalError( "idIndexBuffer::Update: size overrun, %i > %i\n", size, GetSize() );
	}

	if( usage == BU_DYNAMIC )
	{
		CopyBuffer(
#if defined( USE_AMD_ALLOCATOR )
			( byte* )allocation.pMappedData + GetOffset() + offset,
#else
			allocation.data + GetOffset() + offset,
#endif
			( const byte* )data, size );
	}
	else
	{
		VkBuffer stageBuffer;
		VkCommandBuffer commandBuffer;
		int stageOffset = 0;
		byte* stageData = stagingManager.Stage( size, 1, commandBuffer, stageBuffer, stageOffset );

		memcpy( stageData, data, size );

		VkBufferCopy bufferCopy = {};
		bufferCopy.srcOffset = stageOffset;
		bufferCopy.dstOffset = GetOffset() + offset;
		bufferCopy.size = size;

		vkCmdCopyBuffer( commandBuffer, stageBuffer, apiObject, 1, &bufferCopy );
	}
}

/*
========================
idIndexBuffer::MapBuffer
========================
*/
void* idIndexBuffer::MapBuffer( bufferMapType_t mapType )
{
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idIndexBuffer::MapBuffer: Cannot map a buffer marked as BU_STATIC." );
	}

#if defined( USE_AMD_ALLOCATOR )
	void* buffer = ( byte* )allocation.pMappedData + GetOffset();
#else
	void* buffer = allocation.data + GetOffset();
#endif

	SetMapped();

	if( buffer == NULL )
	{
		idLib::FatalError( "idIndexBuffer::MapBuffer: failed" );
	}
	return buffer;
}

/*
========================
idIndexBuffer::UnmapBuffer
========================
*/
void idIndexBuffer::UnmapBuffer()
{
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idIndexBuffer::UnmapBuffer: Cannot unmap a buffer marked as BU_STATIC." );
	}

	SetUnmapped();
}

/*
========================
idIndexBuffer::ClearWithoutFreeing
========================
*/
void idIndexBuffer::ClearWithoutFreeing()
{
	size = 0;
	offsetInOtherBuffer = OWNS_BUFFER_FLAG;
	apiObject = VK_NULL_HANDLE;
#if defined( USE_AMD_ALLOCATOR )
	allocation = VmaAllocationInfo();
	vmaAllocation = NULL;
#else
	allocation.deviceMemory = VK_NULL_HANDLE;
#endif
}

/*
================================================================================================

idUniformBuffer

================================================================================================
*/

/*
========================
idUniformBuffer::idUniformBuffer
========================
*/
idUniformBuffer::idUniformBuffer()
{
	usage = BU_DYNAMIC;
	SetUnmapped();
}

/*
========================
idUniformBuffer::AllocBufferObject
========================
*/
bool idUniformBuffer::AllocBufferObject( const void* data, int allocSize, bufferUsageType_t _usage )
{
	assert( apiObject == VK_NULL_HANDLE );
	assert_16_byte_aligned( data );

	if( allocSize <= 0 )
	{
		idLib::Error( "idUniformBuffer::AllocBufferObject: allocSize = %i", allocSize );
	}

	size = allocSize;
	usage = _usage;

	bool allocationFailed = false;

	const int numBytes = GetAllocedSize();

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.size = numBytes;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	if( usage == BU_STATIC )
	{
		bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

#if defined( USE_AMD_ALLOCATOR )
	VmaAllocationCreateInfo vmaReq = {};
	if( usage == BU_STATIC )
	{
		vmaReq.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	}
	else if( usage == BU_DYNAMIC )
	{
#if defined(__APPLE__)
		// SRS - VMA_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_ONLY;
#else
		vmaReq.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
#endif
		vmaReq.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	}

	ID_VK_CHECK( vmaCreateBuffer( vmaAllocator, &bufferCreateInfo, &vmaReq, &apiObject, &vmaAllocation, &allocation ) );

#else
	VkResult ret = vkCreateBuffer( vkcontext.device, &bufferCreateInfo, NULL, &apiObject );
	assert( ret == VK_SUCCESS );

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements( vkcontext.device, apiObject, &memoryRequirements );

#if defined(__APPLE__)
	// SRS - VULKAN_MEMORY_USAGE_CPU_ONLY required for BU_DYNAMIC host coherency on OSX, otherwise black screen
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_ONLY;
#else
	vulkanMemoryUsage_t memUsage = ( usage == BU_STATIC ) ? VULKAN_MEMORY_USAGE_GPU_ONLY : VULKAN_MEMORY_USAGE_CPU_TO_GPU;
#endif

	allocation = vulkanAllocator.Allocate(
					 memoryRequirements.size,
					 memoryRequirements.alignment,
					 memoryRequirements.memoryTypeBits,
					 memUsage,
					 VULKAN_ALLOCATION_TYPE_BUFFER );

	ID_VK_CHECK( vkBindBufferMemory( vkcontext.device, apiObject, allocation.deviceMemory, allocation.offset ) );
#endif

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "joint buffer alloc %p, (%i bytes)\n", this, GetSize() );
	}

	// copy the data
	if( data != NULL )
	{
		Update( data, allocSize );
	}

	return !allocationFailed;
}

/*
========================
idUniformBuffer::FreeBufferObject
========================
*/
void idUniformBuffer::FreeBufferObject()
{
	if( IsMapped() )
	{
		UnmapBuffer();
	}

	// if this is a sub-allocation inside a larger buffer, don't actually free anything.
	if( OwnsBuffer() == false )
	{
		ClearWithoutFreeing();
		return;
	}

	if( apiObject == VK_NULL_HANDLE )
	{
		return;
	}

	if( r_showBuffers.GetBool() )
	{
		idLib::Printf( "joint buffer free %p, (%i bytes)\n", this, GetSize() );
	}

	if( apiObject != VK_NULL_HANDLE )
	{
#if defined( USE_AMD_ALLOCATOR )
		vmaDestroyBuffer( vmaAllocator, apiObject, vmaAllocation );
		apiObject = VK_NULL_HANDLE;
		allocation = VmaAllocationInfo();
		vmaAllocation = NULL;
#else
		vulkanAllocator.Free( allocation );
		vkDestroyBuffer( vkcontext.device, apiObject, NULL );
		apiObject = VK_NULL_HANDLE;
		allocation = vulkanAllocation_t();
#endif
	}

	ClearWithoutFreeing();
}

/*
========================
idUniformBuffer::Update
========================
*/
void idUniformBuffer::Update( const void* data, int size, int offset ) const
{
	assert( apiObject != VK_NULL_HANDLE );
	assert_16_byte_aligned( data );
	assert( ( GetOffset() & 15 ) == 0 );

	if( size > GetSize() )
	{
		idLib::FatalError( "idUniformBuffer::Update: size overrun, %i > %i\n", size, size );
	}

	if( usage == BU_DYNAMIC )
	{
		CopyBuffer(
#if defined( USE_AMD_ALLOCATOR )
			( byte* )allocation.pMappedData + GetOffset() + offset,
#else
			allocation.data + GetOffset() + offset,
#endif
			( const byte* )data, size );
	}
	else
	{
		VkBuffer stageBuffer;
		VkCommandBuffer commandBuffer;
		int stageOffset = 0;
		byte* stageData = stagingManager.Stage( size, 1, commandBuffer, stageBuffer, stageOffset );

		memcpy( stageData, data, size );

		VkBufferCopy bufferCopy = {};
		bufferCopy.srcOffset = stageOffset;
		bufferCopy.dstOffset = GetOffset() + offset;
		bufferCopy.size = size;

		vkCmdCopyBuffer( commandBuffer, stageBuffer, apiObject, 1, &bufferCopy );
	}
}

/*
========================
idUniformBuffer::MapBuffer
========================
*/
void* idUniformBuffer::MapBuffer( bufferMapType_t mapType )
{
	assert( mapType == BM_WRITE );
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idUniformBuffer::MapBuffer: Cannot map a buffer marked as BU_STATIC." );
	}

#if defined( USE_AMD_ALLOCATOR )
	void* buffer = ( byte* )allocation.pMappedData + GetOffset();
#else
	void* buffer = allocation.data + GetOffset();
#endif

	SetMapped();

	if( buffer == NULL )
	{
		idLib::FatalError( "idUniformBuffer::MapBuffer: failed" );
	}
	return buffer;
}

/*
========================
idUniformBuffer::UnmapBuffer
========================
*/
void idUniformBuffer::UnmapBuffer()
{
	assert( apiObject != VK_NULL_HANDLE );

	if( usage == BU_STATIC )
	{
		idLib::FatalError( "idUniformBuffer::UnmapBuffer: Cannot unmap a buffer marked as BU_STATIC." );
	}

	SetUnmapped();
}

/*
========================
idUniformBuffer::ClearWithoutFreeing
========================
*/
void idUniformBuffer::ClearWithoutFreeing()
{
	size = 0;
	offsetInOtherBuffer = OWNS_BUFFER_FLAG;
	apiObject = VK_NULL_HANDLE;
#if defined( USE_AMD_ALLOCATOR )
	allocation = VmaAllocationInfo();
	vmaAllocation = NULL;
#else
	allocation.deviceMemory = VK_NULL_HANDLE;
#endif
}
