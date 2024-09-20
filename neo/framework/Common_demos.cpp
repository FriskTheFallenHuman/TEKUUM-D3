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

#include "Session_local.h"

/*
================
FindUnusedFileName
================
*/
static idStr FindUnusedFileName( const char* format )
{
	idStr filename;

	for( int i = 0 ; i < 999 ; i++ )
	{
		filename.Format( format, i );
		int len = fileSystem->ReadFile( filename, NULL, NULL );
		if( len <= 0 )
		{
			return filename;	// file doesn't exist
		}
	}

	return filename;
}

//extern idCVar com_smp;            // SRS - No longer require non-smp mode for demos

void WriteDeclCache( idDemoFile* f, int demoCategory, int demoCode, declType_t  declType )
{
	f->WriteInt( demoCategory );
	f->WriteInt( demoCode );

	int numDecls = 0;

	for( int i = 0; i < declManager->GetNumDecls( declType ); i++ )
	{
		const idDecl* decl = declManager->DeclByIndex( declType, i, false );
		if( decl && decl->IsValid() )
		{
			++numDecls;
		}
	}

	f->WriteInt( numDecls );
	for( int i = 0; i < declManager->GetNumDecls( declType ); i++ )
	{
		const idDecl* decl = declManager->DeclByIndex( declType, i, false );
		if( decl && decl->IsValid() )
		{
			f->WriteHashString( decl->GetName() );
		}
	}
}

/*
================
idSessionLocal::StartRecordingRenderDemo
================
*/
void idSessionLocal::StartRecordingRenderDemo( const char* demoName )
{
	if( writeDemo )
	{
		// allow it to act like a toggle
		StopRecordingRenderDemo();
		return;
	}

	if( !demoName[0] )
	{
		common->Printf( "idSessionLocal::StartRecordingRenderDemo: no name specified\n" );
		return;
	}

	console->Close();

//  com_smp.SetInteger( 0 );        // SRS - No longer require non-smp mode for demos

	writeDemo = new idDemoFile;
	if( !writeDemo->OpenForWriting( demoName ) )
	{
		common->Printf( "error opening %s\n", demoName );
		delete writeDemo;
		writeDemo = NULL;
		return;
	}

	common->Printf( "recording to %s\n", writeDemo->GetName() );

	writeDemo->WriteInt( DS_VERSION );
	writeDemo->WriteInt( RENDERDEMO_VERSION );

	// if we are in a map already, dump the current state
	sw->StartWritingDemo( writeDemo );
	rw->StartWritingDemo( writeDemo );
}

/*
================
idSessionLocal::StopRecordingRenderDemo
================
*/
void idSessionLocal::StopRecordingRenderDemo()
{
	if( !writeDemo )
	{
		common->Printf( "idSessionLocal::StopRecordingRenderDemo: not recording\n" );
		return;
	}
	sw->StopWritingDemo();
	rw->StopWritingDemo();

	writeDemo->Close();
	common->Printf( "stopped recording %s.\n", writeDemo->GetName() );
	delete writeDemo;
	writeDemo = NULL;
//	com_smp.SetInteger( 1 ); // motorsep 12-30-2014; turn multithreading back on;  SRS - No longer require non-smp mode for demos
}

/*
================
idSessionLocal::StopPlayingRenderDemo

Reports timeDemo numbers and finishes any avi recording
================
*/
void idSessionLocal::StopPlayingRenderDemo()
{
	if( !readDemo )
	{
		timeDemo = TD_NO;
		return;
	}

	// Record the stop time before doing anything that could be time consuming
	int timeDemoStopTime = Sys_Milliseconds();

	EndAVICapture();

	readDemo->Close();

	sw->StopAllSounds();
	soundSystem->SetPlayingSoundWorld( menuSoundWorld );

	common->Printf( "stopped playing %s.\n", readDemo->GetName() );
	delete readDemo;
	readDemo = NULL;

	if( timeDemo )
	{
		// report the stats
		float	demoSeconds = ( timeDemoStopTime - timeDemoStartTime ) * 0.001f;
		float	demoFPS = numDemoFrames / demoSeconds;
		idStr	message = va( "%i frames rendered in %3.1f seconds = %3.1f fps\n", numDemoFrames, demoSeconds, demoFPS );

		common->Printf( "%s", message.c_str() );
		if( timeDemo == TD_YES_THEN_QUIT )
		{
			cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
		}
		else
		{
			soundSystem->SetMute( true );
			MessageBox( MSG_OK, message, "Time Demo Results", true );
			soundSystem->SetMute( false );
		}
		timeDemo = TD_NO;
	}

//    com_smp.SetInteger( 1 ); // motorsep 12-30-2014; turn multithreading back on;  SRS - No longer require non-smp mode for demos
}

