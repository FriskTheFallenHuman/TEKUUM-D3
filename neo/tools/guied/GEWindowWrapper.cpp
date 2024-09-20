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


#include "../../sys/win32/rc/resource.h"
#include "../../renderer/RenderCommon.h"
#include "../../ui/EditWindow.h"
#include "../../ui/ListWindow.h"
#include "../../ui/BindWindow.h"
#include "../../ui/RenderWindow.h"
#include "../../ui/ChoiceWindow.h"

#include "GEApp.h"

rvGEWindowWrapper::rvGEWindowWrapper( idWindow* window, EWindowType type )
{
	assert( window );

	mWindow = window;
	mFlippedHorz = false;
	mFlippedVert = false;
	mHidden = false;
	mDeleted = false;
	mSelected = false;
	mType = type;
	mExpanded = false;
	mOldVisible = false;
	mMoveable = true;

	if( dynamic_cast< idEditWindow*>( window ) )
	{
		mType = WT_EDIT;
	}
	else if( dynamic_cast< idListWindow*>( window ) )
	{
		mType = WT_LIST;
	}
	else if( dynamic_cast< idBindWindow*>( window ) )
	{
		mType = WT_BIND;
	}
	else if( dynamic_cast< idRenderWindow*>( window ) )
	{
		mType = WT_RENDER;
	}
	else if( dynamic_cast< idChoiceWindow*>( window ) )
	{
		mType = WT_CHOICE;
	}
	else
	{
		mType = WT_NORMAL;
	}

	// Attach the wrapper to the window by adding a defined variable
	// with the wrappers pointer stuffed into (an integer) - actually a string now
#if 0
	idWinInt* var = new idWinInt();
	int x = ( int )this;
	*var = x;
#else // DG: use idWinStr, because idWinInt can't cold 64bit pointers
	idWinStr* var = new idWinStr();

	// convert this to hex-string (*without* "0x" prefix)
	const ULONG_PTR thisULP = ( ULONG_PTR )this;
	char buf[32] = {0};
	_ui64toa( thisULP, buf, 16 );

	var->Set( buf );
#endif
	var->SetEval( false );
	var->SetName( "guied_wrapper" );
	mWindow->AddDefinedVar( var );

	SetStateKey( "name", mWindow->GetName(), false );
}

/*
================
rvGEWindowWrapper::GetWrapper

Static method that returns the window wrapper for the given window class
================
*/
rvGEWindowWrapper* rvGEWindowWrapper::GetWrapper( idWindow* window )
{
#if 0
	idWinInt* var;
	var = dynamic_cast< idWinInt*>( window->GetWinVarByName( "guied_wrapper" ) );
	return var ? ( ( rvGEWindowWrapper* )( int )( *var ) ) : NULL;
#else
	// DG: use idWinStr, because idWinInt can't cold 64bit pointers
	idWinStr* var = ( idWinStr* )window->GetWinVarByName( "guied_wrapper" );
	if( var == NULL )
	{
		return NULL;
	}

	ULONG_PTR thisULP = ( ULONG_PTR )_strtoui64( var->c_str(), NULL, 16 );
	return ( rvGEWindowWrapper* )thisULP;
#endif
}

/*
================
rvGEWindowWrapper::UpdateRect

Updates the gui editor's representation of the window rectangle from the
windows rectangle
================
*/
void rvGEWindowWrapper::UpdateRect()
{
	idVec4 rect;
	idWinRectangle* winrect;

	winrect = dynamic_cast< idWinRectangle*>( mWindow->GetWinVarByName( "rect" ) );
	assert( winrect );
	rect = winrect->ToVec4();

	mFlippedHorz = false;
	mFlippedVert = false;

	if( rect[2] < 0 )
	{
		mFlippedHorz = true;
		rect[2] *= -1;
	}

	if( rect[3] < 0 )
	{
		mFlippedVert = true;
		rect[3] *= -1;
	}

	mClientRect = rect;

	CalcScreenRect();

	const char* rstr = mState.GetString( "rect", "0,0,0,0" );
	mMoveable = !IsExpression( rstr );
}

