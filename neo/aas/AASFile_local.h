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

#ifndef __AASFILELOCAL_H__
#define __AASFILELOCAL_H__

/*
===============================================================================

	AAS File Local

===============================================================================
*/

class idAASFileLocal : public idAASFile
{
	friend class idAASBuild;
	friend class idAASReach;
	friend class idAASCluster;
public:
	idAASFileLocal();
	virtual 					~idAASFileLocal();

public:
	virtual idVec3				EdgeCenter( int edgeNum ) const;
	virtual idVec3				FaceCenter( int faceNum ) const;
	virtual idVec3				AreaCenter( int areaNum ) const;

	virtual idBounds			EdgeBounds( int edgeNum ) const;
	virtual idBounds			FaceBounds( int faceNum ) const;
	virtual idBounds			AreaBounds( int areaNum ) const;

	virtual int					PointAreaNum( const idVec3& origin ) const;
	virtual int					PointReachableAreaNum( const idVec3& origin, const idBounds& searchBounds, const int areaFlags, const int excludeTravelFlags ) const;
	virtual int					BoundsReachableAreaNum( const idBounds& bounds, const int areaFlags, const int excludeTravelFlags ) const;
	virtual void				PushPointIntoAreaNum( int areaNum, idVec3& point ) const;
	virtual bool				Trace( aasTrace_t& trace, const idVec3& start, const idVec3& end ) const;
	virtual void				PrintInfo() const;

public:
	bool						Load( const idStr& fileName, unsigned int mapFileCRC );
	bool						Write( const idStr& fileName, unsigned int mapFileCRC );

	int							MemorySize() const;
	void						ReportRoutingEfficiency() const;
	void						Optimize();
	void						LinkReversedReachability();
	void						FinishAreas();

	void						Clear();
	void						DeleteReachabilities();
	void						DeleteClusters();

private:
	bool						ParseIndex( idLexer& src, idList<aasIndex_t>& indexes );
	bool						ParsePlanes( idLexer& src );
	bool						ParseVertices( idLexer& src );
	bool						ParseEdges( idLexer& src );
	bool						ParseFaces( idLexer& src );
	bool						ParseReachabilities( idLexer& src, int areaNum );
	bool						ParseAreas( idLexer& src );
	bool						ParseNodes( idLexer& src );
	bool						ParsePortals( idLexer& src );
	bool						ParseClusters( idLexer& src );

private:
	int							BoundsReachableAreaNum_r( int nodeNum, const idBounds& bounds, const int areaFlags, const int excludeTravelFlags ) const;
	void						MaxTreeDepth_r( int nodeNum, int& depth, int& maxDepth ) const;
	int							MaxTreeDepth() const;
	int							AreaContentsTravelFlags( int areaNum ) const;
	idVec3						AreaReachableGoal( int areaNum ) const;
	int							NumReachabilities() const;
};

#endif /* !__AASFILELOCAL_H__ */
