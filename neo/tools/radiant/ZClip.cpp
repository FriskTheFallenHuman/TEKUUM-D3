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

#include "qe3.h"
#include "Radiant.h"

#include "zclip.h"


CZClip::CZClip()
{
	LONG
	lSize = sizeof( m_bEnabled );
	if( !LoadRegistryInfo( "radiant_ZClipEnabled",	&m_bEnabled, &lSize ) )
	{
		m_bEnabled = false;
	}

	lSize = sizeof( m_iZClipTop );
	if( !LoadRegistryInfo( "radiant_ZClipTop",		&m_iZClipTop, &lSize ) )
	{
		m_iZClipTop = 64;
	}

	lSize = sizeof( m_iZClipBottom );
	if( !LoadRegistryInfo( "radiant_ZClipBottom",	&m_iZClipBottom, &lSize ) )
	{
		m_iZClipBottom = -64;
	}

	Legalise();
}

CZClip::~CZClip()
{
	// TODO: registry save

	SaveRegistryInfo( "radiant_ZClipEnabled", &m_bEnabled,		sizeof( m_bEnabled ) );
	SaveRegistryInfo( "radiant_ZClipTop",     &m_iZClipTop,		sizeof( m_iZClipTop ) );
	SaveRegistryInfo( "radiant_ZClipBottom",  &m_iZClipBottom,	sizeof( m_iZClipBottom ) );
}

void CZClip::Reset()
{
	m_iZClipTop		= 64;		// arb. starting values, but must be at least 64 apart
	m_iZClipBottom	= -64;
	m_bEnabled		= false;

	Legalise();
}


int	CZClip::GetTop()
{
	return m_iZClipTop;
}

int CZClip::GetBottom()
{
	return m_iZClipBottom;
}

void CZClip::Legalise()
{
	// need swapping?
	//
	if( m_iZClipTop < m_iZClipBottom )
	{
		int iTemp = m_iZClipTop;
		m_iZClipTop = m_iZClipBottom;
		m_iZClipBottom = iTemp;
	}

	// too close together?
	//
#define ZCLIP_MIN_SPACING 64

	if( abs( m_iZClipTop - m_iZClipBottom ) < ZCLIP_MIN_SPACING )
	{
		m_iZClipBottom = m_iZClipTop - ZCLIP_MIN_SPACING;
	}
}


void CZClip::SetTop( int iNewZ )
{
	m_iZClipTop = iNewZ;

	Legalise();
}

void CZClip::SetBottom( int iNewZ )
{
	m_iZClipBottom = iNewZ;

	Legalise();
}

bool CZClip::IsEnabled()
{
	return m_bEnabled;
}


bool CZClip::Enable( bool bOnOff )
{
	m_bEnabled = !m_bEnabled;
	return IsEnabled();
}

#define ZCLIP_BAR_THICKNESS 8
#define ZCLIP_ARROWHEIGHT (ZCLIP_BAR_THICKNESS*8)

void CZClip::Paint()
{
	float	x, y;
	int	xCam = z.width / 4;	// hmmm, a rather unpleasant and obscure global name, but it was already called that so...

	glColor3f( ZCLIP_COLOUR ); //1.0, 0.0, 1.0);

	// draw TOP marker...
	//
	x = 0;
	y = m_iZClipTop;

	if( m_bEnabled )
	{
		glBegin( GL_QUADS );
	}
	else
	{
		glBegin( GL_LINE_LOOP );
	}

	glVertex3f( x - xCam, y, 0 );
	glVertex3f( x - xCam, y + ZCLIP_BAR_THICKNESS, 0 );
	glVertex3f( x + xCam, y + ZCLIP_BAR_THICKNESS, 0 );
	glVertex3f( x + xCam, y, 0 );
	glEnd();

	glColor3f( ZCLIP_COLOUR_DIM ); //0.8, 0.0, 0.8);

	if( m_bEnabled )
	{
		glBegin( GL_TRIANGLES );
	}
	else
	{
		glBegin( GL_LINE_LOOP );
	}
	glVertex3f( x, ( y + ZCLIP_BAR_THICKNESS ), 0 );
	glVertex3f( x - xCam, ( y + ZCLIP_BAR_THICKNESS ) + ( ZCLIP_ARROWHEIGHT / 2 ), 0 );
	glVertex3f( x + xCam, ( y + ZCLIP_BAR_THICKNESS ) + ( ZCLIP_ARROWHEIGHT / 2 ), 0 );
	glEnd();

	// draw bottom marker...
	//
	glColor3f( ZCLIP_COLOUR ); //1.0, 0.0, 1.0);
	x = 0;
	y = m_iZClipBottom;

	if( m_bEnabled )
	{
		glBegin( GL_QUADS );
	}
	else
	{
		glBegin( GL_LINE_LOOP );
	}
	glVertex3f( x - xCam, y, 0 );
	glVertex3f( x - xCam, y - ZCLIP_BAR_THICKNESS, 0 );
	glVertex3f( x + xCam, y - ZCLIP_BAR_THICKNESS, 0 );
	glVertex3f( x + xCam, y, 0 );
	glEnd();

	glColor3f( ZCLIP_COLOUR_DIM ); //0.8, 0.0, 0.8);

	if( m_bEnabled )
	{
		glBegin( GL_TRIANGLES );
	}
	else
	{
		glBegin( GL_LINE_LOOP );
	}
	glVertex3f( x, ( y - ZCLIP_BAR_THICKNESS ), 0 );
	glVertex3f( x - xCam, ( y - ZCLIP_BAR_THICKNESS ) - ( ZCLIP_ARROWHEIGHT / 2 ), 0 );
	glVertex3f( x + xCam, ( y - ZCLIP_BAR_THICKNESS ) - ( ZCLIP_ARROWHEIGHT / 2 ), 0 );
	glEnd();
}


///////////////// eof ///////////////////