/*
================
rvGEWindowWrapper::CalcScreenRect

Calculates the screen rectangle from the client rectangle by running through
the parent windows and adding their offsets
================
*/
void rvGEWindowWrapper::CalcScreenRect()
{
	idWindow* parent;

	mScreenRect = mClientRect;

	if( NULL != ( parent = mWindow->GetParent() ) )
	{
		rvGEWindowWrapper* wrapper = GetWrapper( parent );
		assert( wrapper );

		mScreenRect[0] += wrapper->GetScreenRect()[0];
		mScreenRect[1] += wrapper->GetScreenRect()[1];
	}

	// Since our screen rectangle has changed we need to tell all our our children to update
	// their screen rectangles as well.
	int i;
	int count;
	rvGEWindowWrapper* pwrapper;

	pwrapper = rvGEWindowWrapper::GetWrapper( mWindow );

	for( count = pwrapper->GetChildCount(), i = 0; i < count; i ++ )
	{
		rvGEWindowWrapper* wrapper;

		wrapper = rvGEWindowWrapper::GetWrapper( pwrapper->GetChild( i ) );

		// Usually we assert if there is no wrapper but since this method is called
		// when the wrappers are being attached to the windows there may be no wrappers
		// for any of the children.
		if( !wrapper )
		{
			continue;
		}

		wrapper->CalcScreenRect();
	}
}

/*
================
rvGEWindowWrapper::SetRect

Sets the wrapper's rectangle and the attached windows rectangle as well
================
*/
void rvGEWindowWrapper::SetRect( idRectangle& rect )
{
	const char* s;

	mClientRect = rect;
	CalcScreenRect();

	s = va( "%d,%d,%d,%d", ( int )( rect.x + 0.5f ), ( int )( rect.y + 0.5f ), ( int )( ( rect.w + 0.5f ) * ( mFlippedHorz ? -1 : 1 ) ), ( int )( ( rect.h + 0.5f ) * ( mFlippedVert ? -1 : 1 ) ) );

	mState.Set( "rect", s );

	UpdateWindowState();
}

/*
================
rvGEWindowWrapper::SetHidden

Sets the wrappers hidden state
================
*/
void rvGEWindowWrapper::SetHidden( bool h )
{
	mHidden = h;

	UpdateWindowState();
}

/*
================
rvGEWindowWrapper::SetDeleted

Sets the deleted state of the wrapper which in turns sets whether or
not the window is visible
================
*/
void rvGEWindowWrapper::SetDeleted( bool del )
{
	mDeleted = del;

	UpdateWindowState();
}

/*
================
rvGEWindowWrapper::SetState

Sets the window state from the given dictionary
================
*/
void rvGEWindowWrapper::SetState( const idDict& dict )
{
	mState.Clear();
	mState.Copy( dict );

	// Push the window state to the window itself
	UpdateWindowState();

	// Update the internal rectangle since it may have changed in the state
	UpdateRect();
}

/*
================
rvGEWindowWrapper::SetStateKey

Sets the given state key and updates the
================
*/
void rvGEWindowWrapper::SetStateKey( const char* key, const char* value, bool update )
{
	mState.Set( key, value );

	if( update )
	{
		UpdateWindowState();

		// Make sure the rectangle gets updated if its changing
		if( !idStr::Icmp( key, "rect" ) )
		{
			UpdateRect();
		}
	}
}

/*
================
rvGEWindowWrapper::DeleteStateKey

Sets the given state key and updates the
================
*/
void rvGEWindowWrapper::DeleteStateKey( const char* key )
{
	if( !idStr::Icmp( key, "rect" ) || !idStr::Icmp( key, "name" ) )
	{
		return;
	}

	mState.Delete( key );
}

