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
#ifndef GEMODIFIERSTACK_H_
#define GEMODIFIERSTACK_H_

#ifndef GEMODIFIER_H_
	#include "GEModifier.h"
#endif

class rvGEModifierStack
{
public:

	rvGEModifierStack( );
	~rvGEModifierStack( );

	void			BlockNextMerge();

	bool			Append( rvGEModifier* modifier );
	bool			Undo();
	bool			Redo();

	void			Reset();

	bool			CanUndo();
	bool			CanRedo();

	rvGEModifier*	GetUndoModifier();
	rvGEModifier*	GetRedoModifier();

protected:

	idList<rvGEModifier*>	mModifiers;
	int						mCurrentModifier;
	bool					mMergeBlock;
};

ID_INLINE bool rvGEModifierStack::CanRedo()
{
	return mCurrentModifier < mModifiers.Num() - 1;
}

ID_INLINE bool rvGEModifierStack::CanUndo()
{
	return mCurrentModifier >= 0;
}

ID_INLINE void rvGEModifierStack::BlockNextMerge()
{
	mMergeBlock = true;
}

ID_INLINE rvGEModifier* rvGEModifierStack::GetUndoModifier()
{
	assert( CanUndo( ) );
	return mModifiers[mCurrentModifier];
}

ID_INLINE rvGEModifier* rvGEModifierStack::GetRedoModifier()
{
	assert( CanRedo( ) );
	return mModifiers[mCurrentModifier + 1];
}

#endif
