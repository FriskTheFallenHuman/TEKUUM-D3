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

#ifndef GESELECTIONMGR_H_
#define GESELECTIONMGR_H_

class rvGEWorkspace;

class rvGESelectionMgr
{
public:

	enum EHitTest
	{
		HT_NONE,
		HT_SELECT,
		HT_MOVE,
		HT_SIZE_TOPLEFT,
		HT_SIZE_TOP,
		HT_SIZE_TOPRIGHT,
		HT_SIZE_RIGHT,
		HT_SIZE_BOTTOMRIGHT,
		HT_SIZE_BOTTOM,
		HT_SIZE_BOTTOMLEFT,
		HT_SIZE_LEFT
	};

	rvGESelectionMgr( );

	void			SetWorkspace( rvGEWorkspace* workspace );

	void			Set( idWindow* );
	void			Add( idWindow* window, bool expand = true );
	void			Remove( idWindow* );
	void			Clear();

	int				Num();

	void			Render();

	EHitTest		HitTest( float x, float y );

	bool			IsSelected( idWindow* window );
	bool			IsExpression();

	idRectangle&	GetRect();
	idWindow*		GetBottomMost();

	idWindow*&		operator[]( int index );

protected:

	void		UpdateRectangle();
	void		UpdateExpression();

	idList<idWindow*>	mSelections;
	idRectangle			mRect;
	rvGEWorkspace*		mWorkspace;
	bool				mExpression;
};

ID_INLINE int rvGESelectionMgr::Num()
{
	return mSelections.Num( );
}

ID_INLINE idWindow*& rvGESelectionMgr::operator[]( int index )
{
	assert( index >= 0 );
	assert( index < mSelections.Num() );

	return mSelections[ index ];
}

ID_INLINE void rvGESelectionMgr::SetWorkspace( rvGEWorkspace* workspace )
{
	mWorkspace = workspace;
}

ID_INLINE idRectangle& rvGESelectionMgr::GetRect()
{
	UpdateRectangle( );
	return mRect;
}

ID_INLINE bool rvGESelectionMgr::IsSelected( idWindow* window )
{
	return mSelections.FindIndex( window ) != -1 ? true : false;
}

ID_INLINE bool rvGESelectionMgr::IsExpression()
{
	return mExpression;
}

#endif // GESELECTIONMGR_H_
