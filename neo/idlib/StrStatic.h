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
#ifndef	__STRSTATIC_H__
#define	__STRSTATIC_H__

/*
================================================
idStrStatic
================================================
*/
template< int _size_ >
class idStrStatic : public idStr
{
public:
	ID_INLINE void operator=( const idStrStatic& text )
	{
		// we should only get here when the types, including the size, are identical
		len = text.Length();
		memcpy( data, text.data, len + 1 );
	}

	// all idStr operators are overloaded and the idStr default constructor is called so that the
	// static buffer can be initialized in the body of the constructor before the data is ever
	// copied.
	ID_INLINE	idStrStatic()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
	}
	ID_INLINE	idStrStatic( const idStrStatic& text ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	ID_INLINE	idStrStatic( const idStr& text ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	ID_INLINE	idStrStatic( const idStrStatic& text, int start, int end ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text.c_str(), start, end );
	}

	ID_INLINE	idStrStatic( const char* text ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( text );
	}

	ID_INLINE	idStrStatic( const char* text, int start, int end ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		CopyRange( text, start, end );
	}

	ID_INLINE	explicit idStrStatic( const bool b ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( b ) );
	}

	ID_INLINE	explicit idStrStatic( const char c ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( c ) );
	}

	ID_INLINE	explicit idStrStatic( const int i ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( i ) );
	}

	ID_INLINE	explicit idStrStatic( const unsigned u ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( u ) );
	}

	ID_INLINE	explicit idStrStatic( const float f ) :
		idStr()
	{
		buffer[ 0 ] = '\0';
		SetStaticBuffer( buffer, _size_ );
		idStr::operator=( idStr( f ) );
	}

private:
	char		buffer[ _size_ ];
};
#endif	// __STRSTATIC_H__ 
