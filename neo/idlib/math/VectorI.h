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
#ifndef __MATH_VECTORI_H__
#define __MATH_VECTORI_H__

static ID_INLINE int MinInt( int a, int b )
{
	return ( a ) < ( b ) ? ( a ) : ( b );
}
static ID_INLINE int MaxInt( int a, int b )
{
	return ( a ) < ( b ) ? ( b ) : ( a );
}

class idVec2i
{
public:
	int      x, y;

	idVec2i() {}
	idVec2i( int _x, int _y ) : x( _x ), y( _y ) {}

	void		Set( int _x, int _y )
	{
		x = _x;
		y = _y;
	}
	int			Area() const
	{
		return x * y;
	};

	void		Min( idVec2i& v )
	{
		x = MinInt( x, v.x );
		y = MinInt( y, v.y );
	}
	void		Max( idVec2i& v )
	{
		x = MaxInt( x, v.x );
		y = MaxInt( y, v.y );
	}

	int			operator[]( const int index ) const
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}
	int& 		operator[]( const int index )
	{
		assert( index == 0 || index == 1 );
		return ( &x )[index];
	}

	idVec2i 	operator-() const
	{
		return idVec2i( -x, -y );
	}
	idVec2i 	operator!() const
	{
		return idVec2i( !x, !y );
	}

	idVec2i 	operator>>( const int a ) const
	{
		return idVec2i( x >> a, y >> a );
	}
	idVec2i 	operator<<( const int a ) const
	{
		return idVec2i( x << a, y << a );
	}
	idVec2i 	operator&( const int a ) const
	{
		return idVec2i( x & a, y & a );
	}
	idVec2i 	operator|( const int a ) const
	{
		return idVec2i( x | a, y | a );
	}
	idVec2i 	operator^( const int a ) const
	{
		return idVec2i( x ^ a, y ^ a );
	}
	idVec2i 	operator*( const int a ) const
	{
		return idVec2i( x * a, y * a );
	}
	idVec2i 	operator/( const int a ) const
	{
		return idVec2i( x / a, y / a );
	}
	idVec2i 	operator+( const int a ) const
	{
		return idVec2i( x + a, y + a );
	}
	idVec2i 	operator-( const int a ) const
	{
		return idVec2i( x - a, y - a );
	}

	bool		operator==( const idVec2i& a ) const
	{
		return a.x == x && a.y == y;
	};
	bool		operator!=( const idVec2i& a ) const
	{
		return a.x != x || a.y != y;
	};

	idVec2i		operator>>( const idVec2i& a ) const
	{
		return idVec2i( x >> a.x, y >> a.y );
	}
	idVec2i		operator<<( const idVec2i& a ) const
	{
		return idVec2i( x << a.x, y << a.y );
	}
	idVec2i		operator&( const idVec2i& a ) const
	{
		return idVec2i( x & a.x, y & a.y );
	}
	idVec2i		operator|( const idVec2i& a ) const
	{
		return idVec2i( x | a.x, y | a.y );
	}
	idVec2i		operator^( const idVec2i& a ) const
	{
		return idVec2i( x ^ a.x, y ^ a.y );
	}
	idVec2i		operator*( const idVec2i& a ) const
	{
		return idVec2i( x * a.x, y * a.y );
	}
	idVec2i		operator/( const idVec2i& a ) const
	{
		return idVec2i( x / a.x, y / a.y );
	}
	idVec2i		operator+( const idVec2i& a ) const
	{
		return idVec2i( x + a.x, y + a.y );
	}
	idVec2i		operator-( const idVec2i& a ) const
	{
		return idVec2i( x - a.x, y - a.y );
	}

	idVec2i& 	operator+=( const int a )
	{
		x += a;
		y += a;
		return *this;
	}
	idVec2i& 	operator-=( const int a )
	{
		x -= a;
		y -= a;
		return *this;
	}
	idVec2i& 	operator/=( const int a )
	{
		x /= a;
		y /= a;
		return *this;
	}
	idVec2i& 	operator*=( const int a )
	{
		x *= a;
		y *= a;
		return *this;
	}
	idVec2i& 	operator>>=( const int a )
	{
		x >>= a;
		y >>= a;
		return *this;
	}
	idVec2i& 	operator<<=( const int a )
	{
		x <<= a;
		y <<= a;
		return *this;
	}
	idVec2i& 	operator&=( const int a )
	{
		x &= a;
		y &= a;
		return *this;
	}
	idVec2i& 	operator|=( const int a )
	{
		x |= a;
		y |= a;
		return *this;
	}
	idVec2i& 	operator^=( const int a )
	{
		x ^= a;
		y ^= a;
		return *this;
	}

	idVec2i& 	operator>>=( const idVec2i& a )
	{
		x >>= a.x;
		y >>= a.y;
		return *this;
	}
	idVec2i& 	operator<<=( const idVec2i& a )
	{
		x <<= a.x;
		y <<= a.y;
		return *this;
	}
	idVec2i& 	operator&=( const idVec2i& a )
	{
		x &= a.x;
		y &= a.y;
		return *this;
	}
	idVec2i& 	operator|=( const idVec2i& a )
	{
		x |= a.x;
		y |= a.y;
		return *this;
	}
	idVec2i& 	operator^=( const idVec2i& a )
	{
		x ^= a.x;
		y ^= a.y;
		return *this;
	}
	idVec2i& 	operator+=( const idVec2i& a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}
	idVec2i& 	operator-=( const idVec2i& a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
	idVec2i& 	operator/=( const idVec2i& a )
	{
		x /= a.x;
		y /= a.y;
		return *this;
	}
	idVec2i& 	operator*=( const idVec2i& a )
	{
		x *= a.x;
		y *= a.y;
		return *this;
	}
};

#endif
