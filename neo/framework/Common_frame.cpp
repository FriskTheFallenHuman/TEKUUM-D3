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

/*

New for tech4x:

Unlike previous SMP work, the actual GPU command drawing is done in the main thread, which avoids the
OpenGL problems with needing windows to be created by the same thread that creates the context, as well
as the issues with passing context ownership back and forth on the 360.

The game tic and the generation of the draw command list is now run in a separate thread, and overlapped
with the interpretation of the previous draw command list.

While the game tic should be nicely contained, the draw command generation winds through the user interface
code, and is potentially hazardous.  For now, the overlap will be restricted to the renderer back end,
which should also be nicely contained.

*/
#define DEFAULT_FIXED_TIC "0"
#define DEFAULT_NO_SLEEP "0"

idCVar com_deltaTimeClamp( "com_deltaTimeClamp", "50", CVAR_INTEGER, "don't process more than this time in a single frame" );

idCVar com_fixedTic( "com_fixedTic", DEFAULT_FIXED_TIC, CVAR_BOOL, "run a single game frame per render frame", -1, 10 );
idCVar com_noSleep( "com_noSleep", DEFAULT_NO_SLEEP, CVAR_BOOL, "don't sleep if the game is running too fast" );
idCVar com_aviDemoSamples( "com_aviDemoSamples", "16", CVAR_SYSTEM, "" );
idCVar com_aviDemoWidth( "com_aviDemoWidth", "256", CVAR_SYSTEM, "" );
idCVar com_aviDemoHeight( "com_aviDemoHeight", "256", CVAR_SYSTEM, "" );
idCVar com_aviDemoTics( "com_aviDemoTics", "2", CVAR_SYSTEM | CVAR_INTEGER, "", 1, 60 );
idCVar com_skipGameDraw( "com_skipGameDraw", "0", CVAR_SYSTEM | CVAR_BOOL, "" );

/*
===============
idSessionLocal::DrawWipeModel

Draw the fade material over everything that has been drawn
===============
*/
void	idSessionLocal::DrawWipeModel()
{
	int		latchedTic = com_ticNumber;

	if( wipeStartTic >= wipeStopTic )
	{
		return;
	}

	if( !wipeHold && latchedTic >= wipeStopTic )
	{
		return;
	}

	float fade = ( float )( latchedTic - wipeStartTic ) / ( wipeStopTic - wipeStartTic );
	renderSystem->SetColor4( 1, 1, 1, fade );
	renderSystem->DrawStretchPic( 0, 0, 640, 480, 0, 0, 1, 1, wipeMaterial );
}

/*
===============
idSessionLocal::Draw
===============
*/
void idSessionLocal::Draw()
{
	bool fullConsole = false;

	if( insideExecuteMapChange )
	{
		if( guiLoading )
		{
			guiLoading->Redraw( com_frameTime );
		}
		if( guiActive == guiMsg )
		{
			guiMsg->Redraw( com_frameTime );
		}
	}
	else if( guiTest )
	{
		// if testing a gui, clear the screen and draw it
		// clear the background, in case the tested gui is transparent
		// NOTE that you can't use this for aviGame recording, it will tick at real com_frameTime between screenshots..
		renderSystem->SetColor( colorBlack );
		renderSystem->DrawStretchPic( 0, 0, 640, 480, 0, 0, 1, 1, declManager->FindMaterial( "_white" ) );
		guiTest->Redraw( com_frameTime );
	}
	else if( guiActive && !guiActive->State().GetBool( "gameDraw" ) )
	{

		// draw the frozen gui in the background
		if( guiActive == guiMsg && guiMsgRestore )
		{
			guiMsgRestore->Redraw( com_frameTime );
		}

		// draw the menus full screen
		if( guiActive == guiTakeNotes && !com_skipGameDraw.GetBool() )
		{
			game->Draw( GetLocalClientNum() );
		}

		guiActive->Redraw( com_frameTime );
	}
	else if( readDemo )
	{
		rw->RenderScene( &currentDemoRenderView );
		renderSystem->DrawDemoPics();
	}
	else if( mapSpawned )
	{
		bool gameDraw = false;
		// normal drawing for both single and multi player
		if( !com_skipGameDraw.GetBool() && GetLocalClientNum() >= 0 )
		{
			// draw the game view
			int	start = Sys_Milliseconds();
			gameDraw = game->Draw( GetLocalClientNum() );
			int end = Sys_Milliseconds();
			time_gameDraw += ( end - start );	// note time used for com_speeds
		}
		if( !gameDraw )
		{
			renderSystem->SetColor( colorBlack );
			renderSystem->DrawStretchPic( 0, 0, 640, 480, 0, 0, 1, 1, declManager->FindMaterial( "_white" ) );
		}

		// save off the 2D drawing from the game
		if( writeDemo )
		{
			renderSystem->WriteDemoPics();
		}
	}
	else
	{
#if ID_CONSOLE_LOCK
		if( com_allowConsole.GetBool() )
		{
			console->Draw( true );
		}
		else
		{
			emptyDrawCount++;
			if( emptyDrawCount > 5 )
			{
				// it's best if you can avoid triggering the watchgod by doing the right thing somewhere else
				assert( false );
				common->Warning( "idSession: triggering mainmenu watchdog" );
				emptyDrawCount = 0;
				StartMenu();
			}
			renderSystem->SetColor4( 0, 0, 0, 1 );
			renderSystem->DrawStretchPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1, declManager->FindMaterial( "_white" ) );
		}
#else
		// draw the console full screen - this should only ever happen in developer builds
		console->Draw( true );
#endif
		fullConsole = true;
	}

