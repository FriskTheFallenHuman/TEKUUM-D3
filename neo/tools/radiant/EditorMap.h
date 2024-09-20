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

extern	char		currentmap[1024];

// head/tail of doubly linked lists
extern	idEditorBrush	active_brushes;	// brushes currently being displayed
extern	idEditorBrush	selected_brushes;	// highlighted


extern CPtrArray& g_ptrSelectedFaces;
extern CPtrArray& g_ptrSelectedFaceBrushes;
//extern	face_t	*selected_face;
//extern	idEditorBrush	*selected_face_brush;
extern	idEditorBrush	filtered_brushes;	// brushes that have been filtered or regioned

extern	idEditorEntity	entities;
extern	idEditorEntity*	world_entity;	// the world entity is NOT included in
// the entities chain

extern	int modified;		// for quit confirmations

extern	idVec3	region_mins, region_maxs;
extern	bool	region_active;

void	Map_LoadFile( const char* filename );
bool	Map_SaveFile( const char* filename, bool use_region, bool autosave = false );
void	Map_New();
void	Map_BuildBrushData();

void	Map_RegionOff();
void	Map_RegionXY();
void	Map_RegionTallBrush();
void	Map_RegionBrush();
void	Map_RegionSelectedBrushes();
bool	Map_IsBrushFiltered( idEditorBrush* b );

void	Map_SaveSelected( CMemFile* pMemFile, CMemFile* pPatchFile = NULL );
void	Map_ImportBuffer( char* buf, bool renameEntities = true );
int		Map_GetUniqueEntityID( const char* prefix, const char* eclass );

idMapPrimitive* BrushToMapPrimitive( const idEditorBrush* b, const idVec3& origin );
