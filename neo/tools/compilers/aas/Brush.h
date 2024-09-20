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

#ifndef __BRUSH_H__
#define __BRUSH_H__

/*
===============================================================================

	Brushes

===============================================================================
*/


#define BRUSH_PLANESIDE_FRONT		1
#define BRUSH_PLANESIDE_BACK		2
#define BRUSH_PLANESIDE_BOTH		( BRUSH_PLANESIDE_FRONT | BRUSH_PLANESIDE_BACK )
#define BRUSH_PLANESIDE_FACING		4

class idBrush;
class idBrushList;

void DisplayRealTimeString( char const* string, ... ) ID_STATIC_ATTRIBUTE_PRINTF( 1, 2 );


//===============================================================
//
//	idBrushSide
//
//===============================================================

#define SFL_SPLIT					0x0001
#define SFL_BEVEL					0x0002
#define SFL_USED_SPLITTER			0x0004
#define SFL_TESTED_SPLITTER			0x0008

class idBrushSide
{

	friend class idBrush;

public:
	idBrushSide();
	idBrushSide( const idPlane& plane, int planeNum );
	~idBrushSide();

	int						GetFlags() const
	{
		return flags;
	}
	void					SetFlag( int flag )
	{
		flags |= flag;
	}
	void					RemoveFlag( int flag )
	{
		flags &= ~flag;
	}
	const idPlane& 			GetPlane() const
	{
		return plane;
	}
	void					SetPlaneNum( int num )
	{
		planeNum = num;
	}
	int						GetPlaneNum()
	{
		return planeNum;
	}
	const idWinding* 		GetWinding() const
	{
		return winding;
	}
	idBrushSide* 			Copy() const;
	int						Split( const idPlane& splitPlane, idBrushSide** front, idBrushSide** back ) const;

private:
	int						flags;
	int						planeNum;
	idPlane					plane;
	idWinding* 				winding;
};


//===============================================================
//
//	idBrush
//
//===============================================================

#define BFL_NO_VALID_SPLITTERS		0x0001

class idBrush
{

	friend class idBrushList;

public:
	idBrush();
	~idBrush();

	int						GetFlags() const
	{
		return flags;
	}
	void					SetFlag( int flag )
	{
		flags |= flag;
	}
	void					RemoveFlag( int flag )
	{
		flags &= ~flag;
	}
	void					SetEntityNum( int num )
	{
		entityNum = num;
	}
	void					SetPrimitiveNum( int num )
	{
		primitiveNum = num;
	}
	void					SetContents( int contents )
	{
		this->contents = contents;
	}
	int						GetContents() const
	{
		return contents;
	}
	const idBounds& 		GetBounds() const
	{
		return bounds;
	}
	float					GetVolume() const;
	int						GetNumSides() const
	{
		return sides.Num();
	}
	idBrushSide* 			GetSide( int i ) const
	{
		return sides[i];
	}
	void					SetPlaneSide( int s )
	{
		planeSide = s;
	}
	void					SavePlaneSide()
	{
		savedPlaneSide = planeSide;
	}
	int						GetSavedPlaneSide() const
	{
		return savedPlaneSide;
	}
	bool					FromSides( idList<idBrushSide*>& sideList );
	bool					FromWinding( const idWinding& w, const idPlane& windingPlane );
	bool					FromBounds( const idBounds& bounds );
	void					Transform( const idVec3& origin, const idMat3& axis );
	idBrush* 				Copy() const;
	bool					TryMerge( const idBrush* brush, const idPlaneSet& planeList );
	// returns true if the brushes did intersect
	bool					Subtract( const idBrush* b, idBrushList& list ) const;
	// split the brush into a front and back brush
	int						Split( const idPlane& plane, int planeNum, idBrush** front, idBrush** back ) const;
	// expand the brush for an axial bounding box
	void					ExpandForAxialBox( const idBounds& bounds );
	// next brush in list
	idBrush* 				Next() const
	{
		return next;
	}

private:
	mutable idBrush* 		next;				// next brush in list
	int						entityNum;			// entity number in editor
	int						primitiveNum;		// primitive number in editor
	int						flags;				// brush flags
	bool					windingsValid;		// set when side windings are valid
	int						contents;			// contents of brush
	int						planeSide;			// side of a plane this brush is on
	int						savedPlaneSide;		// saved plane side
	idBounds				bounds;				// brush bounds
	idList<idBrushSide*>	sides;				// list with sides

private:
	bool					CreateWindings();
	void					BoundBrush( const idBrush* original = NULL );
	void					AddBevelsForAxialBox();
	bool					RemoveSidesWithoutWinding();
};


//===============================================================
//
//	idBrushList
//
//===============================================================

class idBrushList
{
public:
	idBrushList();
	~idBrushList();

	int						Num() const
	{
		return numBrushes;
	}
	int						NumSides() const
	{
		return numBrushSides;
	}
	idBrush* 				Head() const
	{
		return head;
	}
	idBrush* 				Tail() const
	{
		return tail;
	}
	void					Clear()
	{
		head = tail = NULL;
		numBrushes = 0;
	}
	bool					IsEmpty() const
	{
		return ( numBrushes == 0 );
	}
	idBounds				GetBounds() const;
	// add brush to the tail of the list
	void					AddToTail( idBrush* brush );
	// add list to the tail of the list
	void					AddToTail( idBrushList& list );
	// add brush to the front of the list
	void					AddToFront( idBrush* brush );
	// add list to the front of the list
	void					AddToFront( idBrushList& list );
	// remove the brush from the list
	void					Remove( idBrush* brush );
	// remove the brush from the list and delete the brush
	void					Delete( idBrush* brush );
	// returns a copy of the brush list
	idBrushList* 			Copy() const;
	// delete all brushes in the list
	void					Free();
	// split the brushes in the list into two lists
	void					Split( const idPlane& plane, int planeNum, idBrushList& frontList, idBrushList& backList, bool useBrushSavedPlaneSide = false );
	// chop away all brush overlap
	void					Chop( bool ( *ChopAllowed )( idBrush* b1, idBrush* b2 ) );
	// merge brushes
	void					Merge( bool ( *MergeAllowed )( idBrush* b1, idBrush* b2 ) );
	// set the given flag on all brush sides facing the plane
	void					SetFlagOnFacingBrushSides( const idPlane& plane, int flag );
	// get a list with planes for all brushes in the list
	void					CreatePlaneList( idPlaneSet& planeList ) const;
	// write a brush map with the brushes in the list
	void					WriteBrushMap( const idStr& fileName, const idStr& ext ) const;

private:
	idBrush* 				head;
	idBrush* 				tail;
	int						numBrushes;
	int						numBrushSides;
};


//===============================================================
//
//	idBrushMap
//
//===============================================================

class idBrushMap
{

public:
	idBrushMap( const idStr& fileName, const idStr& ext );
	~idBrushMap();
	void					SetTexture( const idStr& textureName )
	{
		texture = textureName;
	}
	void					WriteBrush( const idBrush* brush );
	void					WriteBrushList( const idBrushList& brushList );

private:
	idFile* 				fp;
	idStr					texture;
	int						brushCount;
};

#endif /* !__BRUSH_H__ */