/*
================
idSessionLocal::DemoShot

A demoShot is a single frame demo
================
*/
void idSessionLocal::DemoShot( const char* demoName )
{
	StartRecordingRenderDemo( demoName );

	// force draw one frame
	const bool captureToImage = false;
	UpdateScreen( captureToImage );

	StopRecordingRenderDemo();
}

/*
================
idSessionLocal::StartPlayingRenderDemo
================
*/
void idSessionLocal::StartPlayingRenderDemo( idStr demoName )
{
	if( !demoName[0] )
	{
		common->Printf( "idSessionLocal::StartPlayingRenderDemo: no name specified\n" );
		return;
	}

//  com_smp.SetInteger( 0 );        // SRS - No longer require non-smp mode for demos

	// make sure localSound / GUI intro music shuts up
	sw->StopAllSounds();
	sw->PlayShaderDirectly( "", 0 );
	menuSoundWorld->StopAllSounds();
	menuSoundWorld->PlayShaderDirectly( "", 0 );

	// exit any current game
	Stop();

	// automatically put the console away
	console->Close();

	guiLoading = uiManager->FindGui( "guis/map/loading.gui", true, false, true );
	guiLoading->SetStateString( "demo", common->GetLanguageDict()->GetString( "#str_02087" ) );

	// bring up the loading screen manually, since demos won't
	// call ExecuteMapChange()
	readDemo = new idDemoFile;
	demoName.DefaultFileExtension( ".demo" );
	if( !readDemo->OpenForReading( demoName ) )
	{
		common->Printf( "couldn't open %s\n", demoName.c_str() );
		delete readDemo;
		readDemo = NULL;
		Stop();                   // SRS - drop back to main menu if demo playback fails
		StartMenu();
		soundSystem->SetMute( false );
		return;
	}

	int opcode = -1, demoVersion = -1;
	readDemo->ReadInt( opcode );
	if( opcode != DS_VERSION )
	{
		common->Printf( "StartPlayingRenderDemo invalid demo file\n" );
		readDemo->Close();
		delete readDemo;
		readDemo = NULL;
		Stop();                   // SRS - drop back to main menu if demo playback fails
		StartMenu();
		soundSystem->SetMute( false );
		return;
	}

	readDemo->ReadInt( demoVersion );
	if( demoVersion != RENDERDEMO_VERSION )
	{
		common->Printf( "StartPlayingRenderDemo got version %d, expected version %d\n", demoVersion, RENDERDEMO_VERSION );
		readDemo->Close();
		delete readDemo;
		readDemo = NULL;
		Stop();                   // SRS - drop back to main menu if demo playback fails
		StartMenu();
		soundSystem->SetMute( false );
		return;
	}

	numDemoFrames = 0;                      // SRS - Moved ahead of first call to AdvanceRenderDemo to properly handle demoshots
	//numShotFrames = 0;                      // SRS - Initialize count of demoShot frames to play before timeout to main menu

	insideExecuteMapChange = true;
	const bool captureToImage = true;
	UpdateScreen( captureToImage );
	insideExecuteMapChange = false;

	guiLoading->SetStateString( "demo", "" );

	// setup default render demo settings
	// that's default for <= Doom3 v1.1
	renderdemoVersion = 1;
	savegameVersion = 16;

	renderSystem->BeginLevelLoad();         // SRS - Free static data from previous level before loading demo assets
	soundSystem->BeginLevelLoad();          // SRS - Free sound media from previous level before loading demo assets
	declManager->BeginLevelLoad();          // SRS - Clear declaration manager data before loading demo assets
	uiManager->BeginLevelLoad();            // SRS - Clear gui manager data before loading demo assets

	AdvanceRenderDemo( true );              // SRS - Call AdvanceRenderDemo() once to load map and initial assets (like level load)

	renderSystem->EndLevelLoad();           // SRS - Define static data for use by RB_StencilShadowPass if stencil shadows enabled
	soundSystem->EndLevelLoad( "" );
	declManager->EndLevelLoad();
	uiManager->EndLevelLoad( "" );          // SRS - FIXME: No gui assets are currently saved/reloaded in demo file, fix later?

	//Game()->StartDemoPlayback( renderWorld );

	rw->GenerateAllInteractions();

	soundSystem->SetPlayingSoundWorld( sw );

	lastDemoTic = -1;
	timeDemoStartTime = Sys_Milliseconds();
}

