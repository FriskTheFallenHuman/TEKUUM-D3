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

#ifndef __DECLSKIN_H__
#define __DECLSKIN_H__

/*
===============================================================================

	idDeclSkin

===============================================================================
*/

typedef struct
{
	const idMaterial* 		from;			// 0 == any unmatched shader
	const idMaterial* 		to;
} skinMapping_t;

class idDeclSkin : public idDecl
{
public:
	virtual size_t			Size() const;
	virtual bool			SetDefaultText();
	virtual const char* 	DefaultDefinition() const;
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion );
	virtual void			FreeData();

	const idMaterial* 		RemapShaderBySkin( const idMaterial* shader ) const;

	// model associations are just for the preview dialog in the editor
	const int				GetNumModelAssociations() const;
	const char* 			GetAssociatedModel( int index ) const;

private:
	idList<skinMapping_t>	mappings;
	idStrList				associatedModels;
};

#endif /* !__DECLSKIN_H__ */
