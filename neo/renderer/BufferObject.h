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
#ifndef __BUFFEROBJECT_H__
#define __BUFFEROBJECT_H__

#if defined( USE_VULKAN )
	#include "Vulkan/Allocator_VK.h"
#endif


enum bufferMapType_t
{
	BM_READ,			// map for reading
	BM_WRITE			// map for writing
};

enum bufferUsageType_t
{
	BU_STATIC,			// GPU R
	BU_DYNAMIC,			// GPU R, CPU R/W
};

// Returns all targets to virtual memory use instead of buffer object use.
// Call this before doing any conventional buffer reads, like screenshots.
void UnbindBufferObjects();
bool IsWriteCombined( void* base );
void CopyBuffer( byte* dst, const byte* src, int numBytes );

/*
================================================================================================

idBufferObject

================================================================================================
*/

class idBufferObject
{
public:
	idBufferObject();

	int					GetSize() const
	{
		return ( size & ~MAPPED_FLAG );
	}
	int					GetAllocedSize() const
	{
		return ( ( size & ~MAPPED_FLAG ) + 15 ) & ~15;
	}
	bufferUsageType_t	GetUsage() const
	{
		return usage;
	}
#if defined( USE_VULKAN )
	VkBuffer			GetAPIObject() const
	{
		return apiObject;
	}
#else
	GLintptr			GetAPIObject() const
	{
		return apiObject;
	}
#endif
	int					GetOffset() const
	{
		return ( offsetInOtherBuffer & ~OWNS_BUFFER_FLAG );
	}

	bool				IsMapped() const
	{
		return ( size & MAPPED_FLAG ) != 0;
	}

protected:
	void				SetMapped() const
	{
		const_cast< int& >( size ) |= MAPPED_FLAG;
	}
	void				SetUnmapped() const
	{
		const_cast< int& >( size ) &= ~MAPPED_FLAG;
	}
	bool				OwnsBuffer() const
	{
		return ( ( offsetInOtherBuffer & OWNS_BUFFER_FLAG ) != 0 );
	}

protected:
	int					size;					// size in bytes
	int					offsetInOtherBuffer;	// offset in bytes
	bufferUsageType_t	usage;

#if defined( USE_VULKAN )
	VkBuffer			apiObject;

#if defined( USE_AMD_ALLOCATOR )
	VmaAllocation		vmaAllocation;
	VmaAllocationInfo	allocation;
#else
	vulkanAllocation_t	allocation;
#endif

#else
	// GL
	GLintptr			apiObject;
	void* 				buffer;
#endif

	// sizeof() confuses typeinfo...
	static const int	MAPPED_FLAG			= 1 << ( 4 /* sizeof( int ) */ * 8 - 1 );
	static const int	OWNS_BUFFER_FLAG	= 1 << ( 4 /* sizeof( int ) */ * 8 - 1 );
};

/*
================================================================================================

idVertexBuffer

================================================================================================
*/
class idVertexBuffer : public idBufferObject
{
public:
	idVertexBuffer();
	~idVertexBuffer();

	// Allocate or free the buffer.
	bool				AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage );
	void				FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void				Reference( const idVertexBuffer& other );
	void				Reference( const idVertexBuffer& other, int refOffset, int refSize );

	// Copies data to the buffer. 'size' may be less than the originally allocated size.
	void				Update( const void* data, int size, int offset = 0 ) const;

	void* 				MapBuffer( bufferMapType_t mapType );
	idDrawVert* 		MapVertexBuffer( bufferMapType_t mapType )
	{
		return static_cast< idDrawVert* >( MapBuffer( mapType ) );
	}
	void				UnmapBuffer();

private:
	void				ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idVertexBuffer );
};

/*
================================================================================================

idIndexBuffer

================================================================================================
*/
class idIndexBuffer : public idBufferObject
{
public:
	idIndexBuffer();
	~idIndexBuffer();

	// Allocate or free the buffer.
	bool				AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage );
	void				FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void				Reference( const idIndexBuffer& other );
	void				Reference( const idIndexBuffer& other, int refOffset, int refSize );

	// Copies data to the buffer. 'size' may be less than the originally allocated size.
	void				Update( const void* data, int size, int offset = 0 ) const;

	void* 				MapBuffer( bufferMapType_t mapType );
	triIndex_t* 		MapIndexBuffer( bufferMapType_t mapType )
	{
		return static_cast< triIndex_t* >( MapBuffer( mapType ) );
	}
	void				UnmapBuffer();

private:
	void				ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idIndexBuffer );
};

/*
================================================================================================

idUniformBuffer

IMPORTANT NOTICE: on the PC, binding to an offset in uniform buffer objects
is limited to GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, which is 256 on current nvidia cards,
so joint offsets, which are multiples of 48 bytes, must be in multiples of 16 = 768 bytes.
================================================================================================
*/
class idUniformBuffer : public idBufferObject
{
public:
	idUniformBuffer();
	~idUniformBuffer();

	// Allocate or free the buffer.
	bool				AllocBufferObject( const void* data, int allocSize, bufferUsageType_t usage );
	void				FreeBufferObject();

	// Make this buffer a reference to another buffer.
	void				Reference( const idUniformBuffer& other );
	void				Reference( const idUniformBuffer& other, int refOffset, int refSize );

	// Copies data to the buffer. 'size' may be less than the originally allocated size.
	void				Update( const void* data, int size, int offset = 0 ) const;

	void* 				MapBuffer( bufferMapType_t mapType );
	void				UnmapBuffer();

private:
	void				ClearWithoutFreeing();

	DISALLOW_COPY_AND_ASSIGN( idUniformBuffer );
};

#endif // !__BUFFEROBJECT_H__