/*
================
rvGEWindowWrapper::UpdateWindowState

Updates the windows real state with wrappers internal state.  Visibility is
handled specially
================
*/
void rvGEWindowWrapper::UpdateWindowState()
{
	idStr realVisible;
	bool tempVisible;
	//	int	  i;

	realVisible = mState.GetString( "visible", "1" );
	tempVisible = atoi( realVisible ) ? true : false;
	if( tempVisible != mOldVisible )
	{
		mHidden = !tempVisible;
		mOldVisible = tempVisible;
	}

	tempVisible = !mDeleted && !mHidden;

	// Temporarily change the visible state so we can hide/unhide the window
	mState.Set( "visible", tempVisible ? "1" : "0" );

	mWindow->UpdateFromDictionary( mState );

	// Put the visible state back to the real value
	mState.Set( "visible", realVisible );
}

/*
================
rvGEWindowWrapper::WindowFromPoint

Returns the topmost window under the given point
================
*/
idWindow* rvGEWindowWrapper::WindowFromPoint( float x, float y, bool visibleOnly )
{
	int count;
	int i;
	rvGEWindowWrapper* pwrapper;

	// If the window isnt visible then skip it
	if( visibleOnly && ( mHidden || mDeleted ) )
	{
		return NULL;
	}

	// Now check our children next
	pwrapper = GetWrapper( mWindow );
	count = pwrapper->GetChildCount();

	for( i = count - 1; i >= 0; i -- )
	{
		rvGEWindowWrapper* wrapper;
		idWindow* child;

		child = pwrapper->GetChild( i );
		assert( child );

		wrapper = rvGEWindowWrapper::GetWrapper( child );
		if( !wrapper )
		{
			continue;
		}

		if( child = wrapper->WindowFromPoint( x, y ) )
		{
			return child;
		}
	}

	// We have to check this last because a child could be out outside of the parents
	// rectangle and we still want it selectable.
	if( !mScreenRect.Contains( x, y ) )
	{
		return NULL;
	}

	return mWindow;
}

/*
================
rvGEWindowWrapper::StringToWindowType

Converts the given string to a window type
================
*/
rvGEWindowWrapper::EWindowType rvGEWindowWrapper::StringToWindowType( const char* string )
{
	if( !idStr::Icmp( string, "windowDef" ) )
	{
		return WT_NORMAL;
	}
	else if( !idStr::Icmp( string, "editDef" ) )
	{
		return WT_EDIT;
	}
	else if( !idStr::Icmp( string, "choiceDef" ) )
	{
		return WT_CHOICE;
	}
	else if( !idStr::Icmp( string, "sliderDef" ) )
	{
		return WT_SLIDER;
	}
	else if( !idStr::Icmp( string, "bindDef" ) )
	{
		return WT_BIND;
	}
	else if( !idStr::Icmp( string, "listDef" ) )
	{
		return WT_LIST;
	}
	else if( !idStr::Icmp( string, "renderDef" ) )
	{
		return WT_RENDER;
	}
	else if( !idStr::Icmp( string, "htmlDef" ) )
	{
		return WT_HTML;
	}

	return WT_UNKNOWN;
}

/*
================
rvGEWindowWrapper::WindowTypeToString

Converts the given window type to a string
================
*/
const char* rvGEWindowWrapper::WindowTypeToString( EWindowType type )
{
	static const char* typeNames[] =
	{
		"Unknown", "windowDef", "editDef", "htmlDef", "choiceDef", "sliderDef", "bindDef", "listDef", "renderDef"
	};

	return typeNames[( int ) type];
}

/*
================
rvGEWindowWrapper::GetChildCount

Returns the number of children the window being wrapped has
================
*/
int rvGEWindowWrapper::GetChildCount()
{
	if( !CanHaveChildren() )
	{
		return 0;
	}

	return mWindow->GetChildCount();
}

/*
================
rvGEWindowWrapper::EnumChildren

Enumerates over the child windows while properly ignoring any that
are not wrapped.
================
*/
bool rvGEWindowWrapper::EnumChildren( PFNENUMCHILDRENPROC proc, void* data )
{
	int count;
	int i;

	if( !CanHaveChildren() )
	{
		return false;
	}

	for( count = GetChildCount(), i = 0; i < count; i ++ )
	{
		if( !proc( rvGEWindowWrapper::GetWrapper( GetChild( i ) ), data ) )
		{
			return false;
		}
	}

	return true;
}

