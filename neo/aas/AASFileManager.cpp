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

#include "AASFile.h"
#include "AASFile_local.h"

/*
===============================================================================

	AAS File Manager

===============================================================================
*/

class idAASFileManagerLocal : public idAASFileManager
{
public:
	virtual						~idAASFileManagerLocal() {}

	virtual idAASFile* 			LoadAAS( const char* fileName, unsigned int mapFileCRC );
	virtual void				FreeAAS( idAASFile* file );
};

idAASFileManagerLocal			AASFileManagerLocal;
idAASFileManager* 				AASFileManager = &AASFileManagerLocal;


/*
================
idAASFileManagerLocal::LoadAAS
================
*/
idAASFile* idAASFileManagerLocal::LoadAAS( const char* fileName, unsigned int mapFileCRC )
{
	idAASFileLocal* file = new idAASFileLocal();
	if( !file->Load( fileName, mapFileCRC ) )
	{
		delete file;
		return NULL;
	}
	return file;
}

/*
================
idAASFileManagerLocal::FreeAAS
================
*/
void idAASFileManagerLocal::FreeAAS( idAASFile* file )
{
	delete file;
}