/*
================
idSessionLocal::TimeRenderDemo
================
*/
void idSessionLocal::TimeRenderDemo( const char* demoName, bool twice, bool quit )
{
	idStr demo = demoName;

	// no sound in time demos
	soundSystem->SetMute( true );

	StartPlayingRenderDemo( demo );

	if( twice && readDemo )
	{
		timeDemo = TD_YES;                      // SRS - Set timeDemo to TD_YES to disable time demo playback pause when window not in focus

		// cycle through once to precache everything
		guiLoading->SetStateString( "demo", common->GetLanguageDict()->GetString( "#str_04852" ) );
		guiLoading->StateChanged( Sys_Milliseconds() );
		while( readDemo )
		{
			insideExecuteMapChange = true;
			Sys_GenerateEvents();
			const bool captureToImage = false;
			UpdateScreen( captureToImage );
			insideExecuteMapChange = false;
			AdvanceRenderDemo( true );
			eventLoop->RunEventLoop();          // SRS - Run event loop to allow keyboard escape to cancel first pass of the demo
		}
		guiLoading->SetStateString( "demo", "" );
		StartPlayingRenderDemo( demo );
	}


	if( !readDemo )
	{
		timeDemo = TD_NO;                       // SRS - Make sure timeDemo flag is off if readDemo is NULL
		return;
	}

	if( quit )
	{
		// this allows hardware vendors to automate some testing
		timeDemo = TD_YES_THEN_QUIT;
	}
	else
	{
		timeDemo = TD_YES;
	}
}


/*
================
idSessionLocal::BeginAVICapture
================
*/
void idSessionLocal::BeginAVICapture( const char* demoName )
{
	idStr name = demoName;
	name.ExtractFileBase( aviDemoShortName );
	aviCaptureMode = true;
	aviDemoFrameCount = 0;
	aviTicStart = 0;
	sw->AVIOpen( va( "demos/%s/", aviDemoShortName.c_str() ), aviDemoShortName.c_str() );
}

/*
================
idSessionLocal::EndAVICapture
================
*/
void idSessionLocal::EndAVICapture()
{
	if( !aviCaptureMode )
	{
		return;
	}

	sw->AVIClose();

	// write a .roqParam file so the demo can be converted to a roq file
	idFile* f = fileSystem->OpenFileWrite( va( "demos/%s/%s.roqParam",
										   aviDemoShortName.c_str(), aviDemoShortName.c_str() ) );
	f->Printf( "INPUT_DIR demos/%s\n", aviDemoShortName.c_str() );
	f->Printf( "FILENAME demos/%s/%s.RoQ\n", aviDemoShortName.c_str(), aviDemoShortName.c_str() );
	f->Printf( "\nINPUT\n" );
	f->Printf( "%s_*.tga [00000-%05i]\n", aviDemoShortName.c_str(), ( int )( aviDemoFrameCount - 1 ) );
	f->Printf( "END_INPUT\n" );
	delete f;

	common->Printf( "captured %i frames for %s.\n", ( int )aviDemoFrameCount, aviDemoShortName.c_str() );

	aviCaptureMode = false;
}


/*
================
idSessionLocal::AVIRenderDemo
================
*/
void idSessionLocal::AVIRenderDemo( const char* _demoName )
{
	idStr	demoName = _demoName;	// copy off from va() buffer

	StartPlayingRenderDemo( demoName );
	if( !readDemo )
	{
		return;
	}

	BeginAVICapture( demoName.c_str() ) ;

	// I don't understand why I need to do this twice, something
	// strange with the nvidia swapbuffers?
	const bool captureToImage = false;
	UpdateScreen( captureToImage );
}

