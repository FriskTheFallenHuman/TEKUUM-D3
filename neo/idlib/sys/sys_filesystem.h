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
#ifndef SYS_FILESYSTEM_H
#define SYS_FILESYSTEM_H

void			Sys_Mkdir( const char* path );
bool			Sys_Rmdir( const char* path );
bool			Sys_IsFileWritable( const char* path );

enum sysFolder_t
{
	FOLDER_ERROR	= -1,
	FOLDER_NO		= 0,
	FOLDER_YES		= 1
};

// returns FOLDER_YES if the specified path is a folder
sysFolder_t		Sys_IsFolder( const char* path );

// use fs_debug to verbose Sys_ListFiles
// returns -1 if directory was not found (the list is cleared)
int				Sys_ListFiles( const char* directory, const char* extension, idList<class idStr>& list );

#endif
