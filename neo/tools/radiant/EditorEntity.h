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

void		Eclass_InitForSourceDirectory();
eclass_t* 	Eclass_ForName( const char* name, bool has_brushes );

struct idEditorEntity
{
	idEditorEntity* prev, * next;
	idEditorBrush		brushes;					// head/tail of list
	int			undoId, redoId, entityId;	// used for undo/redo
	idVec3		origin;
	qhandle_t	lightDef;
	qhandle_t	modelDef;
	idSoundEmitter* soundEmitter;
	const eclass_t* eclass;
	idDict		epairs;
	idMat3		rotation;
	idVec3		lightOrigin;		// for lights that have been combined with models
	idMat3		lightRotation;		// ''
	bool		trackLightOrigin;
	idCurve<idVec3>* curve;
	renderEntity_t	refent;

	idEditorEntity();
	~idEditorEntity();

	void		BuildEntityRenderState( idEditorEntity* ent, bool update );

	const char* ValueForKey( const char* key ) const;
	int			GetNumKeys() const;
	const char* GetKeyString( int iIndex ) const;
	void		SetKeyValue( const char* key, const char* value, bool trackAngles = true );
	void		DeleteKey( const char* key );
	float		FloatForKey( const char* key );
	int			IntForKey( const char* key );
	bool		GetVectorForKey( const char* key, idVec3& vec );
	bool		GetVector4ForKey( const char* key, idVec4& vec );
	bool		GetFloatForKey( const char* key, float* f );
	void		SetKeyVec3( const char* key, idVec3 v );
	void		SetKeyMat3( const char* key, idMat3 m );
	bool		GetMatrixForKey( const char* key, idMat3& mat );

	void		UpdateSoundEmitter();
	idCurve<idVec3>* MakeCurve();
	void		UpdateCurveData();
	void		SetCurveData();

	void		FreeEpairs();
	int			MemorySize() const;

	void		WriteSelected( FILE* f );
	void		WriteSelected( CMemFile* );

	idEditorEntity* 	Clone() const;
	void		AddToList( idEditorEntity* list );
	void		RemoveFromList();
	bool		HasModel() const;

	void		PostParse( idEditorBrush* pList );

	void		SetName( const char* name );

	//Timo : used for parsing epairs in brush primitive
	void		Name( bool force );
};

void		ParseEpair( idDict* dict );

idEditorEntity* 	Entity_Parse( bool onlypairs, idEditorBrush* pList = NULL );
idEditorEntity* 	Entity_Create( eclass_t* c, bool forceFixed = false );

void		Entity_LinkBrush( idEditorEntity* e, idEditorBrush* b );
void		Entity_UnlinkBrush( idEditorBrush* b );
idEditorEntity* 	FindEntity( const char* pszKey, const char* pszValue );
idEditorEntity* 	FindEntityInt( const char* pszKey, int iValue );

bool		IsBrushSelected( const idEditorBrush* bSel );
