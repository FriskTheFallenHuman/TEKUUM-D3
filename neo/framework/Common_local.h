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

#ifndef __COMMON_LOCAL_H__
#define __COMMON_LOCAL_H__

typedef enum
{
	ERP_NONE,
	ERP_FATAL,						// exit the entire game with a popup window
	ERP_DROP,						// print to console and disconnect from game
	ERP_DISCONNECT					// don't kill server
} errorParm_t;

#define	MAX_PRINT_MSG_SIZE	4096
#define MAX_WARNING_LIST	256

class idCommonLocal : public idCommon
{
public:
	idCommonLocal();

	virtual void				Init( int argc, char** argv );
	virtual void				Shutdown();
	virtual void				Quit();
	virtual bool				IsInitialized() const;
	virtual void				Frame();
	virtual void				GUIFrame( bool execCmd, bool network );
	virtual void				StartupVariable( const char* match, bool once );
	virtual void				InitTool( const toolFlag_t tool, const idDict* dict );
	virtual void				ActivateTool( bool active );
	virtual void				WriteConfigToFile( const char* filename );
	virtual void				WriteFlaggedCVarsToFile( const char* filename, int flags, const char* setCmd );
	virtual void				DebuggerCheckBreakpoint( idInterpreter* interpreter, idProgram* program, int instructionPointer );
	virtual void				BeginRedirect( char* buffer, int buffersize, void ( *flush )( const char* ) );
	virtual void				EndRedirect();
	virtual void				SetRefreshOnPrint( bool set );
	virtual void                Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				VPrintf( const char* fmt, va_list arg );
	virtual void                DPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void                Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void                DWarning( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void				PrintWarnings();
	virtual void				ClearWarnings( const char* reason );
	virtual void                Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual void                FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 );
	virtual const idLangDict* 	GetLanguageDict();

	virtual const char* 		KeysFromBinding( const char* bind );
	virtual const char* 		BindingFromKey( const char* key );

	virtual int					ButtonState( int key );
	virtual int					KeyState( int key );

	void						InitGame();
	void						ShutdownGame( bool reloading );

	// localization
	void						InitLanguageDict();
	void						LocalizeGui( const char* fileName, idLangDict& langDict );
	void						LocalizeMapData( const char* fileName, idLangDict& langDict );
	void						LocalizeSpecificMapData( const char* fileName, idLangDict& langDict, const idLangDict& replaceArgs );

	bool						IsShuttingDown()
	{
		return com_shuttingDown;
	}

	uint64_t		GetRendererBackEndMicroseconds() const
	{
		return time_backend;
	}

	uint64_t		GetRendererShadowsMicroseconds() const
	{
		return time_shadows;
	}

	uint64_t		GetRendererGPUMicroseconds() const
	{
		return time_gpu;
	}

private:
	void						InitCommands();
	void						InitRenderSystem();
	void						InitSIMD();
	bool						AddStartupCommands();
	void						ParseCommandLine( int argc, char** argv );
	void						ClearCommandLine();
	bool						SafeMode();
	void						CheckToolMode();
	void						CloseLogFile();
	void						WriteConfiguration();
	void						DumpWarnings();
	void						LoadGameDLL();
	void						UnloadGameDLL();
	void						PrintLoadingMessage( const char* msg );
	void						FilterLangList( idStrList* list, idStr lang );

	bool						com_fullyInitialized;
	bool						com_refreshOnPrint;		// update the screen every print for dmap
	int							com_errorEntered;		// 0, ERP_DROP, etc
	bool						com_shuttingDown;

	idFile* 					logFile;

	char						errorMessage[MAX_PRINT_MSG_SIZE];

	char* 						rd_buffer;
	int							rd_buffersize;
	void	( *rd_flush )( const char* buffer );

	idStr						warningCaption;
	idStrList					warningList;
	idStrList					errorList;

	uintptr_t					gameDLL;

	idLangDict					languageDict;

#ifdef ID_WRITE_VERSION
	idCompressor* 				config_compressor;
#endif
};

extern idCommonLocal commonLocal;

#endif /* !__COMMON_LOCAL_H__ */
