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

#ifndef __BOUNDSTRACK_H__
#define __BOUNDSTRACK_H__


struct shortBounds_t;

class idBoundsTrack
{
public:
	idBoundsTrack();
	~idBoundsTrack();

	void ClearAll();

	// more than this will thrash a 32k L1 data cache
	static const int MAX_BOUNDS_TRACK_INDEXES = 2048;

	// the bounds will be clamped and rounded to short integers
	void	SetIndex( const int index, const idBounds& bounds );

	// an index that has been cleared will never be returned by FindIntersections()
	void	ClearIndex( const int index );

	// returns the number of indexes filled in intersectedIndexes[]
	//
	// The intersections may include some bounds that are not truly overlapping
	// due to the rounding from float to short integers.
	int		FindIntersections( const idBounds& testBounds, int intersectedIndexes[ MAX_BOUNDS_TRACK_INDEXES ] ) const;

	// validate implementation
	void	Test();

private:
	// All elements that haven't had SetIndex() called since ClearAll() will be
	// in the cleared state, so they can safely be compared against by an
	// unwound loop.
	shortBounds_t* boundsList;	// [MAX_BOUNDS_TRACK_INDEXES]

	// set to 0 at ClearAll(), maintained greater than the highest index passed
	// to SetIndex() since ClearAll().
	int				maxIndex;
};


#endif // __BOUNDSTRACK_H__
