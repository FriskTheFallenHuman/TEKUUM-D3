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

#ifndef __LANGDICT_H__
#define __LANGDICT_H__

/*
===============================================================================

	Simple dictionary specifically for the localized string tables.

===============================================================================
*/

class idLangKeyValue
{
public:
	idStr					key;
	idStr					value;
};

class idLangDict
{
public:
	idLangDict();
	~idLangDict();

	void					Clear();
	bool					Load( const char* fileName, bool clear = true );
	void					Save( const char* fileName );

	const char* 			AddString( const char* str );
	const char* 			GetString( const char* str ) const;

	// adds the value and key as passed (doesn't generate a "#str_xxxxx" key or ensure the key/value pair is unique)
	void					AddKeyVal( const char* key, const char* val );

	int						GetNumKeyVals() const;
	const idLangKeyValue* 	GetKeyVal( int i ) const;

	void					SetBaseID( int id )
	{
		baseID = id;
	};

private:
	idList<idLangKeyValue>	args;
	idHashIndex				hash;

	bool					ExcludeString( const char* str ) const;
	int						GetNextId() const;
	int						GetHashKey( const char* str ) const;

	int						baseID;
};

#endif /* !__LANGDICT_H__ */
