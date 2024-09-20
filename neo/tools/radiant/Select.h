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

#ifndef __SELECT_H_
#define __SELECT_H_

typedef enum
{
	sel_brush,
	// sel_sticky_brush,
	// sel_face,
	sel_vertex,
	sel_edge,
	sel_singlevertex,
	sel_curvepoint,
	sel_area,
	sel_addpoint,			// for dropping points
	sel_editpoint			// for editing points
} select_t;

class CDragPoint
{
public:
	idVec3 vec;
	idEditorBrush* pBrush;
	int nType;
	bool priority;
	CDragPoint() {};
	CDragPoint( idEditorBrush* b, idVec3 v, int type, bool p )
	{
		pBrush = b;
		VectorCopy( v, vec );
		nType = type;
		priority = p;
	}

	void Set( idEditorBrush* b, idVec3 v, int type )
	{
		pBrush = b;
		VectorCopy( v, vec );
		nType = type;
	}

	bool PointWithin( idVec3 p, ViewType nView = ( ViewType ) - 1 );
};


typedef struct
{
	idEditorBrush*		brush;
	face_t*		face;
	CDragPoint* point;
	float		dist;
	bool	selected;
} qertrace_t;


#define	SF_SELECTED_ONLY	 0x01
#define	SF_ENTITIES_FIRST	 0x02
#define	SF_SINGLEFACE		   0x04
#define SF_IGNORECURVES    0x08
#define SF_IGNOREGROUPS    0x10
#define SF_CYCLE           0x20


qertrace_t Test_Ray( const idVec3& origin, const idVec3& dir, int flags );
CDragPoint* PointRay( const idVec3& org, const idVec3& dir, float* dist );
void SelectCurvePointByRay( const idVec3& org, const idVec3& dir, int buttons );

void Select_GetBounds( idVec3& mins, idVec3& maxs );
void Select_Brush( idEditorBrush* b, bool bComplete = true, bool bStatus = true );
void Select_Ray( idVec3 origin, idVec3 dir, int flags );
void Select_Delete();
void Select_Deselect( bool bDeselectFaces = true );
void Select_Invert();
void Select_Clone();
void Select_Move( idVec3 delta, bool bSnap = true );
void WINAPI Select_SetTexture( texdef_t* texdef, brushprimit_texdef_t* brushprimit_texdef, bool bFitScale = false, void* pPlugTexdef = NULL, bool update = true );
void Select_FlipAxis( int axis );
void Select_RotateAxis( int axis, float deg, bool bPaint = true, bool bMouse = false );
void Select_CompleteTall();
void Select_PartialTall();
void Select_Touching();
void Select_Inside();
void Select_CenterOrigin();
void Select_AllOfType();
void Select_Reselect();
void Select_FitTexture( float height = 1.0, float width = 1.0 );
void Select_InitializeRotation();
void Select_FinalizeRotation();

void Select_Hide( bool invert = false );
void Select_ShowAllHidden();
void Select_WireFrame( bool wireFrame );
void Select_ForceVisible( bool visible );
void Select_GetMid( idVec3& mid );
void Select_SetDefaultTexture( const idMaterial* mat, bool fitScale, bool setTexture );
void Select_UpdateTextureName( const char* name );

void Select_FlipTexture( bool y );
void Select_SetPatchFit( float dim1, float dim2, float srcWidth, float srcHeight, float rot );
void Select_SetPatchST( float s1, float t1, float s2, float t2 );
void Select_ProjectFaceOntoPatch( face_t* face );
void Select_CopyPatchTextureCoords( patchMesh_t* p );
void Select_AllTargets();
void Select_Scale( float x, float y, float z );
void Select_RotateTexture( float amt, bool absolute = false );
void Select_ScaleTexture( float x, float y, bool update = true, bool absolute = true );
void Select_ShiftTexture( float x, float y, bool autoAdjust = false );
void Select_GetTrueMid( idVec3& mid );
void Select_Scale( float x, float y, float z );


#endif