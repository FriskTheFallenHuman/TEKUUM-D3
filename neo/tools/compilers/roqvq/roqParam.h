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
#ifndef __roqParam_h__
#define __roqParam_h__

#include "gdefs.h"

class roqParam
{
public:

	const char*		RoqFilename();
	const char*		RoqTempFilename();
	const char*		GetNextImageFilename();
	const char*		SoundFilename();
	void			InitFromFile( const char* fileName );
	void			GetNthInputFileName( idStr& fileName, int n );
	bool			MoreFrames();
	bool			OutputVectors();
	bool			Timecode();
	bool			DeltaFrames();
	bool			NoAlpha();
	bool			SearchType();
	bool			TwentyFourToThirty();
	bool			HasSound();
	int				NumberOfFrames();
	int				NormalFrameSize();
	int				FirstFrameSize();
	int				JpegQuality();
	bool			IsScaleable();

	idStr			outputFilename;
	int				numInputFiles;
private:
	int*				range;
	bool*			padding, *padding2;
	idStrList		file;
	idStrList		file2;
	idStr			soundfile;
	idStr			currentPath;
	idStr			tempFilename;
	idStr			startPal;
	idStr			endPal;
	idStr			currentFile;
	int*				skipnum, *skipnum2;
	int*				startnum, *startnum2;
	int*				endnum, *endnum2;
	int*				numpadding, *numpadding2;
	int*				numfiles;
	byte			keyR, keyG, keyB;
	int				field;
	int				realnum;
	int				onFrame;
	int				firstframesize;
	int				normalframesize;
	int				jpegDefault;

	bool			scaleDown;
	bool			twentyFourToThirty;
	bool			encodeVideo;
	bool			useTimecodeForRange;
	bool			addPath;
	bool			screenShots;
	bool			startPalette;
	bool			endPalette;
	bool			fixedPalette;
	bool			keyColor;
	bool			justDelta;
	bool			make3DO;
	bool			makeVectors;
	bool			justDeltaFlag;
	bool			noAlphaAtAll;
	bool			fullSearch;
	bool			hasSound;
	bool			isScaleable;
};


#endif // roqParam
