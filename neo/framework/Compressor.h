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

#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__

/*
===============================================================================

	idCompressor is a layer ontop of idFile which provides lossless data
	compression. The compressor can be used as a regular file and multiple
	compressors can be stacked ontop of each other.

===============================================================================
*/

class idCompressor : public idFile
{
public:
	// compressor allocation
	static idCompressor* 	AllocNoCompression();
	static idCompressor* 	AllocBitStream();
	static idCompressor* 	AllocRunLength();
	static idCompressor* 	AllocRunLength_ZeroBased();
	static idCompressor* 	AllocHuffman();
	static idCompressor* 	AllocArithmetic();
	static idCompressor* 	AllocLZSS();
	static idCompressor* 	AllocLZSS_WordAligned();
	static idCompressor* 	AllocLZW();

	// initialization
	virtual void			Init( idFile* f, bool compress, int wordLength ) = 0;
	virtual void			FinishCompress() = 0;
	virtual float			GetCompressionRatio() const = 0;

	// common idFile interface
	virtual const char* 	GetName() = 0;
	virtual const char* 	GetFullPath() = 0;
	virtual int				Read( void* outData, int outLength ) = 0;
	virtual int				Write( const void* inData, int inLength ) = 0;
	virtual int				Length() = 0;
	virtual ID_TIME_T			Timestamp() = 0;
	virtual int				Tell() = 0;
	virtual void			ForceFlush() = 0;
	virtual void			Flush() = 0;
	virtual int				Seek( long offset, fsOrigin_t origin ) = 0;
};

#endif /* !__COMPRESSOR_H__ */
