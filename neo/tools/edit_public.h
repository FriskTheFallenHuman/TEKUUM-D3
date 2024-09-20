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

#ifndef __EDIT_PUBLIC_H__
#define __EDIT_PUBLIC_H__

/*
===============================================================================

	Editors.

===============================================================================
*/


class	idProgram;
class	idInterpreter;


// Radiant Level Editor
void	RadiantInit();
void	RadiantShutdown();
void	RadiantRun();
void	RadiantPrint( const char* text );


// in-game Light Editor
void	LightEditorInit( const idDict* spawnArgs );
void	LightEditorShutdown();
void	LightEditorRun();


// in-game Sound Editor
void	SoundEditorInit( const idDict* spawnArgs );
void	SoundEditorShutdown();
void	SoundEditorRun();


// in-game Articulated Figure Editor
void	AFEditorInit( const idDict* spawnArgs );
void	AFEditorShutdown();
void	AFEditorRun();


// in-game Particle Editor
void	ParticleEditorInit( const idDict* spawnArgs );
void	ParticleEditorShutdown();
void	ParticleEditorRun();


// in-game PDA Editor
void	PDAEditorInit( const idDict* spawnArgs );
void	PDAEditorShutdown();
void	PDAEditorRun();


// in-game Script Editor
void	ScriptEditorInit( const idDict* spawnArgs );
void	ScriptEditorShutdown();
void	ScriptEditorRun();


// in-game Declaration Browser
void	DeclBrowserInit( const idDict* spawnArgs );
void	DeclBrowserShutdown();
void	DeclBrowserRun();
void	DeclBrowserReloadDeclarations();


// GUI Editor
void	GUIEditorInit();
void	GUIEditorShutdown();
void	GUIEditorRun();
bool	GUIEditorHandleMessage( void* msg );


// Script Debugger
void	DebuggerClientLaunch();
void	DebuggerClientInit( const char* cmdline );
bool	DebuggerServerInit();
void	DebuggerServerShutdown();
void	DebuggerServerPrint( const char* text );
void	DebuggerServerCheckBreakpoint( idInterpreter* interpreter, idProgram* program, int instructionPointer );

//Material Editor
void	MaterialEditorInit();
void	MaterialEditorRun();
void	MaterialEditorShutdown();
void	MaterialEditorPrintConsole( const char* msg );

#endif /* !__EDIT_PUBLIC_H__ */