/*
================
rvGEWindowWrapper::CanHaveChildren

Returns true if the window is allowed to have child windows
================
*/
bool rvGEWindowWrapper::CanHaveChildren()
{
	if( mType == WT_HTML || mType == WT_LIST )
	{
		return false;
	}

	return true;
}

/*
================
rvGEWindowWrapper::GetDepth

Returns the depth of the wrapped window
================
*/
int rvGEWindowWrapper::GetDepth()
{
	idWindow* parent;
	int depth;

	for( depth = 0, parent = mWindow->GetParent(); parent; parent = parent->GetParent(), depth++ )
		;

	return depth;
}

/*
================
rvGEWindowWrapper::Expand

Expand the window in the navigator and all of its parents too
================
*/
bool rvGEWindowWrapper::Expand()
{
	bool result;

	if( mWindow->GetParent() )
	{
		result = rvGEWindowWrapper::GetWrapper( mWindow->GetParent() )->Expand();
	}
	else
	{
		result = false;
	}

	if( mExpanded || !CanHaveChildren() || !GetChildCount() )
	{
		return result;
	}

	mExpanded = true;

	return true;
}

/*
================
rvGEWindowWrapper::Collapse

Returns the depth of the wrapped window
================
*/
bool rvGEWindowWrapper::Collapse()
{
	bool result;

	if( mWindow->GetParent() )
	{
		result = rvGEWindowWrapper::GetWrapper( mWindow->GetParent() )->Expand();
	}
	else
	{
		result = false;
	}

	if( !mExpanded )
	{
		return result;
	}

	mExpanded = false;

	return true;
}

/*
================
rvGEWindowWrapper::Finish

After a the windwo wrapper is attached to a window and the window is parsed
the finish method is called to finish up any last details
================
*/
void rvGEWindowWrapper::Finish()
{
	mOldVisible = ( ( bool ) * dynamic_cast< idWinBool*>( mWindow->GetWinVarByName( "visible" ) ) );
	mHidden = mOldVisible ? false : true;
	UpdateRect();
}

/*
================
rvGEWindowWrapper::Finish

After a the windwo wrapper is attached to a window and the window is parsed
the finish method is called to finish up any last details
================
*/
bool rvGEWindowWrapper::VerfiyStateKey( const char* name, const char* value, idStr* result )
{
	idStr old;
	bool failed;
	idParser src( value, strlen( value ), "", LEXFL_NOSTRINGCONCAT | LEXFL_ALLOWMULTICHARLITERALS | LEXFL_ALLOWBACKSLASHSTRINGCONCAT );

	// Save the current value
	old = mState.GetString( name );
	failed = false;

	// Try to parse in the value
	try
	{
		if( !mWindow->ParseInternalVar( name, &src ) )
		{
			// Kill the old register since the parse reg entry will add a new one
			if( !mWindow->ParseRegEntry( name, &src ) )
			{
				failed = true;
			}
		}
	}
	catch( idException& )
	{
		failed = true;
	}

	// Restore the old value
	idParser src2( old, old.Length(), "", LEXFL_NOFATALERRORS | LEXFL_NOSTRINGCONCAT | LEXFL_ALLOWMULTICHARLITERALS | LEXFL_ALLOWBACKSLASHSTRINGCONCAT );
	if( !mWindow->ParseInternalVar( name, &src2 ) )
	{
		if( !mWindow->ParseRegEntry( name, &src2 ) )
		{
		}
	}

	// Check to see if the old value matches the new value
	idStr before;
	idStr after;

	before = value;
	before.StripTrailingWhitespace();
	src.GetStringFromMarker( after );
	after.StripTrailingWhitespace();

	if( result )
	{
		*result = after;
	}

	if( failed || before.Cmp( after ) )
	{
		return false;
	}

	return true;
}
