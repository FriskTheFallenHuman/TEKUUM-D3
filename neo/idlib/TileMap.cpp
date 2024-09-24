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

#include "TileMap.h"

static unsigned int GetLog2( float x )
{
	return ( unsigned int )( ceil( log( x ) / log( 2.0f ) ) );
}

void TileMap::Release()
{
	//tileNodeList.DeleteContents();
}

bool TileMap::Init( unsigned int mapSize, unsigned int maxAbsTileSize, unsigned int numLevels )
{
	if( ( !idMath::IsPowerOfTwo( mapSize ) ) || ( numLevels < 1 ) || ( maxAbsTileSize > mapSize ) || ( maxAbsTileSize < 16.0f ) )
	{
		return false;
	}

	this->mapSize = ( float )mapSize;
	log2MapSize = GetLog2( this->mapSize );
	this->maxAbsTileSize = ( float )maxAbsTileSize;
	this->numLevels = numLevels;

	minAbsTileSize = this->mapSize;
	for( unsigned int i = 0; i < ( numLevels - 1 ); i++ )
	{
		minAbsTileSize *= 0.5f;
	}
	if( ( minAbsTileSize < 16.0f ) || ( minAbsTileSize > maxAbsTileSize ) )
	{
		return false;
	}

	numNodes = 1;
	unsigned int multiplier = 1;
	for( unsigned int i = 1; i < numLevels; i++ )
	{
		multiplier *= 4;
		numNodes += multiplier;
	}
	tileNodeList.SetNum( numNodes );

	TileNode& rootNode = tileNodeList[nodeIndex];
	rootNode.position.x = 0;
	rootNode.position.y = 0;
	rootNode.level = 0;
	rootNode.minLevel = 0;
	BuildTree( rootNode, 0 );

	return true;
}

void TileMap::BuildTree( TileNode& parentNode, unsigned int level )
{
	level++;
	if( level == numLevels )
	{
		return;
	}

	for( unsigned int i = 0; i < 4; i++ )
	{
		parentNode.childIndices[i] = ++nodeIndex;
		assert( nodeIndex < numNodes );
		TileNode& currentNode = tileNodeList[parentNode.childIndices[i]];
		unsigned int denominator = 1 << level;
		const float size = 1.0f / ( ( float )denominator );
		idVec2 offsets[4] = { idVec2( -size, size ), idVec2( -size, -size ),  idVec2( size, -size ),  idVec2( size, size ) };
		//idVec2 offsets[4] = { idVec2( 0, size * 2 ), idVec2( 0, -0 ),  idVec2( size * 2, 0 ),  idVec2( size * 2, size * 2 ) };
		currentNode.position = parentNode.position + offsets[i];
		currentNode.level = level;
		currentNode.minLevel = 0;

		BuildTree( currentNode, level );
	}
}

void TileMap::Clear()
{
	for( unsigned int i = 0; i < numNodes; i++ )
	{
		tileNodeList[i].minLevel = 0;
	}
}

bool TileMap::GetTile( float size, Tile& tile )
{
	size = idMath::ClampInt( minAbsTileSize, maxAbsTileSize, size );
	unsigned int requiredLevel = log2MapSize - GetLog2( size );

	foundNode = NULL;
	TileNode& rootNode = tileNodeList[0];
	FindNode( rootNode, requiredLevel );
	if( !foundNode )
	{
		return false;
	}

	tile.position.x = foundNode->position.x;
	tile.position.y = foundNode->position.y;
	tile.size = size / mapSize;

	return true;
}

void TileMap::FindNode( TileNode& parentNode, unsigned int level )
{
	if( foundNode )
	{
		return;
	}

	for( unsigned int i = 0; i < 4; i++ )
	{
		if( foundNode )
		{
			return;
		}

		int childIndex = parentNode.childIndices[i];
		if( childIndex < 0 )
		{
			return;
		}

		TileNode& currentNode = tileNodeList[childIndex];
		if( level < currentNode.minLevel )
		{
			continue;
		}

		if( level == currentNode.level )
		{
			parentNode.minLevel = level;
			currentNode.minLevel = numLevels;
			foundNode = &currentNode;
			return;
		}

		FindNode( currentNode, level );
	}
}

