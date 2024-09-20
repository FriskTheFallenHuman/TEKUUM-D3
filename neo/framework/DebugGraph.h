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
#ifndef __DEBUGGRAPH_H__
#define __DEBUGGRAPH_H__

/*
================================================================================================
Contains the DebugGraph declaration.
================================================================================================
*/

/*
================================================
The *Debug Graph, idDebugGraph, contains graphing functionality common to many debug tools.
================================================
*/
class idDebugGraph
{
public:
	idDebugGraph( int numItems = 0 );

	void	Enable( bool b )
	{
		enable = b;
	}

	// create a graph with the specified number of bars
	void	Init( int numBars );

	void	AddGridLine( float value, const idVec4& color );

	// sets a bar value, pass -1 to append an element
	void	SetValue( int b, float value, const idVec4& color );
	float	GetValue( int b )
	{
		return bars[b].value;
	}

	// sets a bar label
	void	SetLabel( int b, const char* text );

	enum fillMode_t
	{
		GRAPH_LINE,				// only draw a single top line for each bar
		GRAPH_FILL,				// fill the entire bar from the bottom (or left)
		GRAPH_FILL_REVERSE,		// fill the entire bar from the top (or right)
	};
	void	SetFillMode( fillMode_t m )
	{
		mode = m;
	}

	// render the graph sideways?
	void	SetSideways( bool s )
	{
		sideways = s;
	}

	// the background color is what's drawn between bars and in the empty space
	void	SetBackgroundColor( const idVec4& color )
	{
		bgColor = color;
	}
	void	SetLabelColor( const idVec4& color )
	{
		fontColor = color;
	}

	// the border specifies the amount of space between bars as well as the amount of space around the entire graph
	void	SetBorder( float b )
	{
		border = b;
	}

	// set the screen position for the graph
	void	SetPosition( float x, float y, float w, float h )
	{
		position.Set( x, y, w, h );
	}

	void	Render( idRenderSystem* gui );

private:
	const class idMaterial* white;
	const class idMaterial* font;

	idVec4	bgColor;
	idVec4	fontColor;
	fillMode_t mode;
	bool	sideways;
	float	border;
	idVec4	position;
	bool	enable;

	struct graphPlot_t
	{
		float	value;
		idVec4	color;
	};
	idList< graphPlot_t >	bars;
	idList< graphPlot_t >	grid;
	idList< idStr >			labels;
};

#endif
