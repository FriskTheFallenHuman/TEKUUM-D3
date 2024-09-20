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
