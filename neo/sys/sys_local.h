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

#ifndef __SYS_LOCAL__
#define __SYS_LOCAL__

/*
==============================================================

	idSysLocal

==============================================================
*/

class idSysLocal : public idSys
{
public:
	virtual void			DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... );
	virtual void			DebugVPrintf( const char* fmt, va_list arg );

	virtual double			GetClockTicks();
	virtual double			ClockTicksPerSecond();
	virtual cpuid_t			GetProcessorId();
	virtual void			FPU_SetFTZ( bool enable );
	virtual void			FPU_SetDAZ( bool enable );

	virtual bool			LockMemory( void* ptr, int bytes );
	virtual bool			UnlockMemory( void* ptr, int bytes );

	virtual uintptr_t		DLL_Load( const char* dllName );
	virtual void* 			DLL_GetProcAddress( uintptr_t dllHandle, const char* procName );
	virtual void			DLL_Unload( uintptr_t dllHandle );
	virtual void			DLL_GetFileName( const char* baseName, char* dllName, int maxLength );

	virtual sysEvent_t		GenerateMouseButtonEvent( int button, bool down );
	virtual sysEvent_t		GenerateMouseMoveEvent( int deltax, int deltay );

	virtual void			OpenURL( const char* url, bool quit );
	virtual void			StartProcess( const char* exeName, bool quit );
};

#endif /* !__SYS_LOCAL__ */
