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

#include "precompiled.h"
#pragma hdrstop


/*
=================
idDeclSkin::Size
=================
*/
size_t idDeclSkin::Size() const
{
	return sizeof( idDeclSkin );
}

/*
================
idDeclSkin::FreeData
================
*/
void idDeclSkin::FreeData()
{
	mappings.Clear();
}

/*
================
idDeclSkin::Parse
================
*/
bool idDeclSkin::Parse( const char* text, const int textLength, bool allowBinaryVersion )
{
	idLexer src;
	idToken	token, token2;

	src.LoadMemory( text, textLength, GetFileName(), GetLineNum() );
	src.SetFlags( DECL_LEXER_FLAGS );
	src.SkipUntilString( "{" );

	associatedModels.Clear();

	while( 1 )
	{
		if( !src.ReadToken( &token ) )
		{
			break;
		}

		if( !token.Icmp( "}" ) )
		{
			break;
		}
		if( !src.ReadToken( &token2 ) )
		{
			src.Warning( "Unexpected end of file" );
			MakeDefault();
			return false;
		}

		if( !token.Icmp( "model" ) )
		{
			associatedModels.Append( token2 );
			continue;
		}

		skinMapping_t	map;

		if( !token.Icmp( "*" ) )
		{
			// wildcard
			map.from = NULL;
		}
		else
		{
			map.from = declManager->FindMaterial( token );
		}

		map.to = declManager->FindMaterial( token2 );

		mappings.Append( map );
	}

	return false;
}

/*
================
idDeclSkin::SetDefaultText
================
*/
bool idDeclSkin::SetDefaultText()
{
	// if there exists a material with the same name
	if( declManager->FindType( DECL_MATERIAL, GetName(), false ) )
	{
		char generated[2048];

		idStr::snPrintf( generated, sizeof( generated ),
						 "skin %s // IMPLICITLY GENERATED\n"
						 "{\n"
						 "_default %s\n"
						 "}\n", GetName(), GetName() );
		SetText( generated );
		return true;
	}
	else
	{
		return false;
	}
}

/*
================
idDeclSkin::DefaultDefinition
================
*/
const char* idDeclSkin::DefaultDefinition() const
{
	return
		"{\n"
		"\t"	"\"*\"\t\"_default\"\n"
		"}";
}

/*
================
idDeclSkin::GetNumModelAssociations
================
*/
const int idDeclSkin::GetNumModelAssociations() const
{
	return associatedModels.Num();
}

/*
================
idDeclSkin::GetAssociatedModel
================
*/
const char* idDeclSkin::GetAssociatedModel( int index ) const
{
	if( index >= 0 && index < associatedModels.Num() )
	{
		return associatedModels[ index ];
	}
	return "";
}

/*
===============
RemapShaderBySkin
===============
*/
const idMaterial* idDeclSkin::RemapShaderBySkin( const idMaterial* shader ) const
{
	int		i;

	if( !shader )
	{
		return NULL;
	}

	// never remap surfaces that were originally nodraw, like collision hulls
	if( !shader->IsDrawn() )
	{
		return shader;
	}

	for( i = 0; i < mappings.Num() ; i++ )
	{
		const skinMapping_t*	map = &mappings[i];

		// NULL = wildcard match
		if( !map->from || map->from == shader )
		{
			return map->to;
		}
	}

	// didn't find a match or wildcard, so stay the same
	return shader;
}