/*
================
idSessionLocal::AVICmdDemo
================
*/
void idSessionLocal::AVICmdDemo( const char* demoName )
{
	StartPlayingCmdDemo( demoName );

	BeginAVICapture( demoName ) ;
}

/*
================
idSessionLocal::AVIGame

Start AVI recording the current game session
================
*/
void idSessionLocal::AVIGame( const char* demoName )
{
	if( aviCaptureMode )
	{
		EndAVICapture();
		return;
	}

	if( !mapSpawned )
	{
		common->Printf( "No map spawned.\n" );
	}

	if( !demoName || !demoName[0] )
	{
		idStr filename = FindUnusedFileName( "demos/game%03i.game" );
		demoName = filename.c_str();

		// write a one byte stub .game file just so the FindUnusedFileName works,
		fileSystem->WriteFile( demoName, demoName, 1 );
	}

	BeginAVICapture( demoName ) ;
}

/*
================
idSessionLocal::CompressDemoFile
================
*/
void idSessionLocal::CompressDemoFile( const char* scheme, const char* demoName )
{
	idStr	fullDemoName = "demos/";
	fullDemoName += demoName;
	fullDemoName.DefaultFileExtension( ".demo" );
	idStr compressedName = fullDemoName;
	compressedName.StripFileExtension();
	compressedName.Append( "_compressed.demo" );

	int savedCompression = cvarSystem->GetCVarInteger( "com_compressDemos" );
	bool savedPreload = cvarSystem->GetCVarBool( "com_preloadDemos" );
	cvarSystem->SetCVarBool( "com_preloadDemos", false );
	cvarSystem->SetCVarInteger( "com_compressDemos", atoi( scheme ) );

	idDemoFile demoread, demowrite;
	if( !demoread.OpenForReading( fullDemoName ) )
	{
		common->Printf( "Could not open %s for reading\n", fullDemoName.c_str() );
		return;
	}
	if( !demowrite.OpenForWriting( compressedName ) )
	{
		common->Printf( "Could not open %s for writing\n", compressedName.c_str() );
		demoread.Close();
		cvarSystem->SetCVarBool( "com_preloadDemos", savedPreload );
		cvarSystem->SetCVarInteger( "com_compressDemos", savedCompression );
		return;
	}
	common->SetRefreshOnPrint( true );
	common->Printf( "Compressing %s to %s...\n", fullDemoName.c_str(), compressedName.c_str() );

	static const int bufferSize = 65535;
	char buffer[bufferSize];
	int bytesRead;
	while( 0 != ( bytesRead = demoread.Read( buffer, bufferSize ) ) )
	{
		demowrite.Write( buffer, bytesRead );
		common->Printf( "." );
	}

	demoread.Close();
	demowrite.Close();

	cvarSystem->SetCVarBool( "com_preloadDemos", savedPreload );
	cvarSystem->SetCVarInteger( "com_compressDemos", savedCompression );

	common->Printf( "Done\n" );
	common->SetRefreshOnPrint( false );

}

/*
================
Session_DemoShot_f
================
*/
CONSOLE_COMMAND_SHIP( demoShot, "writes a screenshot as a demo", NULL )
{
	if( args.Argc() != 2 )
	{
		idStr filename = FindUnusedFileName( "demos/shot%03i.demo" );
		sessLocal.DemoShot( filename );
	}
	else
	{
		sessLocal.DemoShot( va( "demos/shot_%s.demo", args.Argv( 1 ) ) );
	}
}

#ifndef	ID_DEDICATED
/*
================
Session_RecordDemo_f
================
*/
CONSOLE_COMMAND_SHIP( recordDemo, "records a demo", NULL )
{
	if( args.Argc() != 2 )
	{
		idStr filename = FindUnusedFileName( "demos/demo%03i.demo" );
		sessLocal.StartRecordingRenderDemo( filename );
	}
	else
	{
		sessLocal.StartRecordingRenderDemo( va( "demos/%s.demo", args.Argv( 1 ) ) );
	}
}

