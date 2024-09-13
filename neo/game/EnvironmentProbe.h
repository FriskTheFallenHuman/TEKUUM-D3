/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __GAME_ENVIRONMENTPROBE_H__
#define __GAME_ENVIRONMENTPROBE_H__

/*
===============================================================================

  Environment probe for Image Based Lighting (part of PBR).

===============================================================================
*/

class idEnvProbes : public idEntity
{
public:
	CLASS_PROTOTYPE( idEnvProbes );

	idEnvProbes();
	~idEnvProbes();

	void			Spawn();

	void			Save( idSaveGame* savefile ) const;					// archives object for save game file
	void			Restore( idRestoreGame* savefile );					// unarchives object from save game file

	virtual void	UpdateChangeableSpawnArgs( const idDict* source );
	virtual void	Think();
	virtual void	FreeEnvprobeDef();
	void			Present();

	void			SaveState( idDict* args );

	qhandle_t		GetEnvprobeDefHandle() const
	{
		return envprobeDefHandle;
	}

	virtual void	ShowEditingDialog();

	virtual void	ClientPredictionThink();
	virtual void	WriteToSnapshot( idBitMsgDelta& msg ) const;
	virtual void	ReadFromSnapshot( const idBitMsgDelta& msg );

private:
	renderEnvironmentProbe_t	renderEnvprobe;		// envprobe presented to the renderer
	idVec3			localEnvprobeOrigin;			// light origin relative to the physics origin
	idMat3			localEnvprobeAxis;				// light axis relative to physics axis
	qhandle_t		envprobeDefHandle;				// handle to renderer light def

private:
	void			PresentEnvprobeDefChange();

	void			Event_Hide();
	void			Event_Show();
};

#endif /* !__GAME_ENVIRONMENTPROBE_H__ */
