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
#ifndef GEMODIFIER_H_
#define GEMODIFIER_H_

class idWindow;
class rvGEWindowWrapper;

class rvGEModifier
{
public:

	rvGEModifier( const char* name, idWindow* window );
	virtual ~rvGEModifier( ) { }

	virtual bool		Apply() = 0;
	virtual bool		Undo() = 0;
	virtual const char*	GetName();
	virtual bool		CanMerge( rvGEModifier* merge );

	virtual bool		IsValid();

	virtual bool		Merge( rvGEModifier* merge );

	idWindow*			GetWindow();


protected:

	idWindow*			mWindow;
	rvGEWindowWrapper*	mWrapper;
	idStr				mName;
};

ID_INLINE bool rvGEModifier::IsValid()
{
	return true;
}

ID_INLINE idWindow* rvGEModifier::GetWindow()
{
	return mWindow;
}

ID_INLINE const char* rvGEModifier::GetName()
{
	return mName;
}

ID_INLINE bool rvGEModifier::CanMerge( rvGEModifier* merge )
{
	return false;
}

ID_INLINE bool rvGEModifier::Merge( rvGEModifier* merge )
{
	return false;
}

#endif // GEMODIFIER_H_
