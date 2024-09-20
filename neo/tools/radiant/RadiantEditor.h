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

#pragma once

/*
=================================
This file holds the Radiant functionaly
that's not tied to any UI
=================================
*/

#define ID_SHOW_MODELS ( WM_USER + 28477 )

const int RAD_SHIFT =   0x01;
const int RAD_ALT =     0x02;
const int RAD_CONTROL = 0x04;
const int RAD_PRESS   = 0x08;

struct SCommandInfo
{
	char* m_strCommand;
	unsigned int   m_nKey;
	unsigned int   m_nModifiers;
	unsigned int m_nCommand;
};

struct SKeyInfo
{
	char* m_strName;
	unsigned int m_nVKKey;
};

extern int g_nCommandCount;
extern int g_nKeyCount;

extern HINSTANCE g_DoomInstance;
extern bool g_editorAlive;

/*
=================================
Radiant "Editor" Implementation
=================================
*/
void RadiantPrint( const char* text );
void RadiantShutdown();
void RadiantInit();
void RadiantRun();

bool SaveRegistryInfo( const char* pszName, void* pvBuf, long lSize );
bool LoadRegistryInfo( const char* pszName, void* pvBuf, long* plSize );

bool SaveWindowState( HWND hWnd, const char* pszName );
bool LoadWindowState( HWND hWnd, const char* pszName );

void Sys_UpdateStatusBar();
void Sys_Status( const char* psz, int part );