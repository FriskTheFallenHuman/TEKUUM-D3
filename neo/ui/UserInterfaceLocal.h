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

#ifndef __USERINTERFACE_LOCAL_H__
#define __USERINTERFACE_LOCAL_H__

class idWindow;

class idUserInterfaceLocal : public idUserInterface
{
	friend class idUserInterfaceManagerLocal;
public:
	idUserInterfaceLocal();
	virtual						~idUserInterfaceLocal();

	virtual const char* 		Name() const;
	virtual const char* 		Comment() const;
	virtual bool				IsInteractive() const;
	virtual bool				InitFromFile( const char* qpath, bool rebuild = true, bool cache = true );
	virtual const char* 		HandleEvent( const sysEvent_t* event, int time, bool* updateVisuals );
	virtual void				HandleNamedEvent( const char* namedEvent );
	virtual void				Redraw( int time, bool hud );
	virtual void				DrawCursor();
	virtual const idDict& 		State() const;
	virtual void				DeleteStateVar( const char* varName );
	virtual void				SetStateString( const char* varName, const char* value );
	virtual void				SetStateBool( const char* varName, const bool value );
	virtual void				SetStateInt( const char* varName, const int value );
	virtual void				SetStateFloat( const char* varName, const float value );

	// Gets a gui state variable
	virtual const char*			GetStateString( const char* varName, const char* defaultString = "" ) const;
	virtual bool				GetStateBool( const char* varName, const char* defaultString = "0" ) const;
	virtual int					GetStateInt( const char* varName, const char* defaultString = "0" ) const;
	virtual float				GetStateFloat( const char* varName, const char* defaultString = "0" ) const;

	virtual void				StateChanged( int time, bool redraw );
	virtual const char* 		Activate( bool activate, int time );
	virtual void				Trigger( int time );
	virtual void				ReadFromDemoFile( class idDemoFile* f );
	virtual void				WriteToDemoFile( class idDemoFile* f );
	virtual bool				WriteToSaveGame( idFile* savefile ) const;
	virtual bool				ReadFromSaveGame( idFile* savefile );
	virtual void				SetKeyBindingNames();
	virtual bool				IsUniqued() const
	{
		return uniqued;
	};
	virtual void				SetUniqued( bool b )
	{
		uniqued = b;
	};
	virtual void				SetCursor( float x, float y );

	virtual float				CursorX()
	{
		return cursorX;
	}
	virtual float				CursorY()
	{
		return cursorY;
	}

	size_t						Size();

	idDict* 					GetStateDict()
	{
		return &state;
	}

	const char* 				GetSourceFile() const
	{
		return source;
	}
	ID_TIME_T						GetTimeStamp() const
	{
		return timeStamp;
	}

	idWindow* 					GetDesktop() const
	{
		return desktop;
	}
	void						SetBindHandler( idWindow* win )
	{
		bindHandler = win;
	}
	bool						Active() const
	{
		return active;
	}
	int							GetTime() const
	{
		return time;
	}
	void						SetTime( int _time )
	{
		time = _time;
	}

	void						ClearRefs()
	{
		refs = 0;
	}
	void						AddRef()
	{
		refs++;
	}
	int							GetRefs()
	{
		return refs;
	}

	void						RecurseSetKeyBindingNames( idWindow* window );
	idStr&						GetPendingCmd()
	{
		return pendingCmd;
	};
	idStr&						GetReturnCmd()
	{
		return returnCmd;
	};

	virtual idRectangle			GetScreenRect( void )
	{
		return desktop->drawRect;
	}

private:
	bool						active;
	bool						loading;
	bool						interactive;
	bool						uniqued;

	idDict						state;
	idWindow* 					desktop;
	idWindow* 					bindHandler;

	idStr						source;
	idStr						activateStr;
	idStr						pendingCmd;
	idStr						returnCmd;
	ID_TIME_T						timeStamp;

	float						cursorX;
	float						cursorY;

	int							time;

	int							refs;
};

class idUserInterfaceManagerLocal : public idUserInterfaceManager
{
	friend class idUserInterfaceLocal;

public:
	virtual void				Init();
	virtual void				Shutdown();
	virtual void				SetDrawingDC();
	virtual void				Touch( const char* name );
	virtual void				WritePrecacheCommands( idFile* f );
	virtual void				SetSize( float width, float height );
	virtual void				BeginLevelLoad();
	virtual void				EndLevelLoad( const char* mapName );
//	virtual void				Preload( const char* mapName );
	virtual void				Reload( bool all );
	virtual void				ListGuis() const;
	virtual bool				CheckGui( const char* qpath ) const;
	virtual idUserInterface* 	Alloc() const;
	virtual void				DeAlloc( idUserInterface* gui );
	virtual idUserInterface* 	FindGui( const char* qpath, bool autoLoad = false, bool needInteractive = false, bool forceUnique = false );
	virtual idUserInterface* 	FindDemoGui( const char* qpath );
	virtual	idListGUI* 			AllocListGUI() const;
	virtual void				FreeListGUI( idListGUI* listgui );
	idTokenParser& 				GetBinaryParser()
	{
		return mapParser;
	}
private:
	idRectangle					screenRect;
	idDeviceContext				dcOld;
	idDeviceContextOptimized	dcOptimized;

	idList<idUserInterfaceLocal*> guis;
	idList<idUserInterfaceLocal*> demoGuis;

	idTokenParser				mapParser;
};

// These used to be in every window, but they all pointed at the same one in idUserInterfaceManagerLocal.
// Made a global so it can be switched out dynamically to test optimized versions.
extern idDeviceContext* dc;

#endif /* !__USERINTERFACE_LOCAL_H__ */