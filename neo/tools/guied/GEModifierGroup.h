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
#ifndef GEMODIFIERGROUP_H_
#define GEMODIFIERGROUP_H_

#ifndef GEMODIFIER_H_
	#include "GEModifier.h"
#endif

class rvGEModifierGroup : public rvGEModifier
{
public:

	rvGEModifierGroup( );
	~rvGEModifierGroup( );

	virtual bool		Apply();
	virtual bool		Undo();

	virtual bool		CanMerge( rvGEModifier* merge );

	virtual bool		Merge( rvGEModifier* merge );

	virtual bool		IsValid();

	bool				Append( rvGEModifier* mod );
	int					GetCount();


protected:

	idList<rvGEModifier*>	mModifiers;

};

ID_INLINE int rvGEModifierGroup::GetCount()
{
	return mModifiers.Num( );
}

#endif