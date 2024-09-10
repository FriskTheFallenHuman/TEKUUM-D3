/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "Session_local.h"

#define CDKEY_FILEPATH "../" BASE_GAMEDIR "/" CDKEY_FILE
#define XPKEY_FILEPATH "../" BASE_GAMEDIR "/" XPKEY_FILE

idCVar	idSessionLocal::com_showAngles( "com_showAngles", "0", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar	idSessionLocal::com_minTics( "com_minTics", "1", CVAR_SYSTEM, "" );
idCVar	idSessionLocal::com_showTics( "com_showTics", "0", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar	idSessionLocal::com_showDemo( "com_showDemo", "0", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar	idSessionLocal::com_wipeSeconds( "com_wipeSeconds", "1", CVAR_SYSTEM, "" );
idCVar	idSessionLocal::com_guid( "com_guid", "", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_ROM, "" );

idCVar	idSessionLocal::com_numQuicksaves( "com_numQuicksaves", "4", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_INTEGER,
		"number of quicksaves to keep before overwriting the oldest", 1, 99 );

idSessionLocal		sessLocal;
idSession*			session = &sessLocal;

// these must be kept up to date with window Levelshot in guis/mainmenu.gui
const int PREVIEW_X = 211;
const int PREVIEW_Y = 31;
const int PREVIEW_WIDTH = 398;
const int PREVIEW_HEIGHT = 298;

void RandomizeStack()
{
	// attempt to force uninitialized stack memory bugs
	int		bytes = 4000000;
	byte*	buf = ( byte* )_alloca( bytes );

	int	fill = rand() & 255;
	for( int i = 0 ; i < bytes ; i++ )
	{
		buf[i] = fill;
	}
}

/*
=================
Session_RescanSI_f
=================
*/
void Session_RescanSI_f( const idCmdArgs& args )
{
	sessLocal.mapSpawnData.serverInfo = *cvarSystem->MoveCVarsToDict( CVAR_SERVERINFO );
	if( game && idAsyncNetwork::server.IsActive() )
	{
		game->SetServerInfo( sessLocal.mapSpawnData.serverInfo );
	}
}

/*
==================
Sess_WritePrecache_f
==================
*/
static void Sess_WritePrecache_f( const idCmdArgs& args )
{
	if( args.Argc() != 2 )
	{
		common->Printf( "USAGE: writePrecache <execFile>\n" );
		return;
	}
	idStr	str = args.Argv( 1 );
	str.DefaultFileExtension( ".cfg" );
	idFile* f = fileSystem->OpenFileWrite( str, "fs_configpath" );
	declManager->WritePrecacheCommands( f );
	renderModelManager->WritePrecacheCommands( f );
	uiManager->WritePrecacheCommands( f );

	fileSystem->CloseFile( f );
}

/*
===============
idSessionLocal::MaybeWaitOnCDKey
===============
*/
bool idSessionLocal::MaybeWaitOnCDKey()
{
	if( authEmitTimeout > 0 )
	{
		authWaitBox = true;
		sessLocal.MessageBox( MSG_WAIT, common->GetLanguageDict()->GetString( "#str_07191" ), NULL, true, NULL, NULL, true );
		return true;
	}
	return false;
}

/*
===================
Session_PromptKey_f
===================
*/
static void Session_PromptKey_f( const idCmdArgs& args )
{
	const char*	retkey;
	bool		valid[ 2 ];
	static bool recursed = false;

	if( recursed )
	{
		common->Warning( "promptKey recursed - aborted" );
		return;
	}
	recursed = true;

	do
	{
		// in case we're already waiting for an auth to come back to us ( may happen exceptionally )
		if( sessLocal.MaybeWaitOnCDKey() )
		{
			if( sessLocal.CDKeysAreValid( true ) )
			{
				recursed = false;
				return;
			}
		}
		// the auth server may have replied and set an error message, otherwise use a default
		const char* prompt_msg = sessLocal.GetAuthMsg();
		if( prompt_msg[ 0 ] == '\0' )
		{
			prompt_msg = common->GetLanguageDict()->GetString( "#str_04308" );
		}
		retkey = sessLocal.MessageBox( MSG_CDKEY, prompt_msg, common->GetLanguageDict()->GetString( "#str_04305" ), true, NULL, NULL, true );
		if( retkey )
		{
			if( sessLocal.CheckKey( retkey, false, valid ) )
			{
				// if all went right, then we may have sent an auth request to the master ( unless the prompt is used during a net connect )
				bool canExit = true;
				if( sessLocal.MaybeWaitOnCDKey() )
				{
					// wait on auth reply, and got denied, prompt again
					if( !sessLocal.CDKeysAreValid( true ) )
					{
						// server says key is invalid - MaybeWaitOnCDKey was interrupted by a CDKeysAuthReply call, which has set the right error message
						// the invalid keys have also been cleared in the process
						sessLocal.MessageBox( MSG_OK, sessLocal.GetAuthMsg(), common->GetLanguageDict()->GetString( "#str_04310" ), true, NULL, NULL, true );
						canExit = false;
					}
				}
				if( canExit )
				{
					// make sure that's saved on file
					sessLocal.WriteCDKey();
					sessLocal.MessageBox( MSG_OK, common->GetLanguageDict()->GetString( "#str_04307" ), common->GetLanguageDict()->GetString( "#str_04305" ), true, NULL, NULL, true );
					break;
				}
			}
			else
			{
				// offline check sees key invalid
				// build a message about keys being wrong. do not attempt to change the current key state though
				// ( the keys may be valid, but user would have clicked on the dialog anyway, that kind of thing )
				idStr msg;
				idAsyncNetwork::BuildInvalidKeyMsg( msg, valid );
				sessLocal.MessageBox( MSG_OK, msg, common->GetLanguageDict()->GetString( "#str_04310" ), true, NULL, NULL, true );
			}
		}
		else if( args.Argc() == 2 && idStr::Icmp( args.Argv( 1 ), "force" ) == 0 )
		{
			// cancelled in force mode
			cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
			cmdSystem->ExecuteCommandBuffer();
		}
	}
	while( retkey );
	recursed = false;
}

/*
===============================================================================

SESSION LOCAL

===============================================================================
*/

/*
===============
idSessionLocal::Clear
===============
*/
void idSessionLocal::Clear()
{

	insideUpdateScreen = false;
	insideExecuteMapChange = false;

	loadingSaveGame = false;
	savegameFile = NULL;
	savegameVersion = 0;

	currentMapName.Clear();
	aviDemoShortName.Clear();
	msgFireBack[ 0 ].Clear();
	msgFireBack[ 1 ].Clear();

	timeHitch = 0;

	rw = NULL;
	sw = NULL;
	menuSoundWorld = NULL;
	readDemo = NULL;
	writeDemo = NULL;
	renderdemoVersion = 0;
	cmdDemoFile = NULL;

	// RB begin
	gameFrame = 0;
	gameTime = 0;
	gameTimeResidual = 0;
	// RB end

	syncNextGameFrame = false;
	mapSpawned = false;
	guiActive = NULL;
	aviCaptureMode = false;
	timeDemo = TD_NO;
	waitingOnBind = false;
	lastPacifierTime = 0;

	msgRunning = false;
	guiMsgRestore = NULL;
	msgIgnoreButtons = false;

	bytesNeededForMapLoad = 0;

#if ID_CONSOLE_LOCK
	emptyDrawCount = 0;
#endif
	ClearWipe();

	loadGameList.Clear();
	modsList.Clear();

	authEmitTimeout = 0;
	authWaitBox = false;

	authMsg.Clear();
}

/*
===============
idSessionLocal::idSessionLocal
===============
*/
idSessionLocal::idSessionLocal()
{
	guiInGame = guiMainMenu = guiIntro \
							  = guiRestartMenu = guiLoading = guiGameOver = guiActive \
									  = guiTest = guiMsg = guiMsgRestore = guiTakeNotes = NULL;

	menuSoundWorld = NULL;

	Clear();
}

/*
===============
idSessionLocal::~idSessionLocal
===============
*/
idSessionLocal::~idSessionLocal()
{
}

/*
===============
idSessionLocal::Stop

called on errors and game exits
===============
*/
void idSessionLocal::Stop()
{
	ClearWipe();

	// clear mapSpawned and demo playing flags
	UnloadMap();

	// disconnect async client
	idAsyncNetwork::client.DisconnectFromServer();

	// kill async server
	idAsyncNetwork::server.Kill();

	if( sw )
	{
		sw->StopAllSounds();
	}

	insideUpdateScreen = false;
	insideExecuteMapChange = false;

	// drop all guis
	SetGUI( NULL, NULL );
}

/*
===============
idSessionLocal::Shutdown
===============
*/
void idSessionLocal::Shutdown()
{
	int i;

	if( aviCaptureMode )
	{
		EndAVICapture();
	}

	if( timeDemo == TD_YES )
	{
		// else the game freezes when showing the timedemo results
		timeDemo = TD_YES_THEN_QUIT;
	}

	Stop();

	if( rw )
	{
		delete rw;
		rw = NULL;
	}

	if( sw )
	{
		delete sw;
		sw = NULL;
	}

	if( menuSoundWorld )
	{
		delete menuSoundWorld;
		menuSoundWorld = NULL;
	}

	mapSpawnData.serverInfo.Clear();
	mapSpawnData.syncedCVars.Clear();
	for( i = 0; i < MAX_ASYNC_CLIENTS; i++ )
	{
		mapSpawnData.userInfo[i].Clear();
		mapSpawnData.persistentPlayerInfo[i].Clear();
	}

	if( guiMainMenu_MapList != NULL )
	{
		guiMainMenu_MapList->Shutdown();
		uiManager->FreeListGUI( guiMainMenu_MapList );
		guiMainMenu_MapList = NULL;
	}

	Clear();
}

/*
===============
idSessionLocal::IsMultiplayer
===============
*/
bool	idSessionLocal::IsMultiplayer()
{
	return idAsyncNetwork::IsActive();
}

/*
================
idSessionLocal::ShowLoadingGui
================
*/
void idSessionLocal::ShowLoadingGui()
{
	if( com_ticNumber == 0 )
	{
		return;
	}
	console->Close();

	// introduced in D3XP code. don't think it actually fixes anything, but doesn't hurt either
#if 1
	// Try and prevent the while loop from being skipped over (long hitch on the main thread?)
	int stop = Sys_Milliseconds() + 1000;
	int force = 10;
	while( Sys_Milliseconds() < stop || force-- > 0 )
	{
		com_frameTime = com_ticNumber * USERCMD_MSEC;
		session->Frame();

		// RB: added captureToImage
		const bool captureToImage = false;
		session->UpdateScreen( captureToImage, false );
		// RB end
	}
#else
	int stop = com_ticNumber + 1000.0f / USERCMD_MSEC * 1.0f;
	while( com_ticNumber < stop )
	{
		com_frameTime = com_ticNumber * USERCMD_MSEC;
		session->Frame();
		session->UpdateScreen( false );
	}
#endif
}

/*
================
Session_TestGUI_f
================
*/
static void Session_TestGUI_f( const idCmdArgs& args )
{
	sessLocal.TestGUI( args.Argv( 1 ) );
}

/*
================
idSessionLocal::TestGUI
================
*/
void idSessionLocal::TestGUI( const char* guiName )
{
	if( guiName && *guiName )
	{
		guiTest = uiManager->FindGui( guiName, true, false, true );
	}
	else
	{
		guiTest = NULL;
	}
}

/*
================
Session_Disconnect_f
================
*/
static void Session_Disconnect_f( const idCmdArgs& args )
{
	sessLocal.Stop();
	sessLocal.StartMenu();
	if( soundSystem )
	{
		soundSystem->SetMute( false );
	}
}

/*
==============
SaveCmdDemoFromFile
==============
*/
void idSessionLocal::SaveCmdDemoToFile( idFile* file )
{

	mapSpawnData.serverInfo.WriteToFileHandle( file );

	for( int i = 0 ; i < MAX_ASYNC_CLIENTS ; i++ )
	{
		mapSpawnData.userInfo[i].WriteToFileHandle( file );
		mapSpawnData.persistentPlayerInfo[i].WriteToFileHandle( file );
	}

	file->Write( &mapSpawnData.mapSpawnUsercmd, sizeof( mapSpawnData.mapSpawnUsercmd ) );

	if( numClients < 1 )
	{
		numClients = 1;
	}
	file->Write( loggedUsercmds, numClients * logIndex * sizeof( loggedUsercmds[0] ) );
}

/*
==============
idSessionLocal::LoadCmdDemoFromFile
==============
*/
void idSessionLocal::LoadCmdDemoFromFile( idFile* file )
{

	mapSpawnData.serverInfo.ReadFromFileHandle( file );

	for( int i = 0 ; i < MAX_ASYNC_CLIENTS ; i++ )
	{
		mapSpawnData.userInfo[i].ReadFromFileHandle( file );
		mapSpawnData.persistentPlayerInfo[i].ReadFromFileHandle( file );
	}
	file->Read( &mapSpawnData.mapSpawnUsercmd, sizeof( mapSpawnData.mapSpawnUsercmd ) );
}

/*
==============
idSessionLocal::WriteCmdDemo

Dumps the accumulated commands for the current level.
This should still work after disconnecting from a level
==============
*/
void idSessionLocal::WriteCmdDemo( const char* demoName, bool save )
{

	if( !demoName[0] )
	{
		common->Printf( "idSessionLocal::WriteCmdDemo: no name specified\n" );
		return;
	}

	idStr statsName;
	if( save )
	{
		statsName = demoName;
		statsName.StripFileExtension();
		statsName.DefaultFileExtension( ".stats" );
	}

	common->Printf( "writing save data to %s\n", demoName );

	idFile* cmdDemoFile = fileSystem->OpenFileWrite( demoName );
	if( !cmdDemoFile )
	{
		common->Printf( "Couldn't open for writing %s\n", demoName );
		return;
	}

	if( save )
	{
		cmdDemoFile->Write( &logIndex, sizeof( logIndex ) );
	}

	SaveCmdDemoToFile( cmdDemoFile );

	if( save )
	{
		idFile* statsFile = fileSystem->OpenFileWrite( statsName );
		if( statsFile )
		{
			statsFile->Write( &statIndex, sizeof( statIndex ) );
			statsFile->Write( loggedStats, numClients * statIndex * sizeof( loggedStats[0] ) );
			fileSystem->CloseFile( statsFile );
		}
	}

	fileSystem->CloseFile( cmdDemoFile );
}

/*
===============
idSessionLocal::FinishCmdLoad
===============
*/
void idSessionLocal::FinishCmdLoad()
{
}

/*
===============
idSessionLocal::StartPlayingCmdDemo
===============
*/
void idSessionLocal::StartPlayingCmdDemo( const char* demoName )
{
	// exit any current game
	Stop();

	idStr fullDemoName = "demos/";
	fullDemoName += demoName;
	fullDemoName.DefaultFileExtension( ".cdemo" );
	cmdDemoFile = fileSystem->OpenFileRead( fullDemoName );

	if( cmdDemoFile == NULL )
	{
		common->Printf( "Couldn't open %s\n", fullDemoName.c_str() );
		return;
	}

	guiLoading = uiManager->FindGui( "guis/map/loading.gui", true, false, true );
	//cmdDemoFile->Read(&loadGameTime, sizeof(loadGameTime));

	LoadCmdDemoFromFile( cmdDemoFile );

	// start the map
	ExecuteMapChange();

	cmdDemoFile = fileSystem->OpenFileRead( fullDemoName );

	// have to do this twice as the execmapchange clears the cmddemofile
	LoadCmdDemoFromFile( cmdDemoFile );

	// run one frame to get the view angles correct
	RunGameTic();
}

/*
===============
idSessionLocal::TimeCmdDemo
===============
*/
void idSessionLocal::TimeCmdDemo( const char* demoName )
{
	StartPlayingCmdDemo( demoName );
	ClearWipe();

	// RB: added captureToImage
	const bool captureToImage = true;
	UpdateScreen( captureToImage );

	int		startTime = Sys_Milliseconds();
	int		count = 0;
	int		minuteStart, minuteEnd;
	float	sec;

	// run all the frames in sequence
	minuteStart = startTime;

	while( cmdDemoFile )
	{
		RunGameTic();
		count++;

		if( count / 3600 != ( count - 1 ) / 3600 )
		{
			minuteEnd = Sys_Milliseconds();
			sec = ( minuteEnd - minuteStart ) / 1000.0;
			minuteStart = minuteEnd;
			common->Printf( "minute %i took %3.1f seconds\n", count / 3600, sec );
			UpdateScreen( captureToImage );
		}
	}

	int		endTime = Sys_Milliseconds();
	sec = ( endTime - startTime ) / 1000.0;
	common->Printf( "%i seconds of game, replayed in %5.1f seconds\n", count / 60, sec );
}

/*
===============
TakeViewNotes_f
===============
*/
void TakeViewNotes_f( const idCmdArgs& args )
{
	const char* p = ( args.Argc() > 1 ) ? args.Argv( 1 ) : "";
	sessLocal.TakeNotes( p );
}

/*
===============
TakeViewNotes2_f
===============
*/
void TakeViewNotes2_f( const idCmdArgs& args )
{
	const char* p = ( args.Argc() > 1 ) ? args.Argv( 1 ) : "";
	sessLocal.TakeNotes( p, true );
}

/*
===============
idSessionLocal::TakeNotes
===============
*/
void idSessionLocal::TakeNotes( const char* p, bool extended )
{
	if( !mapSpawned )
	{
		common->Printf( "No map loaded!\n" );
		return;
	}

	if( extended )
	{
		guiTakeNotes = uiManager->FindGui( "guis/takeNotes2.gui", true, false, true );

#if 0
		const char* people[] =
		{
			"Nobody", "Adam", "Brandon", "David", "PHook", "Jay", "Jake",
			"PatJ", "Brett", "Ted", "Darin", "Brian", "Sean"
		};
#else
		const char* people[] =
		{
			"Tim", "Kenneth", "Robert",
			"Matt", "Mal", "Jerry", "Steve", "Pat",
			"Xian", "Ed", "Fred", "James", "Eric", "Andy", "Seneca", "Patrick", "Kevin",
			"MrElusive", "Jim", "Brian", "John", "Adrian", "Nobody"
		};
#endif
		const int numPeople = sizeof( people ) / sizeof( people[0] );

		idListGUI* guiList_people = uiManager->AllocListGUI();
		guiList_people->Config( guiTakeNotes, "person" );
		for( int i = 0; i < numPeople; i++ )
		{
			guiList_people->Push( people[i] );
		}
		uiManager->FreeListGUI( guiList_people );

	}
	else
	{
		guiTakeNotes = uiManager->FindGui( "guis/takeNotes.gui", true, false, true );
	}

	SetGUI( guiTakeNotes, NULL );
	guiActive->SetStateString( "note", "" );
	guiActive->SetStateString( "notefile", p );
	guiActive->SetStateBool( "extended", extended );
	guiActive->Activate( true, com_frameTime );
}

/*
===============
Session_Hitch_f
===============
*/
void Session_Hitch_f( const idCmdArgs& args )
{
	idSoundWorld* sw = soundSystem->GetPlayingSoundWorld();
	if( sw )
	{
		soundSystem->SetMute( true );
		sw->Pause();
		Sys_EnterCriticalSection();
	}
	if( args.Argc() == 2 )
	{
		Sys_Sleep( atoi( args.Argv( 1 ) ) );
	}
	else
	{
		Sys_Sleep( 100 );
	}
	if( sw )
	{
		Sys_LeaveCriticalSection();
		sw->UnPause();
		soundSystem->SetMute( false );
	}
}

/*
===============
idSessionLocal::ProcessEvent
===============
*/
bool idSessionLocal::ProcessEvent( const sysEvent_t* event )
{
	// hitting escape anywhere brings up the menu
	// DG: but shift-escape should bring up console instead so ignore that
	if( !guiActive && event->evType == SE_KEY && event->evValue2 == 1
			&& event->evValue == K_ESCAPE && ( !idKeyInput::IsDown( K_LSHIFT ) || !idKeyInput::IsDown( K_RSHIFT ) ) )
	{
		console->Close();
		if( game )
		{
			idUserInterface*	gui = NULL;
			escReply_t		op;
			op = game->HandleESC( &gui );
			if( op == ESC_IGNORE )
			{
				return true;
			}
			else if( op == ESC_GUI )
			{
				SetGUI( gui, NULL );
				return true;
			}
		}
		StartMenu();
		return true;
	}

	// let the pull-down console take it if desired
	if( console->ProcessEvent( event, false ) )
	{
		return true;
	}

	// if we are testing a GUI, send all events to it
	if( guiTest )
	{
		// hitting escape exits the testgui
		if( event->evType == SE_KEY && event->evValue2 == 1 && event->evValue == K_ESCAPE )
		{
			guiTest = NULL;
			return true;
		}

		static const char* cmd;
		cmd = guiTest->HandleEvent( event, com_frameTime );
		if( cmd && cmd[0] )
		{
			common->Printf( "testGui event returned: '%s'\n", cmd );
		}
		return true;
	}

	// menus / etc
	if( guiActive )
	{
		MenuEvent( event );
		return true;
	}

	// if we aren't in a game, force the console to take it
	if( !mapSpawned )
	{
		console->ProcessEvent( event, true );
		return true;
	}

	// in game, exec bindings for all key downs
	if( event->evType == SE_KEY && event->evValue2 == 1 )
	{
		idKeyInput::ExecKeyBinding( event->evValue );
		return true;
	}

	return false;
}

/*
===============
idSessionLocal::AdvanceRenderDemo
===============
*/
void idSessionLocal::AdvanceRenderDemo( bool singleFrameOnly )
{
	if( lastDemoTic == -1 )
	{
		lastDemoTic = latchedTicNumber - 1;
	}

	int skipFrames = 0;

	if( !aviCaptureMode && !timeDemo && !singleFrameOnly )
	{
		skipFrames = ( ( latchedTicNumber - lastDemoTic ) / USERCMD_PER_DEMO_FRAME ) - 1;
		// never skip too many frames, just let it go into slightly slow motion
		if( skipFrames > 4 )
		{
			skipFrames = 4;
		}
		lastDemoTic = latchedTicNumber - latchedTicNumber % USERCMD_PER_DEMO_FRAME;
	}
	else
	{
		// always advance a single frame with avidemo and timedemo
		lastDemoTic = latchedTicNumber;
	}

	while( skipFrames > -1 )
	{
		int		ds = DS_FINISHED;

		readDemo->ReadInt( ds );
		if( ds == DS_FINISHED )
		{
			if( numDemoFrames != 1 )
			{
				// if the demo has a single frame (a demoShot), continuously replay
				// the renderView that has already been read
				Stop();
				StartMenu();
			}
			break;
		}
		if( ds == DS_RENDER )
		{
			if( rw->ProcessDemoCommand( readDemo, &currentDemoRenderView, &demoTimeOffset ) )
			{
				// a view is ready to render
				skipFrames--;
				numDemoFrames++;
			}
			continue;
		}
		if( ds == DS_SOUND )
		{
			sw->ProcessDemoCommand( readDemo );
			continue;
		}
		// appears in v1.2, with savegame format 17
		if( ds == DS_VERSION )
		{
			readDemo->ReadInt( renderdemoVersion );
			common->Printf( "reading a v%d render demo\n", renderdemoVersion );
			// set the savegameVersion to current for render demo paths that share the savegame paths
			savegameVersion = SAVEGAME_VERSION;
			continue;
		}
		common->Error( "Bad render demo token" );
	}

	if( com_showDemo.GetBool() )
	{
		common->Printf( "frame:%i DemoTic:%i latched:%i skip:%i\n", numDemoFrames, lastDemoTic, latchedTicNumber, skipFrames );
	}

}

/*
===============
idSessionLocal::DrawCmdGraph

Graphs yaw angle for testing smoothness
===============
*/
static const int	ANGLE_GRAPH_HEIGHT = 128;
static const int	ANGLE_GRAPH_STRETCH = 3;
void idSessionLocal::DrawCmdGraph()
{
	if( !com_showAngles.GetBool() )
	{
		return;
	}
	renderSystem->SetColor4( 0.1f, 0.1f, 0.1f, 1.0f );
	renderSystem->DrawStretchPic( 0, 480 - ANGLE_GRAPH_HEIGHT, MAX_BUFFERED_USERCMD * ANGLE_GRAPH_STRETCH, ANGLE_GRAPH_HEIGHT, 0, 0, 1, 1, whiteMaterial );
	renderSystem->SetColor4( 0.9f, 0.9f, 0.9f, 1.0f );
	for( int i = 0 ; i < MAX_BUFFERED_USERCMD - 4 ; i++ )
	{
		usercmd_t	cmd = usercmdGen->TicCmd( latchedTicNumber - ( MAX_BUFFERED_USERCMD - 4 ) + i );
		int h = cmd.angles[1];
		h >>= 8;
		h &= ( ANGLE_GRAPH_HEIGHT - 1 );
		renderSystem->DrawStretchPic( i * ANGLE_GRAPH_STRETCH, 480 - h, 1, h, 0, 0, 1, 1, whiteMaterial );
	}
}

/*
================
idSessionLocal::RunGameTic
================
*/
void idSessionLocal::RunGameTic()
{
	logCmd_t	logCmd;
	usercmd_t	cmd;

	// if we are doing a command demo, read or write from the file
	if( cmdDemoFile )
	{
		if( !cmdDemoFile->Read( &logCmd, sizeof( logCmd ) ) )
		{
			common->Printf( "Command demo completed at logIndex %i\n", logIndex );
			fileSystem->CloseFile( cmdDemoFile );
			cmdDemoFile = NULL;
			if( aviCaptureMode )
			{
				EndAVICapture();
				Shutdown();
			}
			// we fall out of the demo to normal commands
			// the impulse and chat character toggles may not be correct, and the view
			// angle will definitely be wrong
		}
		else
		{
			cmd = logCmd.cmd;
			cmd.ByteSwap();
			logCmd.consistencyHash = LittleInt( logCmd.consistencyHash );
		}
	}

	// if we didn't get one from the file, get it locally
	if( !cmdDemoFile )
	{
		// get a locally created command
		if( com_asyncInput.GetBool() )
		{
			cmd = usercmdGen->TicCmd( lastGameTic );
		}
		else
		{
			// RB begin
			usercmdGen->BuildCurrentUsercmd();
			cmd = usercmdGen->GetCurrentUsercmd();
			// RB end
		}
		lastGameTic++;
	}

	// run the game logic every player move
	int	start = Sys_Milliseconds();
	gameReturn_t	ret = game->RunFrame( &cmd );

	int end = Sys_Milliseconds();
	time_gameFrame += end - start;	// note time used for com_speeds

	// check for constency failure from a recorded command
	if( cmdDemoFile )
	{
		if( ret.consistencyHash != logCmd.consistencyHash )
		{
			common->Printf( "Consistency failure on logIndex %i\n", logIndex );
			Stop();
			return;
		}
	}

	// save the cmd for cmdDemo archiving
	if( logIndex < MAX_LOGGED_USERCMDS )
	{
		loggedUsercmds[logIndex].cmd = cmd;
		// save the consistencyHash for demo playback verification
		loggedUsercmds[logIndex].consistencyHash = ret.consistencyHash;
		if( logIndex % 30 == 0 && statIndex < MAX_LOGGED_STATS )
		{
			loggedStats[statIndex].health = ret.health;
			loggedStats[statIndex].heartRate = ret.heartRate;
			loggedStats[statIndex].stamina = ret.stamina;
			loggedStats[statIndex].combat = ret.combat;
			statIndex++;
		}
		logIndex++;
	}

	syncNextGameFrame = ret.syncNextGameFrame;

	if( ret.sessionCommand[0] )
	{
		idCmdArgs args;

		args.TokenizeString( ret.sessionCommand, false );

		if( !idStr::Icmp( args.Argv( 0 ), "map" ) )
		{
			// get current player states
			for( int i = 0 ; i < numClients ; i++ )
			{
				mapSpawnData.persistentPlayerInfo[i] = game->GetPersistentPlayerInfo( i );
			}
			// clear the devmap key on serverinfo, so player spawns
			// won't get the map testing items
			mapSpawnData.serverInfo.Delete( "devmap" );

			// go to the next map
			MoveToNewMap( args.Argv( 1 ) );
		}
		else if( !idStr::Icmp( args.Argv( 0 ), "devmap" ) )
		{
			mapSpawnData.serverInfo.Set( "devmap", "1" );
			MoveToNewMap( args.Argv( 1 ) );
		}
		else if( !idStr::Icmp( args.Argv( 0 ), "died" ) )
		{
			// restart on the same map
			UnloadMap();
			SetGUI( guiRestartMenu, NULL );
		}
		else if( !idStr::Icmp( args.Argv( 0 ), "disconnect" ) )
		{
			cmdSystem->BufferCommandText( CMD_EXEC_INSERT, "stoprecording ; disconnect" );
		}
	}
}

/*
===============
idSessionLocal::Init

Called in an orderly fashion at system startup,
so commands, cvars, files, etc are all available
===============
*/
void idSessionLocal::Init()
{

	common->Printf( "-------- Initializing Session --------\n" );

	cmdSystem->AddCommand( "writePrecache", Sess_WritePrecache_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "writes precache commands" );

	cmdSystem->AddCommand( "disconnect", Session_Disconnect_f, CMD_FL_SYSTEM, "disconnects from a game" );

	cmdSystem->AddCommand( "testGUI", Session_TestGUI_f, CMD_FL_SYSTEM, "tests a gui" );

	cmdSystem->AddCommand( "takeViewNotes", TakeViewNotes_f, CMD_FL_SYSTEM, "take notes about the current map from the current view" );
	cmdSystem->AddCommand( "takeViewNotes2", TakeViewNotes2_f, CMD_FL_SYSTEM, "extended take view notes" );

	cmdSystem->AddCommand( "rescanSI", Session_RescanSI_f, CMD_FL_SYSTEM, "internal - rescan serverinfo cvars and tell game" );

	cmdSystem->AddCommand( "promptKey", Session_PromptKey_f, CMD_FL_SYSTEM, "prompt and sets the CD Key" );

	cmdSystem->AddCommand( "hitch", Session_Hitch_f, CMD_FL_SYSTEM | CMD_FL_CHEAT, "hitches the game" );

	// the same idRenderWorld will be used for all games
	// and demos, insuring that level specific models
	// will be freed
	rw = renderSystem->AllocRenderWorld();
	sw = soundSystem->AllocSoundWorld( rw );

	menuSoundWorld = soundSystem->AllocSoundWorld( rw );

	// we have a single instance of the main menu
	guiMainMenu = uiManager->FindGui( "guis/mainmenu.gui", true, false, true );
	if( !guiMainMenu )
	{
		guiMainMenu = uiManager->FindGui( "guis/demo_mainmenu.gui", true, false, true );
		demoversion = ( guiMainMenu != NULL );
	}
	guiMainMenu_MapList = uiManager->AllocListGUI();
	guiMainMenu_MapList->Config( guiMainMenu, "mapList" );
	idAsyncNetwork::client.serverList.GUIConfig( guiMainMenu, "serverList" );
	guiRestartMenu = uiManager->FindGui( "guis/restart.gui", true, false, true );
	guiGameOver = uiManager->FindGui( "guis/gameover.gui", true, false, true );
	guiMsg = uiManager->FindGui( "guis/msg.gui", true, false, true );
	guiTakeNotes = uiManager->FindGui( "guis/takeNotes.gui", true, false, true );
	guiIntro = uiManager->FindGui( "guis/intro.gui", true, false, true );

	whiteMaterial = declManager->FindMaterial( "_white" );

	guiInGame = NULL;
	guiTest = NULL;

	guiActive = NULL;
	guiHandle = NULL;

	ReadCDKey();

	common->Printf( "session initialized\n" );
	common->Printf( "--------------------------------------\n" );
}

/*
===============
idSessionLocal::GetLocalClientNum
===============
*/
int idSessionLocal::GetLocalClientNum()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetLocalClientNum();
	}
	else if( idAsyncNetwork::server.IsActive() )
	{
		if( idAsyncNetwork::serverDedicated.GetInteger() == 0 )
		{
			return 0;
		}
		else if( idAsyncNetwork::server.IsClientInGame( idAsyncNetwork::serverDrawClient.GetInteger() ) )
		{
			return idAsyncNetwork::serverDrawClient.GetInteger();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}
}

/*
===============
idSessionLocal::SetPlayingSoundWorld
===============
*/
void idSessionLocal::SetPlayingSoundWorld()
{
	if( guiActive && ( guiActive == guiMainMenu || guiActive == guiIntro || guiActive == guiLoading || ( guiActive == guiMsg && !mapSpawned ) ) )
	{
		soundSystem->SetPlayingSoundWorld( menuSoundWorld );
	}
	else
	{
		soundSystem->SetPlayingSoundWorld( sw );
	}
}

/*
===============
idSessionLocal::TimeHitch

this is used by the sound system when an OnDemand sound is loaded, so the game action
doesn't advance and get things out of sync
===============
*/
void idSessionLocal::TimeHitch( int msec )
{
	timeHitch += msec;
}

/*
=================
idSessionLocal::ReadCDKey
=================
*/
void idSessionLocal::ReadCDKey()
{
	idStr filename;
	idFile* f;
	char buffer[32];

	cdkey_state = CDKEY_UNKNOWN;

	filename = CDKEY_FILEPATH;
	f = fileSystem->OpenExplicitFileRead( fileSystem->RelativePathToOSPath( filename, "fs_configpath" ) );

	// try the install path, which is where the cd installer and steam put it
	if( !f )
	{
		f = fileSystem->OpenExplicitFileRead( fileSystem->RelativePathToOSPath( filename, "fs_basepath" ) );
	}

	if( !f )
	{
		common->Printf( "Couldn't read %s.\n", filename.c_str() );
		cdkey[ 0 ] = '\0';
	}
	else
	{
		memset( buffer, 0, sizeof( buffer ) );
		f->Read( buffer, CDKEY_BUF_LEN - 1 );
		fileSystem->CloseFile( f );
		idStr::Copynz( cdkey, buffer, CDKEY_BUF_LEN );
	}

	xpkey_state = CDKEY_UNKNOWN;

	filename = XPKEY_FILEPATH;
	f = fileSystem->OpenExplicitFileRead( fileSystem->RelativePathToOSPath( filename, "fs_configpath" ) );

	// try the install path, which is where the cd installer and steam put it
	if( !f )
	{
		f = fileSystem->OpenExplicitFileRead( fileSystem->RelativePathToOSPath( filename, "fs_basepath" ) );
	}

	if( !f )
	{
		common->Printf( "Couldn't read %s.\n", filename.c_str() );
		xpkey[ 0 ] = '\0';
	}
	else
	{
		memset( buffer, 0, sizeof( buffer ) );
		f->Read( buffer, CDKEY_BUF_LEN - 1 );
		fileSystem->CloseFile( f );
		idStr::Copynz( xpkey, buffer, CDKEY_BUF_LEN );
	}
}

/*
================
idSessionLocal::WriteCDKey
================
*/
void idSessionLocal::WriteCDKey()
{
	idStr filename;
	idFile* f;
	const char* OSPath;

	filename = CDKEY_FILEPATH;
	// OpenFileWrite advertises creating directories to the path if needed, but that won't work with a '..' in the path
	// occasionally on windows, but mostly on Linux and OSX, the fs_configpath/base may not exist in full
	OSPath = fileSystem->BuildOSPath( cvarSystem->GetCVarString( "fs_configpath" ), BASE_GAMEDIR, CDKEY_FILE );
	fileSystem->CreateOSPath( OSPath );
	f = fileSystem->OpenFileWrite( filename, "fs_configpath" );
	if( !f )
	{
		common->Printf( "Couldn't write %s.\n", filename.c_str() );
		return;
	}
	f->Printf( "%s%s", cdkey, CDKEY_TEXT );
	fileSystem->CloseFile( f );

	filename = XPKEY_FILEPATH;
	f = fileSystem->OpenFileWrite( filename, "fs_configpath" );
	if( !f )
	{
		common->Printf( "Couldn't write %s.\n", filename.c_str() );
		return;
	}
	f->Printf( "%s%s", xpkey, CDKEY_TEXT );
	fileSystem->CloseFile( f );
}

/*
===============
idSessionLocal::ClearKey
===============
*/
void idSessionLocal::ClearCDKey( bool valid[ 2 ] )
{
	if( !valid[ 0 ] )
	{
		memset( cdkey, 0, CDKEY_BUF_LEN );
		cdkey_state = CDKEY_UNKNOWN;
	}
	else if( cdkey_state == CDKEY_CHECKING )
	{
		// if a key was in checking and not explicitely asked for clearing, put it back to ok
		cdkey_state = CDKEY_OK;
	}
	if( !valid[ 1 ] )
	{
		memset( xpkey, 0, CDKEY_BUF_LEN );
		xpkey_state = CDKEY_UNKNOWN;
	}
	else if( xpkey_state == CDKEY_CHECKING )
	{
		xpkey_state = CDKEY_OK;
	}
	WriteCDKey( );
}

/*
================
idSessionLocal::GetCDKey
================
*/
const char* idSessionLocal::GetCDKey( bool xp )
{
	if( !xp )
	{
		return cdkey;
	}
	if( xpkey_state == CDKEY_OK || xpkey_state == CDKEY_CHECKING )
	{
		return xpkey;
	}
	return NULL;
}

// digits to letters table
#define CDKEY_DIGITS "TWSBJCGD7PA23RLH"

/*
===============
idSessionLocal::EmitGameAuth
we toggled some key state to CDKEY_CHECKING. send a standalone auth packet to validate
===============
*/
void idSessionLocal::EmitGameAuth()
{
	// make sure the auth reply is empty, we use it to indicate an auth reply
	authMsg.Empty();
	if( idAsyncNetwork::client.SendAuthCheck( cdkey_state == CDKEY_CHECKING ? cdkey : NULL, xpkey_state == CDKEY_CHECKING ? xpkey : NULL ) )
	{
		authEmitTimeout = Sys_Milliseconds() + CDKEY_AUTH_TIMEOUT;
		common->DPrintf( "authing with the master..\n" );
	}
	else
	{
		// net is not available
		common->DPrintf( "sendAuthCheck failed\n" );
		if( cdkey_state == CDKEY_CHECKING )
		{
			cdkey_state = CDKEY_OK;
		}
		if( xpkey_state == CDKEY_CHECKING )
		{
			xpkey_state = CDKEY_OK;
		}
	}
}

/*
================
idSessionLocal::CheckKey
the function will only modify keys to _OK or _CHECKING if the offline checks are passed
if the function returns false, the offline checks failed, and offline_valid holds which keys are bad
================
*/
bool idSessionLocal::CheckKey( const char* key, bool netConnect, bool offline_valid[ 2 ] )
{
	char lkey[ 2 ][ CDKEY_BUF_LEN ];
	char l_chk[ 2 ][ 3 ];
	char s_chk[ 3 ];
	int imax, i_key;
	unsigned int checksum, chk8;
	bool edited_key[ 2 ];

	// make sure have a right input string
	assert( strlen( key ) == ( CDKEY_BUF_LEN - 1 ) * 2 + 4 + 3 + 4 );

	edited_key[ 0 ] = ( key[0] == '1' );
	idStr::Copynz( lkey[0], key + 2, CDKEY_BUF_LEN );
	idStr::ToUpper( lkey[0] );
	idStr::Copynz( l_chk[0], key + CDKEY_BUF_LEN + 2, 3 );
	idStr::ToUpper( l_chk[0] );
	edited_key[ 1 ] = ( key[ CDKEY_BUF_LEN + 2 + 3 ] == '1' );
	idStr::Copynz( lkey[1], key + CDKEY_BUF_LEN + 7, CDKEY_BUF_LEN );
	idStr::ToUpper( lkey[1] );
	idStr::Copynz( l_chk[1], key + CDKEY_BUF_LEN * 2 + 7, 3 );
	idStr::ToUpper( l_chk[1] );

	if( fileSystem->HasD3XP() )
	{
		imax = 2;
	}
	else
	{
		imax = 1;
	}
	offline_valid[ 0 ] = offline_valid[ 1 ] = true;
	for( i_key = 0; i_key < imax; i_key++ )
	{
		// check that the characters are from the valid set
		int i;
		for( i = 0; i < CDKEY_BUF_LEN - 1; i++ )
		{
			if( !strchr( CDKEY_DIGITS, lkey[i_key][i] ) )
			{
				offline_valid[ i_key ] = false;
				continue;
			}
		}

		if( edited_key[ i_key ] )
		{
			// verify the checksum for edited keys only
			checksum = CRC32_BlockChecksum( lkey[i_key], CDKEY_BUF_LEN - 1 );
			chk8 = ( checksum & 0xff ) ^ ( ( ( checksum & 0xff00 ) >> 8 ) ^ ( ( ( checksum & 0xff0000 ) >> 16 ) ^ ( ( checksum & 0xff000000 ) >> 24 ) ) );
			idStr::snPrintf( s_chk, 3, "%02X", chk8 );
			if( idStr::Icmp( l_chk[i_key], s_chk ) != 0 )
			{
				offline_valid[ i_key ] = false;
				continue;
			}
		}
	}

	if( !offline_valid[ 0 ] || !offline_valid[1] )
	{
		return false;
	}

	// offline checks passed, we'll return true and optionally emit key check requests
	// the function should only modify the key states if the offline checks passed successfully

	// set the keys, don't send a game auth if we are net connecting
	idStr::Copynz( cdkey, lkey[0], CDKEY_BUF_LEN );
	netConnect ? cdkey_state = CDKEY_OK : cdkey_state = CDKEY_CHECKING;
	if( fileSystem->HasD3XP() )
	{
		idStr::Copynz( xpkey, lkey[1], CDKEY_BUF_LEN );
		netConnect ? xpkey_state = CDKEY_OK : xpkey_state = CDKEY_CHECKING;
	}
	else
	{
		xpkey_state = CDKEY_NA;
	}
	if( !netConnect )
	{
		EmitGameAuth();
	}
	SetCDKeyGuiVars();

	return true;
}

/*
===============
idSessionLocal::CDKeysAreValid
checking that the key is present and uses only valid characters
if d3xp is installed, check for a valid xpkey as well
emit an auth packet to the master if possible and needed
===============
*/
bool idSessionLocal::CDKeysAreValid( bool strict )
{
	int i;
	bool emitAuth = false;

	if( cdkey_state == CDKEY_UNKNOWN )
	{
		if( strlen( cdkey ) != CDKEY_BUF_LEN - 1 )
		{
			cdkey_state = CDKEY_INVALID;
		}
		else
		{
			for( i = 0; i < CDKEY_BUF_LEN - 1; i++ )
			{
				if( !strchr( CDKEY_DIGITS, cdkey[i] ) )
				{
					cdkey_state = CDKEY_INVALID;
					break;
				}
			}
		}
		if( cdkey_state == CDKEY_UNKNOWN )
		{
			cdkey_state = CDKEY_CHECKING;
			emitAuth = true;
		}
	}
	if( xpkey_state == CDKEY_UNKNOWN )
	{
		if( fileSystem->HasD3XP() )
		{
			if( strlen( xpkey ) != CDKEY_BUF_LEN - 1 )
			{
				xpkey_state = CDKEY_INVALID;
			}
			else
			{
				for( i = 0; i < CDKEY_BUF_LEN - 1; i++ )
				{
					if( !strchr( CDKEY_DIGITS, xpkey[i] ) )
					{
						xpkey_state = CDKEY_INVALID;
					}
				}
			}
			if( xpkey_state == CDKEY_UNKNOWN )
			{
				xpkey_state = CDKEY_CHECKING;
				emitAuth = true;
			}
		}
		else
		{
			xpkey_state = CDKEY_NA;
		}
	}
	if( emitAuth )
	{
		EmitGameAuth();
	}
	// make sure to keep the mainmenu gui up to date in case we made state changes
	SetCDKeyGuiVars();
	if( strict )
	{
		return cdkey_state == CDKEY_OK && ( xpkey_state == CDKEY_OK || xpkey_state == CDKEY_NA );
	}
	else
	{
		return ( cdkey_state == CDKEY_OK || cdkey_state == CDKEY_CHECKING ) && ( xpkey_state == CDKEY_OK || xpkey_state == CDKEY_CHECKING || xpkey_state == CDKEY_NA );
	}
}

/*
===============
idSessionLocal::WaitingForGameAuth
===============
*/
bool idSessionLocal::WaitingForGameAuth()
{
	return authEmitTimeout != 0;
}

/*
===============
idSessionLocal::CDKeysAuthReply
===============
*/
void idSessionLocal::CDKeysAuthReply( bool valid, const char* auth_msg )
{
	//assert( authEmitTimeout > 0 );
	if( authWaitBox )
	{
		// close the wait box
		StopBox();
		authWaitBox = false;
	}
	if( !valid )
	{
		common->DPrintf( "auth key is invalid\n" );
		authMsg = auth_msg;
		if( cdkey_state == CDKEY_CHECKING )
		{
			cdkey_state = CDKEY_INVALID;
		}
		if( xpkey_state == CDKEY_CHECKING )
		{
			xpkey_state = CDKEY_INVALID;
		}
	}
	else
	{
		common->DPrintf( "client is authed in\n" );
		if( cdkey_state == CDKEY_CHECKING )
		{
			cdkey_state = CDKEY_OK;
		}
		if( xpkey_state == CDKEY_CHECKING )
		{
			xpkey_state = CDKEY_OK;
		}
	}
	authEmitTimeout = 0;
	SetCDKeyGuiVars();
}

/*
===============
idSessionLocal::GetCurrentMapName
===============
*/
const char* idSessionLocal::GetCurrentMapName()
{
	return currentMapName.c_str();
}

/*
===============
idSessionLocal::GetSaveGameVersion
===============
*/
int idSessionLocal::GetSaveGameVersion()
{
	return savegameVersion;
}

/*
===============
idSessionLocal::GetAuthMsg
===============
*/
const char* idSessionLocal::GetAuthMsg()
{
	return authMsg.c_str();
}
