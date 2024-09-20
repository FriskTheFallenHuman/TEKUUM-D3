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

//start operation
void Undo_Start( char* operation );
//end operation
void Undo_End();
//add brush to the undo
void Undo_AddBrush( idEditorBrush* pBrush );
//add a list with brushes to the undo
void Undo_AddBrushList( idEditorBrush* brushlist );
//end a brush after the operation is performed
void Undo_EndBrush( idEditorBrush* pBrush );
//end a list with brushes after the operation is performed
void Undo_EndBrushList( idEditorBrush* brushlist );
//add entity to undo
void Undo_AddEntity( idEditorEntity* entity );
//end an entity after the operation is performed
void Undo_EndEntity( idEditorEntity* entity );
//undo last operation
void Undo_Undo();
//redo last undone operation
void Undo_Redo();
//returns true if there is something to be undone available
int  Undo_UndoAvailable();
//returns true if there is something to redo available
int  Undo_RedoAvailable();
//clear the undo buffer
void Undo_Clear();
//set maximum undo size (default 64)
void Undo_SetMaxSize( int size );
//get maximum undo size
int  Undo_GetMaxSize();
//set maximum undo memory in bytes (default 2 MB)
void Undo_SetMaxMemorySize( int size );
//get maximum undo memory in bytes
int  Undo_GetMaxMemorySize();
//returns the amount of memory used by undo
int  Undo_MemorySize();
