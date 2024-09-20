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

#ifndef	__PLAYERICON_H__
#define	__PLAYERICON_H__

typedef enum
{
	ICON_LAG,
	ICON_CHAT,
	ICON_NONE
} playerIconType_t;

class idPlayerIcon
{
public:

public:
	idPlayerIcon();
	~idPlayerIcon();

	void	Draw( idPlayer* player, jointHandle_t joint );
	void	Draw( idPlayer* player, const idVec3& origin );

public:
	playerIconType_t	iconType;
	renderEntity_t		renderEnt;
	qhandle_t			iconHandle;

public:
	void	FreeIcon();
	bool	CreateIcon( idPlayer* player, playerIconType_t type, const char* mtr, const idVec3& origin, const idMat3& axis );
	bool	CreateIcon( idPlayer* player, playerIconType_t type, const idVec3& origin, const idMat3& axis );
	void	UpdateIcon( idPlayer* player, const idVec3& origin, const idMat3& axis );

};


#endif	/* !_PLAYERICON_H_ */
