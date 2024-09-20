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

#ifndef GEWINDOWWRAPPER_H_
#define GEWINDOWWRAPPER_H_

class idWindow;
class rvGEWindowWrapper;

typedef bool ( *PFNENUMCHILDRENPROC )( rvGEWindowWrapper* wrapper, void* data );

class rvGEWindowWrapper
{
public:

	enum EWindowType
	{
		WT_UNKNOWN,
		WT_NORMAL,
		WT_EDIT,
		WT_HTML,
		WT_CHOICE,
		WT_SLIDER,
		WT_BIND,
		WT_LIST,
		WT_RENDER,
		WT_TRANSITION
	};

	rvGEWindowWrapper( idWindow* window, EWindowType type );

	static rvGEWindowWrapper*	GetWrapper( idWindow* window );

	idWindow*			GetWindow();
	idDict&				GetStateDict();
	idDict&				GetVariableDict();
	idDict&				GetScriptDict();
	idRectangle&		GetClientRect();
	idRectangle&		GetScreenRect();
	EWindowType			GetWindowType();
	int					GetChildCount();
	int					GetDepth();
	idWindow*			GetChild( int index );

	void				SetRect( idRectangle& rect );
	void				SetState( const idDict& dict );
	void				SetStateKey( const char* key, const char* value, bool update = true );
	void				DeleteStateKey( const char* key );
	bool				VerfiyStateKey( const char* name, const char* value, idStr* result = NULL );

	bool				IsFlippedHorz();
	bool				IsFlippedVert();
	bool				IsHidden();
	bool				IsDeleted();
	bool				IsSelected();
	bool				IsExpanded();

	bool				CanHaveChildren();
	bool				CanMoveAndSize();

	void				SetFlippedHorz( bool f );
	void				SetFlippedVert( bool f );
	void				SetHidden( bool v );
	void				SetDeleted( bool del );
	void				SetSelected( bool sel );
	void				SetWindowType( EWindowType type );

	bool				Expand();
	bool				Collapse();

	bool				EnumChildren( PFNENUMCHILDRENPROC proc, void* data );

	idWindow*			WindowFromPoint( float x, float y, bool visibleOnly = true );

	void				Finish();

	static EWindowType	StringToWindowType( const char* string );
	static const char*	WindowTypeToString( EWindowType type );

protected:

	void				CalcScreenRect();
	void				UpdateRect();
	void				UpdateWindowState();

	idRectangle		mClientRect;
	idRectangle		mScreenRect;
	idWindow*		mWindow;
	idDict			mState;
	idDict			mVariables;
	idDict			mScripts;
	bool			mFlippedHorz;
	bool			mFlippedVert;
	bool			mHidden;
	bool			mDeleted;
	bool			mSelected;
	bool			mExpanded;
	bool			mOldVisible;
	bool			mMoveable;
	EWindowType		mType;
};

ID_INLINE idDict& rvGEWindowWrapper::GetStateDict()
{
	return mState;
}

ID_INLINE idDict& rvGEWindowWrapper::GetVariableDict()
{
	return mVariables;
}

ID_INLINE idDict& rvGEWindowWrapper::GetScriptDict()
{
	return mScripts;
}

ID_INLINE bool rvGEWindowWrapper::IsFlippedHorz()
{
	return mFlippedHorz;
}

ID_INLINE bool rvGEWindowWrapper::IsFlippedVert()
{
	return mFlippedVert;
}

ID_INLINE bool rvGEWindowWrapper::IsExpanded()
{
	return mExpanded;
}

ID_INLINE void rvGEWindowWrapper::SetFlippedHorz( bool f )
{
	mFlippedHorz = f;
}

ID_INLINE void rvGEWindowWrapper::SetFlippedVert( bool f )
{
	mFlippedVert = f;
}

ID_INLINE idRectangle& rvGEWindowWrapper::GetClientRect()
{
	return mClientRect;
}

ID_INLINE idRectangle& rvGEWindowWrapper::GetScreenRect()
{
	return mScreenRect;
}

ID_INLINE bool rvGEWindowWrapper::IsHidden()
{
	return mHidden;
}

ID_INLINE bool rvGEWindowWrapper::IsDeleted()
{
	return mDeleted;
}

ID_INLINE bool rvGEWindowWrapper::IsSelected()
{
	return mSelected;
}

ID_INLINE void rvGEWindowWrapper::SetSelected( bool sel )
{
	mSelected = sel;
}

ID_INLINE rvGEWindowWrapper::EWindowType rvGEWindowWrapper::GetWindowType()
{
	return mType;
}

ID_INLINE void rvGEWindowWrapper::SetWindowType( rvGEWindowWrapper::EWindowType type )
{
	mType = type;
}

ID_INLINE idWindow* rvGEWindowWrapper::GetChild( int index )
{
	return mWindow->GetChild( index );
}

ID_INLINE idWindow* rvGEWindowWrapper::GetWindow()
{
	return mWindow;
}

ID_INLINE bool rvGEWindowWrapper::CanMoveAndSize()
{
	return mMoveable;
}

#endif // GEWINDOWWRAPPER_H_
