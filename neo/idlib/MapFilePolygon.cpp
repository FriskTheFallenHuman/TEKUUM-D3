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

#include "precompiled.h"
#pragma hdrstop

/*
==================
MapPolygonMesh::MapPolygonMesh
==================
*/
MapPolygonMesh::MapPolygonMesh()
{
	type = TYPE_MESH;
	originalType = TYPE_MESH;
	polygons.Resize( 8, 4 );

	contents = CONTENTS_SOLID;
	opaque = true;
}

/*
==================
MapPolygonMesh::ConvertFromBrush
==================
*/
void MapPolygonMesh::ConvertFromBrush( const idMapBrush* mapBrush, int entityNum, int primitiveNum )
{
	originalType = TYPE_BRUSH;

	// fix degenerate planes
	idPlane* planes = ( idPlane* ) _alloca16( mapBrush->GetNumSides() * sizeof( planes[0] ) );
	for( int i = 0; i < mapBrush->GetNumSides(); i++ )
	{
		planes[i] = mapBrush->GetSide( i )->GetPlane();
		planes[i].FixDegeneracies( DEGENERATE_DIST_EPSILON );
	}

	idList<idFixedWinding> planeWindings;
	idBounds bounds;
	bounds.Clear();

	int numVerts = 0;
	int numIndexes = 0;

	bool badBrush = false;

	for( int i = 0; i < mapBrush->GetNumSides(); i++ )
	{
		idMapBrushSide* mapSide = mapBrush->GetSide( i );

		const idMaterial* material = declManager->FindMaterial( mapSide->GetMaterial() );
		//contents |= ( material->GetContentFlags() & CONTENTS_REMOVE_UTIL );
		//materials.AddUnique( material );

		// chop base plane by other brush sides
		idFixedWinding& w = planeWindings.Alloc();
		w.BaseForPlane( -planes[i] );

		if( !w.GetNumPoints() )
		{
			common->Printf( "Entity %i, Brush %i: base winding has no points\n", entityNum, primitiveNum );
			badBrush = true;
			break;
		}

		for( int j = 0; j < mapBrush->GetNumSides() && w.GetNumPoints(); j++ )
		{
			if( i == j )
			{
				continue;
			}

			if( !w.ClipInPlace( -planes[j], 0 ) )
			{
				// no intersection
				//badBrush = true;
				common->Printf( "Entity %i, Brush %i: no intersection with other brush plane\n", entityNum, primitiveNum );
				//break;
			}
		}

		if( w.GetNumPoints() <= 2 )
		{
			continue;
		}

		// only used for debugging
		for( int j = 0; j < w.GetNumPoints(); j++ )
		{
			const idVec3& v = w[j].ToVec3();
			bounds.AddPoint( v );
		}
	}

	if( badBrush )
	{
		//common->Error( "" )
		return;
	}

	// copy the data from the windings and build polygons
	for( int i = 0; i < mapBrush->GetNumSides(); i++ )
	{
		idMapBrushSide* mapSide = mapBrush->GetSide( i );

		idFixedWinding& w = planeWindings[i];
		if( !w.GetNumPoints() )
		{
			continue;
		}

		MapPolygon& polygon = polygons.Alloc();
		polygon.SetMaterial( mapSide->GetMaterial() );


		//for( int j = 0; j < w.GetNumPoints(); j++ )

		// reverse order, so normal does not point inwards
		for( int j = w.GetNumPoints() - 1; j >= 0; j-- )
		{
			polygon.AddIndex( verts.Num() + j );
		}

		for( int j = 0; j < w.GetNumPoints(); j++ )
		{
			idDrawVert& dv = verts.Alloc();

			const idVec3& xyz = w[j].ToVec3();

			dv.xyz = xyz;

			// calculate texture s/t from brush primitive texture matrix
			idVec4 texVec[2];
			mapSide->GetTextureVectors( texVec );

			idVec2 st;
			st.x = ( xyz * texVec[0].ToVec3() ) + texVec[0][3];
			st.y = ( xyz * texVec[1].ToVec3() ) + texVec[1][3];

			// flip y
			//st.y = 1.0f - st.y;

			dv.SetTexCoord( st );

			// copy normal
			dv.SetNormal( mapSide->GetPlane().Normal() );

			//if( dv->GetNormal().Length() < 0.9 || dv->GetNormal().Length() > 1.1 )
			//{
			//	common->Error( "Bad normal in TriListForSide" );
			//}
		}
	}

	SetContents();
}

