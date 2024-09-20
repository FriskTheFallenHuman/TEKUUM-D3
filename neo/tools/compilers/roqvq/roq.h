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
#ifndef __roq_h__
#define __roq_h__

//#define JPEG_INTERNALS
//#include <jpeglib.h> // DG: unused

#include "gdefs.h"
#include "roqParam.h"
#include "quaddefs.h"

class codec;
class roqParam;

class NSBitmapImageRep
{
public:

	NSBitmapImageRep();
	NSBitmapImageRep( const char* filename );
	NSBitmapImageRep( int wide, int high );
	~NSBitmapImageRep();

	NSBitmapImageRep& 	operator=( const NSBitmapImageRep& a );

	int					samplesPerPixel();
	int					pixelsWide();
	int					pixelsHigh();
	byte* 				bitmapData();
	bool				hasAlpha();
	bool				isPlanar();

private:

	byte* 				bmap;
	int					width;
	int					height;
	ID_TIME_T				timestamp;

};

class roq
{
public:
	roq();
	~roq();

	//void				WriteLossless();
	void				LoadAndDisplayImage( const char* filename );
	void				CloseRoQFile( bool which );
	void				InitRoQFile( const char* roqFilename );
	void				InitRoQPatterns();
	void				EncodeStream( const char* paramInputFile );
	void				EncodeQuietly( bool which );
	bool				IsQuiet();
	bool				IsLastFrame();
	NSBitmapImageRep* 	CurrentImage();
	void				MarkQuadx( int xat, int yat, int size, float cerror, int choice );
	void				WritePuzzleFrame( quadcel* pquad );
	void				WriteFrame( quadcel* pquad );
	void				WriteCodeBook( byte* codebook );
	void				WwriteCodeBookToStream( byte* codes, int csize, word cflags );
	int					PreviousFrameSize();
	bool				MakingVideo();
	bool				ParamNoAlpha();
	bool				SearchType();
	bool				HasSound();
	const char* 		CurrentFilename();
	int					NormalFrameSize();
	int					FirstFrameSize();
	bool				Scaleable();
	void				WriteHangFrame();
	int					NumberOfFrames();
private:
	void				Write16Word( word* aWord, idFile* stream );
	void				Write32Word( unsigned int* aWord, idFile* stream );
	int					SizeFile( idFile* ftosize );
	void				CloseRoQFile();
	void				WriteCodeBookToStream( byte* codebook, int csize, word cflags );

#if 0
	static	void		JPEGInitDestination( j_compress_ptr cinfo );
	static	boolean		JPEGEmptyOutputBuffer( j_compress_ptr cinfo );
	static	void		JPEGTermDestination( j_compress_ptr cinfo );

	void				JPEGStartCompress( j_compress_ptr cinfo, bool write_all_tables );
	JDIMENSION			JPEGWriteScanlines( j_compress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION num_lines );
	void				JPEGDest( j_compress_ptr cinfo, byte* outfile, int size );
	void				JPEGSave( char* filename, int quality, int image_width, int image_height, unsigned char* image_buffer );
#endif

	codec* 				encoder;
	roqParam* 			paramFile;

	idFile* 			RoQFile;
	NSBitmapImageRep* 	image;
	int					numQuadCels;
	bool				quietMode;
	bool				lastFrame;
	idStr				roqOutfile;
	idStr				currentFile;
	int					numberOfFrames;
	int					previousSize;
	byte				codes[4096];
	bool				dataStuff;

};

extern roq* theRoQ;				// current roq

#endif /* !__roq_h__ */
