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
#ifndef __ARRAY_H__
#define __ARRAY_H__

/*
================================================
idArray is a replacement for a normal C array.

int		myArray[ARRAY_SIZE];

becomes:

idArray<int,ARRAY_SIZE>	myArray;

Has no performance overhead in release builds, but
does index range checking in debug builds.

Unlike idTempArray, the memory is allocated inline with the
object, rather than on the heap.

Unlike idStaticList, there are no fields other than the
actual raw data, and the size is fixed.
================================================
*/
template<class T_, int numElements > class idArray
{
public:
	// returns number of elements in list
	int				Num() const
	{
		return numElements;
	}

	// returns the number of bytes the array takes up
	int				ByteSize() const
	{
		return sizeof( ptr );
	}

	// memset the entire array to zero
	void			Zero()
	{
		memset( ptr, 0, sizeof( ptr ) );
	}

	// memset the entire array to a specific value
	void			Memset( const char fill )
	{
		memset( ptr, fill, numElements * sizeof( *ptr ) );
	}

	// array operators
	const T_& 		operator[]( int index ) const
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}
	T_& 			operator[]( int index )
	{
		assert( ( unsigned )index < ( unsigned )numElements );
		return ptr[index];
	}

	// returns a pointer to the list
	const T_* 		Ptr() const
	{
		return ptr;
	}
	T_* 			Ptr()
	{
		return ptr;
	}

private:
	T_				ptr[numElements];
};

#define ARRAY_COUNT( arrayName ) ( sizeof( arrayName )/sizeof( arrayName[0] ) )
#define ARRAY_DEF( arrayName ) arrayName, ARRAY_COUNT( arrayName )


/*
================================================
id2DArray is essentially a typedef (as close as we can
get for templates before C++11 anyway) to make
declaring two-dimensional idArrays easier.

Usage:
	id2DArray< int, 5, 10 >::type someArray;

================================================
*/
template<class _type_, int _dim1_, int _dim2_ >
struct id2DArray
{
	typedef idArray< idArray< _type_, _dim2_ >, _dim1_ > type;
};


/*
================================================
idTupleSize
Generic way to get the size of a tuple-like type.
Add specializations as needed.
This is modeled after std::tuple_size from C++11,
which works for std::arrays also.
================================================
*/
template< class _type_ >
struct idTupleSize;

template< class _type_, int _num_ >
struct idTupleSize< idArray< _type_, _num_ > >
{
	enum { value = _num_ };
};

#endif // !__ARRAY_H__
