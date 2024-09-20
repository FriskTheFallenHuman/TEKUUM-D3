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

#ifndef __BASE64_H__
#define __BASE64_H__

/*
===============================================================================

	base64

===============================================================================
*/

class idBase64
{
public:
	idBase64();
	idBase64( const idStr& s );
	~idBase64();

	void		Encode( const byte* from, int size );
	void		Encode( const idStr& src );
	int			DecodeLength() const; // minimum size in bytes of destination buffer for decoding
	int			Decode( byte* to ) const; // does not append a \0 - needs a DecodeLength() bytes buffer
	void		Decode( idStr& dest ) const; // decodes the binary content to an idStr (a bit dodgy, \0 and other non-ascii are possible in the decoded content)
	void		Decode( idFile* dest ) const;

	const char*	c_str() const;

	void 		operator=( const idStr& s );

private:
	byte* 		data;
	int			len;
	int			alloced;

	void		Init();
	void		Release();
	void		EnsureAlloced( int size );
};

ID_INLINE idBase64::idBase64()
{
	Init();
}

ID_INLINE idBase64::idBase64( const idStr& s )
{
	Init();
	*this = s;
}

ID_INLINE idBase64::~idBase64()
{
	Release();
}

ID_INLINE const char* idBase64::c_str() const
{
	return ( const char* )data;
}

ID_INLINE void idBase64::Init()
{
	len = 0;
	alloced = 0;
	data = NULL;
}

ID_INLINE void idBase64::Release()
{
	if( data )
	{
		delete[] data;
	}
	Init();
}

ID_INLINE void idBase64::EnsureAlloced( int size )
{
	if( size > alloced )
	{
		Release();
	}
	data = new byte[size];
	alloced = size;
}

ID_INLINE void idBase64::operator=( const idStr& s )
{
	EnsureAlloced( s.Length() + 1 ); // trailing \0 - beware, this does a Release
	strcpy( ( char* )data, s.c_str() );
	len = s.Length();
}

#endif /* !__BASE64_H__ */