/*
================
Session_CompressDemo_f
================
*/
CONSOLE_COMMAND_SHIP( compressDemo, "compresses a demo file", idCmdSystem::ArgCompletion_DemoName )
{
	if( args.Argc() == 2 )
	{
		sessLocal.CompressDemoFile( "2", args.Argv( 1 ) );
	}
	else if( args.Argc() == 3 )
	{
		sessLocal.CompressDemoFile( args.Argv( 2 ), args.Argv( 1 ) );
	}
	else
	{
		common->Printf( "use: CompressDemo <file> [scheme]\nscheme is the same as com_compressDemo, defaults to 2" );
	}
}

/*
================
Session_StopRecordingDemo_f
================
*/
CONSOLE_COMMAND_SHIP( stopRecording, "stops demo recording", NULL )
{
	sessLocal.StopRecordingRenderDemo();
}


/*
================
Session_PlayDemo_f
================
*/
CONSOLE_COMMAND_SHIP( playDemo, "plays back a demo", idCmdSystem::ArgCompletion_DemoName )
{
	if( args.Argc() >= 2 )
	{
		sessLocal.StartPlayingRenderDemo( va( "demos/%s", args.Argv( 1 ) ) );
	}
}


/*
================
Session_TimeDemo_f
================
*/
CONSOLE_COMMAND_SHIP( timeDemo, "times a demo", idCmdSystem::ArgCompletion_DemoName )
{
	if( args.Argc() >= 2 )
	{
		sessLocal.TimeRenderDemo( va( "demos/%s", args.Argv( 1 ) ), ( args.Argc() > 2 ), false );
	}
}

/*
================
Session_TimeDemoQuit_f
================
*/
CONSOLE_COMMAND_SHIP( timeDemoQuit, "times a demo and quits", idCmdSystem::ArgCompletion_DemoName )
{
	sessLocal.TimeRenderDemo( va( "demos/%s", args.Argv( 1 ) ), ( args.Argc() > 2 ), true );    // SRS - fixed missing "twice" argument
}

/*
================
Session_AVIDemo_f
================
*/
CONSOLE_COMMAND_SHIP( aviDemo, "writes AVIs for a demo", idCmdSystem::ArgCompletion_DemoName )
{
	sessLocal.AVIRenderDemo( va( "demos/%s", args.Argv( 1 ) ) );
}

/*
================
Session_WriteCmdDemo_f
================
*/
CONSOLE_COMMAND_SHIP( writeCmdDemo, "writes a command demo", NULL )
{
	if( args.Argc() == 1 )
	{
		idStr	filename = FindUnusedFileName( "demos/cmdDemo%03i.cdemo" );
		sessLocal.WriteCmdDemo( filename );
	}
	else if( args.Argc() == 2 )
	{
		sessLocal.WriteCmdDemo( va( "demos/%s.cdemo", args.Argv( 1 ) ) );
	}
	else
	{
		common->Printf( "usage: writeCmdDemo [demoName]\n" );
	}
}

/*
================
Session_PlayCmdDemo_f
================
*/
CONSOLE_COMMAND_SHIP( playCmdDemo, "plays back a command demo", NULL )
{
	sessLocal.StartPlayingCmdDemo( args.Argv( 1 ) );
}

/*
================
Session_TimeCmdDemo_f
================
*/
CONSOLE_COMMAND_SHIP( timeCmdDemo, "times a command demo", NULL )
{
	sessLocal.TimeCmdDemo( args.Argv( 1 ) );
}

/*
================
Session_ExitCmdDemo_f
================
*/
CONSOLE_COMMAND_SHIP( exitCmdDemo, "exits a command demo", NULL )
{
	if( !sessLocal.cmdDemoFile )
	{
		common->Printf( "not reading from a cmdDemo\n" );
		return;
	}
	fileSystem->CloseFile( sessLocal.cmdDemoFile );
	common->Printf( "Command demo exited at logIndex %i\n", sessLocal.logIndex );
	sessLocal.cmdDemoFile = NULL;
}

/*
================
Session_AVICmdDemo_f
================
*/
CONSOLE_COMMAND_SHIP( aviCmdDemo, "writes AVIs for a command demo", NULL )
{
	sessLocal.AVICmdDemo( args.Argv( 1 ) );
}

/*
================
Session_AVIGame_f
================
*/
CONSOLE_COMMAND_SHIP( aviGame, "writes AVIs for the current game", NULL )
{
	sessLocal.AVIGame( args.Argv( 1 ) );
}
#endif