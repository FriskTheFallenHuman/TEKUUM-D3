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

#ifndef __SYS_TYPEINFO_H__
#define __SYS_TYPEINFO_H__

/*
===================================================================================

	Game Type Info

===================================================================================
*/

const char* 	GetTypeVariableName( const char* typeName, int offset );

void			PrintType( const void* typePtr, const char* typeName );
void			WriteTypeToFile( idFile* fp, const void* typePtr, const char* typeName );
void			InitTypeVariables( const void* typePtr, const char* typeName, int value );

void			ListTypeInfo_f( const idCmdArgs& args );

void			WriteGameState_f( const idCmdArgs& args );
void			CompareGameState_f( const idCmdArgs& args );
void			TestSaveGame_f( const idCmdArgs& args );

#endif /* !__SYS_TYPEINFO_H__ */
