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

#ifndef __KEYINPUT_H__
#define __KEYINPUT_H__

struct keyBindings_t
{
	idStr keyboard;
	idStr mouse;
	idStr gamepad;
};

class idSerializer;

// Converts from a USB HID code to a K_ code
int Key_CovertHIDCode( int hid );

class idKeyInput
{
public:
	static void				Init();
	static void				Shutdown();

	static void				ArgCompletion_KeyName( const idCmdArgs& args, void( *callback )( const char* s ) );
	static void				PreliminaryKeyEvent( int keyNum, bool down );
	static bool				IsDown( int keyNum );
	static int				GetUsercmdAction( int keyNum );
	static bool				GetOverstrikeMode();
	static void				SetOverstrikeMode( bool state );
	static void				ClearStates();

	static keyNum_t			StringToKeyNum( const char* str );		// This is used by the "bind" command
	static const char* 		KeyNumToString( keyNum_t keyNum );		// This is the inverse of StringToKeyNum, used for config files
	static const char* 		LocalizedKeyName( keyNum_t keyNum );	// This returns text suitable to print on screen

	static void				SetBinding( int keyNum, const char* binding );
	static const char* 		GetBinding( int keyNum );
	static bool				UnbindBinding( const char* bind );
	static int				NumBinds( const char* binding );
	static bool				ExecKeyBinding( int keyNum );
	static const char* 		KeysFromBinding( const char* bind );
	static const char* 		BindingFromKey( const char* key );
	static bool				KeyIsBoundTo( int keyNum, const char* binding );
	static void				WriteBindings( idFile* f );
	static keyBindings_t	KeyBindingsFromBinding( const char* bind, bool firstOnly = false, bool localized = false );
};

#endif /* !__KEYINPUT_H__ */
