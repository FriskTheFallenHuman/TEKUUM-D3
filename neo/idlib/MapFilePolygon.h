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

#ifndef __MAPFILEPOLYGON_H__
#define __MAPFILEPOLYGON_H__

class MapPolygon
{
	friend class MapPolygonMesh;

public:
	MapPolygon();
	MapPolygon( int numIndexes );
	~MapPolygon() { }

	const char* 			GetMaterial() const
	{
		return material;
	}

	void					SetMaterial( const char* p )
	{
		material = p;
	}

	void					AddIndex( int index )
	{
		indexes.Append( index );
	}

	void					SetIndexes( const idTempArray<int>& _indexes )
	{
		indexes.Resize( _indexes.Num() );

		for( unsigned int i = 0; i < _indexes.Num(); i++ )
		{
			indexes[i] = _indexes[i];
		}
	}

	const idList<int>&		GetIndexes() const
	{
		return indexes;
	}


protected:
	idStr					material;
	idList<int>				indexes;		// [3..n] references to vertices for each face
};

ID_INLINE MapPolygon::MapPolygon()
{
}

ID_INLINE MapPolygon::MapPolygon( int numIndexes )
{
	//indexes.AssureSize( 3 );
}

class MapPolygonMesh : public idMapPrimitive
{
public:
	MapPolygonMesh();
	~MapPolygonMesh()
	{
		//verts.DeleteContents();
		//polygons.DeleteContents( true );
	}

	void					ConvertFromBrush( const idMapBrush* brush, int entityNum, int primitiveNum );
	void					ConvertFromPatch( const idMapPatch* patch, int entityNum, int primitiveNum );

	static MapPolygonMesh*	Parse( idLexer& src, const idVec3& origin, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile* fp, int primitiveNum, const idVec3& origin ) const;

	int						GetNumVertices() const
	{
		return verts.Num();
	}

	int						AddVertex( const idDrawVert& v )
	{
		return verts.Append( v );
	}


	int						GetNumPolygons() const
	{
		return polygons.Num();
	}

	//int						AddPolygon( MapPolygon* face )
	//{
	//	return polygons.Append( face );
	//}

	const MapPolygon& 			GetFace( int i ) const
	{
		return polygons[i];
	}

	unsigned int			GetGeometryCRC() const;

	const idList<idDrawVert>&	GetDrawVerts() const
	{
		return verts;
	}

	bool					IsOpaque() const
	{
		return opaque;
	}

	bool					IsAreaportal() const;

	void					GetBounds( idBounds& bounds ) const;

private:
	void					SetContents();

	int						originalType;

protected:

	idList<idDrawVert>		verts;			// vertices can be shared between polygons
	idList<MapPolygon>		polygons;

	// derived data after parsing

	// material surface flags
	int						contents;
	bool					opaque;
};

#endif /* !__MAPFILEPOLYGON_H__ */
