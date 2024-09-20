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

#ifndef __STACK_H__
#define __STACK_H__

/*
===============================================================================

	Stack template

===============================================================================
*/

#define idStack( type, next )		idStackTemplate<type, (int)&(((type*)NULL)->next)>

template< class type, int nextOffset >
class idStackTemplate
{
public:
	idStackTemplate();

	void					Add( type* element );
	type* 					Get();

private:
	type* 					top;
	type* 					bottom;
};

#define STACK_NEXT_PTR( element )		(*(type**)(((byte*)element)+nextOffset))

template< class type, int nextOffset >
idStackTemplate<type, nextOffset>::idStackTemplate()
{
	top = bottom = NULL;
}

template< class type, int nextOffset >
void idStackTemplate<type, nextOffset>::Add( type* element )
{
	STACK_NEXT_PTR( element ) = top;
	top = element;
	if( !bottom )
	{
		bottom = element;
	}
}

template< class type, int nextOffset >
type* idStackTemplate<type, nextOffset>::Get()
{
	type* element;

	element = top;
	if( element )
	{
		top = STACK_NEXT_PTR( top );
		if( bottom == element )
		{
			bottom = NULL;
		}
		STACK_NEXT_PTR( element ) = NULL;
	}
	return element;
}

#endif /* !__STACK_H__ */
