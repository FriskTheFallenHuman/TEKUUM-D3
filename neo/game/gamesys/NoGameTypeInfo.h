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

#ifndef __GAMETYPEINFO_H__
#define __GAMETYPEINFO_H__

/*
===================================================================================

	This file has been generated with the Type Info Generator v1.0 (c) 2004 id Software

===================================================================================
*/

typedef struct
{
	const char* name;
	const char* type;
	const char* value;
} constantInfo_t;

typedef struct
{
	const char* name;
	int value;
} enumValueInfo_t;

typedef struct
{
	const char* typeName;
	const enumValueInfo_t* values;
} enumTypeInfo_t;

typedef struct
{
	const char* type;
	const char* name;
	int offset;
	int size;
} classVariableInfo_t;

typedef struct
{
	const char* typeName;
	const char* superType;
	int size;
	const classVariableInfo_t* variables;
} classTypeInfo_t;


static enumTypeInfo_t enumTypeInfo[] =
{
	{ NULL, NULL }
};

static classTypeInfo_t classTypeInfo[] =
{
	{ NULL, NULL, 0, NULL }
};

#endif /* !__GAMETYPEINFO_H__ */
