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

#ifndef __PLANESET_H__
#define __PLANESET_H__

/*
===============================================================================

	Plane Set

===============================================================================
*/

class idPlaneSet : public idList<idPlane>
{
public:

	void					Clear()
	{
		idList<idPlane>::Clear();
		hash.Free();
	}

	int						FindPlane( const idPlane& plane, const float normalEps, const float distEps );

private:
	idHashIndex				hash;
};

ID_INLINE int idPlaneSet::FindPlane( const idPlane& plane, const float normalEps, const float distEps )
{
	int i, border, hashKey;

	assert( distEps <= 0.125f );

	hashKey = ( int )( idMath::Fabs( plane.Dist() ) * 0.125f );
	for( border = -1; border <= 1; border++ )
	{
		for( i = hash.First( hashKey + border ); i >= 0; i = hash.Next( i ) )
		{
			if( ( *this )[i].Compare( plane, normalEps, distEps ) )
			{
				return i;
			}
		}
	}

	if( plane.Type() >= PLANETYPE_NEGX && plane.Type() < PLANETYPE_TRUEAXIAL )
	{
		Append( -plane );
		hash.Add( hashKey, Num() - 1 );
		Append( plane );
		hash.Add( hashKey, Num() - 1 );
		return ( Num() - 1 );
	}
	else
	{
		Append( plane );
		hash.Add( hashKey, Num() - 1 );
		Append( -plane );
		hash.Add( hashKey, Num() - 1 );
		return ( Num() - 2 );
	}
}

#endif /* !__PLANESET_H__ */
