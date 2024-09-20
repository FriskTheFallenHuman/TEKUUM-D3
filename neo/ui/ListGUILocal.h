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

#ifndef __LISTGUILOCAL_H__
#define __LISTGUILOCAL_H__

/*
===============================================================================

	feed data to a listDef
	each item has an id and a display string

===============================================================================
*/

class idListGUILocal : protected idList<idStr>, public idListGUI
{
public:
	idListGUILocal()
	{
		m_pGUI = NULL;
		m_water = 0;
		m_stateUpdates = true;
	}

	// idListGUI interface
	void				Config( idUserInterface* pGUI, const char* name )
	{
		m_pGUI = pGUI;
		m_name = name;
	}
	void				Add( int id, const idStr& s );
	// use the element count as index for the ids
	void				Push( const idStr& s );
	bool				Del( int id );
	void				Clear();
	int					Num()
	{
		return idList<idStr>::Num();
	}
	int					GetSelection( char* s, int size, int sel = 0 ) const; // returns the id, not the list index (or -1)
	void				SetSelection( int sel );
	int					GetNumSelections();
	bool				IsConfigured() const;
	void				SetStateChanges( bool enable );
	void				Shutdown();

private:
	idUserInterface* 	m_pGUI;
	idStr				m_name;
	int					m_water;
	idList<int>			m_ids;
	bool				m_stateUpdates;

	void				StateChanged();
};

#endif /* !__LISTGUILOCAL_H__ */
