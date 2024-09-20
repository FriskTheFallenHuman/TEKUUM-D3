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

#define	MAX_POINTFILE	8192
static idVec3	s_pointvecs[MAX_POINTFILE];
static int		s_num_points = 0;
static int		s_check_point = 0;

void Pointfile_Delete()
{
	char	name[1024];

	strcpy( name, currentmap );
	StripExtension( name );
	strcat( name, ".lin" );

	remove( name );
}

// advance camera to next point
void Pointfile_Next()
{
	if( s_check_point >= s_num_points - 2 )
	{
		Sys_Status( "End of pointfile", 0 );
		return;
	}

	s_check_point++;

	g_pParentWnd->GetCamera()->Camera().origin = s_pointvecs[s_check_point];
	g_pParentWnd->GetXYWnd()->GetOrigin() = s_pointvecs[s_check_point];

	idVec3 dir = s_pointvecs[s_check_point + 1] - g_pParentWnd->GetCamera()->Camera().origin;
	dir.Normalize();
	g_pParentWnd->GetCamera()->Camera().angles[1] = atan2( dir[1], dir[0] ) * 180 / 3.14159;
	g_pParentWnd->GetCamera()->Camera().angles[0] = asin( dir[2] ) * 180 / 3.14159;

	Sys_UpdateWindows( W_ALL );
}

// advance camera to previous point
void Pointfile_Prev()
{
	if( s_check_point == 0 )
	{
		Sys_Status( "Start of pointfile", 0 );
		return;
	}

	s_check_point--;

	g_pParentWnd->GetCamera()->Camera().origin = s_pointvecs[s_check_point];
	g_pParentWnd->GetXYWnd()->GetOrigin() = s_pointvecs[s_check_point];

	idVec3 dir = s_pointvecs[s_check_point + 1] - g_pParentWnd->GetCamera()->Camera().origin;
	dir.Normalize();
	g_pParentWnd->GetCamera()->Camera().angles[1] = atan2( dir[1], dir[0] ) * 180 / 3.14159;
	g_pParentWnd->GetCamera()->Camera().angles[0] = asin( dir[2] ) * 180 / 3.14159;

	Sys_UpdateWindows( W_ALL );
}

void Pointfile_Check()
{
	char	name[1024];
	FILE*	f;
	idVec3	v;

	strcpy( name, currentmap );
	StripExtension( name );
	strcat( name, ".lin" );

	f = fopen( name, "r" );
	if( !f )
	{
		return;
	}

	common->Printf( "Reading pointfile %s\n", name );

	s_num_points = 0;
	do
	{
		const int n = fscanf( f, "%f %f %f\n", &v[0], &v[1], &v[2] );
		if( n != 3  || s_num_points >= MAX_POINTFILE )
		{
			break;
		}

		s_pointvecs[s_num_points] = v;
		s_num_points++;

	}
	while( 1 );

	s_check_point = 0;
	fclose( f );
}

void Pointfile_Draw()
{
	glColor3f( 1.0F, 0.0F, 0.0F );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_TEXTURE_1D );
	glLineWidth( 2 );
	glBegin( GL_LINE_STRIP );

	for( int i = 0; i < s_num_points; i++ )
	{
		glVertex3fv( s_pointvecs[i].ToFloatPtr() );
	}

	glEnd();
	glLineWidth( 0.5 );
}

void Pointfile_Clear()
{
	s_num_points = 0;
}
