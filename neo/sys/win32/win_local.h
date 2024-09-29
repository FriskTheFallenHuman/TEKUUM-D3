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

#ifndef __WIN_LOCAL_H__
#define __WIN_LOCAL_H__

#include <windows.h>

// RB: replaced QGL with GLEW
#if !defined(USE_VULKAN)
	#include <GL/wglew.h> // windows OpenGL extensions
#endif
// RB end

#include "win_input.h"


#define	WINDOW_STYLE	(WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE | WS_THICKFRAME)

void	Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void* ptr, int inputDeviceNum );

void	Sys_CreateConsole();
void	Sys_DestroyConsole();

char*	Sys_ConsoleInput();

void	Win_SetErrorText( const char* text );

cpuid_t	Sys_GetCPUId();

// add additional non keyboard / non mouse movement on top of the keyboard move cmd

void	IN_DeactivateMouseIfWindowed();
void	IN_DeactivateMouse();
void	IN_ActivateMouse();

void	IN_Frame();

const unsigned char* Win_GetScanTable( void );

void Conbuf_AppendText( const char* msg );

// window procedure
LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


struct Win32Vars_t
{
	HWND			hWnd;
	HINSTANCE		hInstance;

	bool			activeApp;			// changed with WM_ACTIVATE messages
	bool			mouseReleased;		// when the game has the console down or is doing a long operation
	bool			movingWindow;		// inhibit mouse grab when dragging the window
	bool			mouseGrabbed;		// current state of grab and hide

	cpuid_t			cpuid;

	bool			windowClassRegistered;

	WNDPROC			wndproc;

	HDC				hDC;							// handle to device context

	HGLRC			hGLRC;							// handle to GL rendering context
	PIXELFORMATDESCRIPTOR pfd;
	int				pixelformat;

	int				desktopBitsPixel;
	int				desktopWidth, desktopHeight;

	int				cdsFullscreen;	// 0 = not fullscreen, otherwise monitor number

	unsigned short	oldHardwareGamma[3][256];
	// desktop gamma is saved here for restoration at exit

	static idCVar	sys_cpustring;
	static idCVar	in_mouse;
	static idCVar	win_outputDebugString;
	static idCVar	win_outputEditString;
	static idCVar	win_viewlog;
	static idCVar	win_timerUpdate;

	CRITICAL_SECTION criticalSections[MAX_CRITICAL_SECTIONS];

	HINSTANCE		hInstDI;			// direct input

	LPDIRECTINPUT8			g_pdi;
	LPDIRECTINPUTDEVICE8	g_pMouse;
	LPDIRECTINPUTDEVICE8	g_pKeyboard;
	idJoystickWin32			g_Joystick;
};

extern Win32Vars_t	win32;

#endif /* !__WIN_LOCAL_H__ */