#if ID_CONSOLE_LOCK
	if( !fullConsole && emptyDrawCount )
	{
		common->DPrintf( "idSession: %d empty frame draws\n", emptyDrawCount );
		emptyDrawCount = 0;
	}
	fullConsole = false;
#endif

	// draw the wipe material on top of this if it hasn't completed yet
	DrawWipeModel();

	// draw debug graphs
	DrawCmdGraph();

	// draw the half console / notify console on top of everything
	if( !fullConsole )
	{
		console->Draw( false );
	}
}

/*
===============
idSessionLocal::UpdateScreen
===============
*/
void idSessionLocal::UpdateScreen( bool captureToImage, bool outOfSequence, bool swapBuffers )
{

#if defined(USE_MFC_TOOLS)
	if( com_editors )
	{
		if( !Sys_IsWindowVisible() )
		{
			return;
		}
	}
#endif

	if( insideUpdateScreen )
	{
		return;
//		common->FatalError( "idSessionLocal::UpdateScreen: recursively called" );
	}

	insideUpdateScreen = true;

	// if this is a long-operation update and we are in windowed mode,
	// release the mouse capture back to the desktop
	if( outOfSequence )
	{
		Sys_GrabMouseCursor( false );
	}

//#define USE_OLD_SYNCING

#if defined(USE_OLD_SYNCING)
	//renderSystem->BeginFrame( renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight() );
	const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers_FinishCommandBuffers();
#endif

	// draw everything
	Draw();

#if 0 //!defined(__ANDROID__)
	if( captureToImage )
	{
		renderSystem->CaptureRenderToImage( "_currentRender", false );
	}
#endif

	// RB begin

#if defined(USE_OLD_SYNCING)
	renderSystem->RenderCommandBuffers( cmd );

	if( com_speeds.GetBool() || com_showFPS.GetInteger() == 1 )
	{
		renderSystem->SwapCommandBuffers_FinishRendering( &time_frontend, &time_backend, &time_shadows, &time_gpu, swapBuffers );
	}
	else
	{
		renderSystem->SwapCommandBuffers_FinishRendering( NULL, NULL, NULL, NULL, swapBuffers );
	}
#else
	// BFG style

	// this should exit right after vsync, with the GPU idle and ready to draw
	const emptyCommand_t* cmd;

	if( com_speeds.GetBool() || com_showFPS.GetInteger() == 1 )
	{
		cmd = renderSystem->SwapCommandBuffers( &time_frontend, &time_backend, &time_shadows, &time_gpu, swapBuffers );
	}
	else
	{
		cmd = renderSystem->SwapCommandBuffers( NULL, NULL, NULL, NULL, swapBuffers );
	}

	// get the GPU busy with new commands
	renderSystem->RenderCommandBuffers( cmd );
#endif

#undef USE_OLD_SYNCING

	// RB end

	insideUpdateScreen = false;
}