/*
==================
MapPolygonMesh::ConvertFromPatch
==================
*/
void MapPolygonMesh::ConvertFromPatch( const idMapPatch* patch, int entityNum, int primitiveNum )
{
	originalType = TYPE_PATCH;

	idSurface_Patch* cp = new idSurface_Patch( *patch );

	if( patch->GetExplicitlySubdivided() )
	{
		cp->SubdivideExplicit( patch->GetHorzSubdivisions(), patch->GetVertSubdivisions(), true );
	}
	else
	{
		cp->Subdivide( DEFAULT_CURVE_MAX_ERROR, DEFAULT_CURVE_MAX_ERROR, DEFAULT_CURVE_MAX_LENGTH, true );
	}

	for( int i = 0; i < cp->GetNumIndexes(); i += 3 )
	{
		verts.Append( ( *cp )[cp->GetIndexes()[i + 1]] );
		verts.Append( ( *cp )[cp->GetIndexes()[i + 2]] );
		verts.Append( ( *cp )[cp->GetIndexes()[i + 0]] );
	}

	for( int i = 0; i < cp->GetNumIndexes(); i += 3 )
	{
		MapPolygon& polygon = polygons.Alloc();
		polygon.SetMaterial( patch->GetMaterial() );

		polygon.AddIndex( i + 0 );
		polygon.AddIndex( i + 1 );
		polygon.AddIndex( i + 2 );
	}

	delete cp;

	SetContents();
}

/*
==================
MapPolygonMesh::Write
==================
*/
bool MapPolygonMesh::Write( idFile* fp, int primitiveNum, const idVec3& origin ) const
{
	fp->WriteFloatString( "// primitive %d\n{\n meshDef\n {\n", primitiveNum );
	//fp->WriteFloatString( "  \"%s\"\n  ( %d %d 0 0 0 )\n", GetMaterial(), GetWidth(), GetHeight() );

	fp->WriteFloatString( "  ( %d %d 0 0 0 )\n", verts.Num(), polygons.Num() );

	fp->WriteFloatString( "  (\n" );
	idVec2 st;
	idVec3 n;
	for( int i = 0; i < verts.Num(); i++ )
	{
		const idDrawVert* v = &verts[ i ];
		st = v->GetTexCoord();
		n = v->GetNormalRaw();

		//fp->WriteFloatString( "   ( %f %f %f %f %f %f %f %f )\n", v->xyz[0] + origin[0], v->xyz[1] + origin[1], v->xyz[2] + origin[2], st[0], st[1], n[0], n[1], n[2] );
		fp->WriteFloatString( "   ( %f %f %f %f %f %f %f %f )\n", v->xyz[0], v->xyz[1], v->xyz[2], st[0], st[1], n[0], n[1], n[2] );
	}
	fp->WriteFloatString( "  )\n" );

	fp->WriteFloatString( "  (\n" );
	for( int i = 0; i < polygons.Num(); i++ )
	{
		const MapPolygon& poly = polygons[ i ];

		fp->WriteFloatString( "   \"%s\" %d = ", poly.GetMaterial(), poly.indexes.Num() );

		for( int j = 0; j < poly.indexes.Num(); j++ )
		{
			fp->WriteFloatString( "%d ", poly.indexes[j] );
		}
		fp->WriteFloatString( "\n" );
	}
	fp->WriteFloatString( "  )\n" );

	fp->WriteFloatString( " }\n}\n" );

	return true;
}

