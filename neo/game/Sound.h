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

#ifndef __GAME_SOUND_H__
#define __GAME_SOUND_H__

/*
===============================================================================

  Generic sound emitter.

===============================================================================
*/

class idSound : public idEntity
{
public:
	CLASS_PROTOTYPE( idSound );

	idSound();

	void			Save( idSaveGame* savefile ) const;
	void			Restore( idRestoreGame* savefile );

	virtual void	UpdateChangeableSpawnArgs( const idDict* source );

	void			Spawn();

	void			ToggleOnOff( idEntity* other, idEntity* activator );
	void			Think();
	void			SetSound( const char* sound, int channel = SND_CHANNEL_ANY );

	virtual void	ShowEditingDialog();

private:
	float			lastSoundVol;
	float			soundVol;
	float			random;
	float			wait;
	bool			timerOn;
	idVec3			shakeTranslate;
	idAngles		shakeRotate;
	int				playingUntilTime;

	void			Event_Trigger( idEntity* activator );
	void			Event_Timer();
	void			Event_On();
	void			Event_Off();
	void			DoSound( bool play );
};

#endif /* !__GAME_SOUND_H__ */