/*
===============
idSessionLocal::Frame
===============
*/
extern bool CheckOpenALDeviceAndRecoverIfNeeded();
void idSessionLocal::Frame()
{

	if( com_asyncSound.GetInteger() == 0 )
	{
		soundSystem->AsyncUpdateWrite( Sys_Milliseconds() );
	}

	// DG: periodically check if sound device is still there and try to reset it if not
	//     (calling this from idSoundSystem::AsyncUpdate(), which runs in a separate thread
	//      by default, causes a deadlock when calling idCommon->Warning())
	CheckOpenALDeviceAndRecoverIfNeeded();

	// Editors that completely take over the game
	if( com_editorActive && ( com_editors & ( EDITOR_RADIANT | EDITOR_GUI ) ) )
	{
		return;
	}

	// if the console is down, we don't need to hold
	// the mouse cursor
	if( console->Active() || com_editorActive )
	{
		Sys_GrabMouseCursor( false );
	}
	else
	{
		Sys_GrabMouseCursor( true );
	}

	// save the screenshot and audio from the last draw if needed
	if( aviCaptureMode )
	{
		idStr	name;

		name = va( "demos/%s/%s_%05i.tga", aviDemoShortName.c_str(), aviDemoShortName.c_str(), aviTicStart );

		float ratio = 30.0f / ( 1000.0f / USERCMD_MSEC / com_aviDemoTics.GetInteger() );
		aviDemoFrameCount += ratio;
		if( aviTicStart + 1 != ( int )aviDemoFrameCount )
		{
			// skipped frames so write them out
			int c = aviDemoFrameCount - aviTicStart;
			while( c-- )
			{
				renderSystem->TakeScreenshot( com_aviDemoWidth.GetInteger(), com_aviDemoHeight.GetInteger(), name, com_aviDemoSamples.GetInteger(), NULL );
				name = va( "demos/%s/%s_%05i.tga", aviDemoShortName.c_str(), aviDemoShortName.c_str(), ++aviTicStart );
			}
		}
		aviTicStart = aviDemoFrameCount;

		// remove any printed lines at the top before taking the screenshot
		console->ClearNotifyLines();

		// this will call Draw, possibly multiple times if com_aviDemoSamples is > 1
		renderSystem->TakeScreenshot( com_aviDemoWidth.GetInteger(), com_aviDemoHeight.GetInteger(), name, com_aviDemoSamples.GetInteger(), NULL );
	}

	//--------------------------------------------
	// wait for the GPU to finish drawing
	//
	// It is imporant to minimize the time spent between this
	// section and the call to renderSystem->RenderCommandBuffers(),
	// because the GPU is completely idle.
	//--------------------------------------------
	// this should exit right after vsync, with the GPU idle and ready to draw
	// This may block if the GPU isn't finished renderng the previous frame.

//	frameTiming.startSyncTime = Sys_Microseconds();

//	const emptyCommand_t* renderCommands = NULL;
//	if( com_smp.GetBool() )
//	{
//		renderCommands = renderSystem->SwapCommandBuffers( &time_frontend, &time_backend, &time_shadows, &time_gpu );
//	}
//	else
	{
		// the GPU will stay idle through command generation for minimal
		// input latency
//		renderSystem->SwapCommandBuffers_FinishRendering( &time_frontend, &time_backend, &time_shadows, &time_gpu );
	}
//	frameTiming.finishSyncTime = Sys_Microseconds();

#if 1

	//--------------------------------------------
	// Determine how many game tics we are going to run,
	// now that the previous frame is completely finished.
	//
	// It is important that any waiting on the GPU be done
	// before this, or there will be a bad stuttering when
	// dropping frames for performance management.
	//--------------------------------------------

	// input:
	// thisFrameTime
	// com_noSleep
	// com_engineHz
	// com_fixedTic
	// com_deltaTimeClamp
	// IsMultiplayer
	//
	// in/out state:
	// gameFrame
	// gameTimeResidual
	// lastFrameTime
	// syncNextFrame
	//
	// Output:
	// numGameFrames

	// How many game frames to run
	int numGameFrames = 0;

	for( ;; )
	{
		const int thisFrameTime = Sys_Milliseconds();
		static int lastFrameTime = thisFrameTime;	// initialized only the first time
		const int deltaMilliseconds = thisFrameTime - lastFrameTime;
		lastFrameTime = thisFrameTime;

		// if there was a large gap in time since the last frame, or the frame
		// rate is very very low, limit the number of frames we will run
		const int clampedDeltaMilliseconds = Min( deltaMilliseconds, com_deltaTimeClamp.GetInteger() );

		gameTimeResidual += clampedDeltaMilliseconds * com_timescale.GetFloat();

		// don't run any frames when paused
		/*
		if( pauseGame )
		{
			gameFrame++;
			gameTimeResidual = 0;
			break;
		}
		*/

		// debug cvar to force multiple game tics
		if( com_fixedTic.GetInteger() > 0 )
		{
			numGameFrames = com_fixedTic.GetInteger();
			gameFrame += numGameFrames;
			gameTimeResidual = 0;
			break;
		}

		if( syncNextGameFrame )
		{
			// don't sleep at all
			syncNextGameFrame = false;
			gameFrame++;
			numGameFrames++;
			gameTimeResidual = 0;
			break;
		}

		for( ;; )
		{
			// How much time to wait before running the next frame,
			// based on com_engineHz
			const int frameDelay = FRAME_TO_MSEC( gameFrame + 1 ) - FRAME_TO_MSEC( gameFrame );
			if( gameTimeResidual < frameDelay )
			{
				break;
			}
			gameTimeResidual -= frameDelay;
			gameFrame++;
			numGameFrames++;
			// if there is enough residual left, we may run additional frames
		}

		if( numGameFrames > 0 )
		{
			// ready to actually run them
			break;
		}

		// if we are vsyncing, we always want to run at least one game
		// frame and never sleep, which might happen due to scheduling issues
		// if we were just looking at real time.
		if( com_noSleep.GetBool() )
		{
			numGameFrames = 1;
			gameFrame += numGameFrames;
			gameTimeResidual = 0;
			break;
		}

		// not enough time has passed to run a frame, as might happen if
		// we don't have vsync on, or the monitor is running at 120hz while
		// com_engineHz is 60, so sleep a bit and check again
		Sys_Sleep( 0 );
	}

#else
	// at startup, we may be backwards
	if( latchedTicNumber > com_ticNumber )
	{
		latchedTicNumber = com_ticNumber;
	}

	// se how many tics we should have before continuing
	int	minTic = latchedTicNumber + 1;
	if( com_minTics.GetInteger() > 1 )
	{
		minTic = lastGameTic + com_minTics.GetInteger();
	}

	if( readDemo )
	{
		if( !timeDemo && numDemoFrames != 1 )
		{
			minTic = lastDemoTic + USERCMD_PER_DEMO_FRAME;
		}
		else
		{
			// timedemos and demoshots will run as fast as they can, other demos
			// will not run more than 30 hz
			minTic = latchedTicNumber;
		}
	}
	else if( writeDemo )
	{
		minTic = lastGameTic + USERCMD_PER_DEMO_FRAME;		// demos are recorded at 30 hz
	}

	// fixedTic lets us run a forced number of usercmd each frame without timing
	if( com_fixedTic.GetInteger() )
	{
		minTic = latchedTicNumber;
	}

	// FIXME: deserves a cleanup and abstraction
#if defined( _WIN32 )
	// Spin in place if needed.  The game should yield the cpu if
	// it is running over 60 hz, because there is fundamentally
	// nothing useful for it to do.
	while( 1 )
	{
		latchedTicNumber = com_ticNumber;
		if( latchedTicNumber >= minTic )
		{
			break;
		}
		Sys_Sleep( 1 );
	}
#else
	while( 1 )
	{
		latchedTicNumber = com_ticNumber;
		if( latchedTicNumber >= minTic )
		{
			break;
		}
		Sys_WaitForEvent( TRIGGER_EVENT_ONE );
	}
#endif


#endif

#if defined(USE_CDKEY)
	if( authEmitTimeout )
	{
		// waiting for a game auth
		if( Sys_Milliseconds() > authEmitTimeout )
		{
			// expired with no reply
			// means that if a firewall is blocking the master, we will let through
			common->DPrintf( "no reply from auth\n" );
			if( authWaitBox )
			{
				// close the wait box
				StopBox();
				authWaitBox = false;
			}
			if( cdkey_state == CDKEY_CHECKING )
			{
				cdkey_state = CDKEY_OK;
			}
			if( xpkey_state == CDKEY_CHECKING )
			{
				xpkey_state = CDKEY_OK;
			}
			// maintain this empty as it's set by auth denials
			authMsg.Empty();
			authEmitTimeout = 0;
			SetCDKeyGuiVars();
		}
	}
#endif

	// send frame and mouse events to active guis
	GuiFrameEvents();

	// advance demos
	if( readDemo )
	{
		AdvanceRenderDemo( false );
		return;
	}

	//------------ single player game tics --------------

	if( !mapSpawned || guiActive )
	{
		if( !com_asyncInput.GetBool() )
		{
			// early exit, won't do RunGameTic .. but still need to update mouse position for GUIs

			// RB begin
			usercmdGen->BuildCurrentUsercmd();
			// RB end
		}
	}

	if( !mapSpawned )
	{
		return;
	}

	if( guiActive )
	{
		lastGameTic = latchedTicNumber;
		return;
	}

	// in message box / GUIFrame, idSessionLocal::Frame is used for GUI interactivity
	// but we early exit to avoid running game frames
	if( idAsyncNetwork::IsActive() )
	{
		return;
	}

	// check for user info changes
	if( cvarSystem->GetModifiedFlags() & CVAR_USERINFO )
	{
		mapSpawnData.userInfo[0] = *cvarSystem->MoveCVarsToDict( CVAR_USERINFO );
		game->SetUserInfo( 0, mapSpawnData.userInfo[0], false, false );
		cvarSystem->ClearModifiedFlags( CVAR_USERINFO );
	}

	// see how many usercmds we are going to run
	int	numCmdsToRun = latchedTicNumber - lastGameTic;

	// don't let a long onDemand sound load unsync everything
	if( timeHitch )
	{
		int	skip = timeHitch / USERCMD_MSEC;
		lastGameTic += skip;
		numCmdsToRun -= skip;
		timeHitch = 0;
	}

	// don't get too far behind after a hitch
	if( numCmdsToRun > 10 )
	{
		lastGameTic = latchedTicNumber - 10;
	}

	// never use more than USERCMD_PER_DEMO_FRAME,
	// which makes it go into slow motion when recording
	if( writeDemo )
	{
		int fixedTic = USERCMD_PER_DEMO_FRAME;
		// we should have waited long enough
		if( numCmdsToRun < fixedTic )
		{
			common->Error( "idSessionLocal::Frame: numCmdsToRun < fixedTic" );
		}
		// we may need to dump older commands
		lastGameTic = latchedTicNumber - fixedTic;
	}
	else if( com_fixedTic.GetInteger() > 0 )
	{
		// this may cause commands run in a previous frame to
		// be run again if we are going at above the real time rate
		lastGameTic = latchedTicNumber - com_fixedTic.GetInteger();
	}
	else if(	aviCaptureMode )
	{
		lastGameTic = latchedTicNumber - com_aviDemoTics.GetInteger();
	}

	// force only one game frame update this frame.  the game code requests this after skipping cinematics
	// so we come back immediately after the cinematic is done instead of a few frames later which can
	// cause sounds played right after the cinematic to not play.
	if( syncNextGameFrame )
	{
		lastGameTic = latchedTicNumber - 1;
		syncNextGameFrame = false;
	}

	// create client commands, which will be sent directly
	// to the game
	if( com_showTics.GetBool() )
	{
		common->Printf( "%i ", latchedTicNumber - lastGameTic );
	}

	//int	gameTicsToRun = latchedTicNumber - lastGameTic;
	for( int i = 0; i < numGameFrames; i++ )
	{
		RunGameTic();
		if( !mapSpawned )
		{
			// exited game play
			break;
		}
		if( syncNextGameFrame )
		{
			// long game frame, so break out and continue executing as if there was no hitch
			break;
		}
	}
}