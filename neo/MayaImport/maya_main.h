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

#ifndef __MAYA_MAIN_H__
#define __MAYA_MAIN_H__

#ifndef ID_MAYA_IMPORT_API
	#define ID_MAYA_IMPORT_API
#endif

/*
==============================================================

	Maya Import

==============================================================
*/

typedef ID_MAYA_IMPORT_API bool ( *exporterDLLEntry_t )( int version, idCommon* common, idSys* sys );
typedef ID_MAYA_IMPORT_API const char* ( *exporterInterface_t )( const char* ospath, const char* commandline );
typedef ID_MAYA_IMPORT_API void ( *exporterShutdown_t )( void );

#endif /* !__MAYA_MAIN_H__ */