/*
==================
MapPolygonMesh::Parse
==================
*/
MapPolygonMesh* MapPolygonMesh::Parse( idLexer& src, const idVec3& origin, float version )
{
	float		info[7];
	idToken		token;
	int			i;

	if( !src.ExpectTokenString( "{" ) )
	{
		return NULL;
	}

	// Parse it
	if( !src.Parse1DMatrix( 5, info ) )
	{
		src.Error( "MapPolygonMesh::Parse: unable to parse meshDef info" );
		return NULL;
	}

	const int numVertices = ( int ) info[0];
	const int numPolygons = ( int ) info[1];

	MapPolygonMesh* mesh = new MapPolygonMesh();

	// parse vertices
	if( !src.ExpectTokenString( "(" ) )
	{
		src.Error( "MapPolygonMesh::Parse: bad mesh vertex data" );
		delete mesh;
		return NULL;
	}

	for( i = 0; i < numVertices; i++ )
	{
		float v[8];

		if( !src.Parse1DMatrix( 8, v ) )
		{
			src.Error( "MapPolygonMesh::Parse: bad vertex column data" );
			delete mesh;
			return NULL;
		}

		// TODO optimize: preallocate vertices
		//vert = &( ( *patch )[i * patch->GetWidth() + j] );

		idDrawVert vert;

		vert.xyz[0] = v[0];// - origin[0];
		vert.xyz[1] = v[1];// - origin[1];
		vert.xyz[2] = v[2];// - origin[2];
		vert.SetTexCoord( v[3], v[4] );

		idVec3 n( v[5], v[6], v[7] );
		vert.SetNormal( n );

		mesh->AddVertex( vert );
	}

	if( !src.ExpectTokenString( ")" ) )
	{
		delete mesh;
		src.Error( "MapPolygonMesh::Parse: unable to parse vertices" );
		return NULL;
	}

	// parse polygons
	if( !src.ExpectTokenString( "(" ) )
	{
		src.Error( "MapPolygonMesh::Parse: bad mesh polygon data" );
		delete mesh;
		return NULL;
	}

	for( i = 0; i < numPolygons; i++ )
	{
		// get material name
		MapPolygon& polygon = mesh->polygons.Alloc();

		src.ReadToken( &token );
		if( token.type == TT_STRING )
		{
			polygon.SetMaterial( token );;
		}
		else
		{
			src.Error( "MapPolygonMesh::Parse: bad mesh polygon data" );
			delete mesh;
			return NULL;
		}

		int numIndexes = src.ParseInt();

		if( !src.ExpectTokenString( "=" ) )
		{
			src.Error( "MapPolygonMesh::Parse: bad mesh polygon data" );
			delete mesh;
			return NULL;
		}

		//idTempArray<int> indexes( numIndexes );
		for( int j = 0; j < numIndexes; j++ )
		{
			//indexes[j] = src.ParseInt();

			int index = src.ParseInt();
			polygon.AddIndex( index );
		}

		//polygon->SetIndexes( indexes );
	}

	if( !src.ExpectTokenString( ")" ) )
	{
		delete mesh;
		src.Error( "MapPolygonMesh::Parse: unable to parse polygons" );
		return NULL;
	}

	if( !src.ExpectTokenString( "}" ) )
	{
		delete mesh;
		src.Error( "MapPolygonMesh::Parse: unable to parse mesh primitive end" );
		return NULL;
	}

	if( !src.ExpectTokenString( "}" ) )
	{
		delete mesh;
		src.Error( "MapPolygonMesh::Parse: unable to parse mesh primitive end" );
		return NULL;
	}

	mesh->SetContents();

	return mesh;
}

/*
==================
MapPolygonMesh::SetContents
==================
*/
void MapPolygonMesh::SetContents()
{
	if( polygons.Num() < 1 )
	{
		contents = CONTENTS_SOLID;
		opaque = true;

		return;
	}

	int			c2;

	MapPolygon* poly = &polygons[0];

	const idMaterial* mat = declManager->FindMaterial( poly->GetMaterial() );
	contents = mat->GetContentFlags();

	//b->contentShader = s->material;
	bool mixed = false;

	// a brush is only opaque if all sides are opaque
	opaque = true;

	for( int i = 1 ; i < polygons.Num() ; i++ )
	{
		poly = &polygons[i];

		const idMaterial* mat2 = declManager->FindMaterial( poly->GetMaterial() );

		c2 = mat2->GetContentFlags();
		if( c2 != contents )
		{
			mixed = true;
			contents |= c2;
		}

		if( mat2->Coverage() != MC_OPAQUE )
		{
			opaque = false;
		}
	}
}

/*
==================
MapPolygonMesh::GetGeometryCRC
==================
*/
unsigned int MapPolygonMesh::GetGeometryCRC() const
{
	int i;
	unsigned int crc;

	crc = 0;
	for( i = 0; i < verts.Num(); i++ )
	{
		crc ^= FloatCRC( verts[i].xyz.x );
		crc ^= FloatCRC( verts[i].xyz.y );
		crc ^= FloatCRC( verts[i].xyz.z );
	}

	for( i = 0; i < polygons.Num(); i++ )
	{
		const MapPolygon& poly = polygons[i];

		crc ^= StringCRC( poly.GetMaterial() );
	}

	return crc;
}

/*
==================
MapPolygonMesh::IsAreaportal
==================
*/
bool MapPolygonMesh::IsAreaportal() const
{
	return ( ( contents & CONTENTS_AREAPORTAL ) != 0 );
}

/*
==================
MapPolygonMesh::GetBounds
==================
*/
void MapPolygonMesh::GetBounds( idBounds& bounds ) const
{
	if( !verts.Num() )
	{
		bounds.Clear();
		return;
	}


	bounds[0] = bounds[1] = verts[0].xyz;
	for( int i = 1; i < verts.Num(); i++ )
	{
		const idVec3& p = verts[i].xyz;

		if( p.x < bounds[0].x )
		{
			bounds[0].x = p.x;
		}
		else if( p.x > bounds[1].x )
		{
			bounds[1].x = p.x;
		}
		if( p.y < bounds[0].y )
		{
			bounds[0].y = p.y;
		}
		else if( p.y > bounds[1].y )
		{
			bounds[1].y = p.y;
		}
		if( p.z < bounds[0].z )
		{
			bounds[0].z = p.z;
		}
		else if( p.z > bounds[1].z )
		{
			bounds[1].z = p.z;
		}
	}
}