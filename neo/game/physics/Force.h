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

#ifndef __FORCE_H__
#define __FORCE_H__

/*
===============================================================================

	Force base class

	A force object applies a force to a physics object.

===============================================================================
*/

class idEntity;
class idPhysics;

class idForce : public idClass
{

public:
	CLASS_PROTOTYPE( idForce );

	idForce();
	virtual				~idForce();
	static void			DeletePhysics( const idPhysics* phys );
	static void			ClearForceList();

public: // common force interface
	// evalulate the force up to the given time
	virtual void		Evaluate( int time );
	// removes any pointers to the physics object
	virtual void		RemovePhysics( const idPhysics* phys );

private:

	static idList<idForce*> forceList;
};

#endif /* !__FORCE_H__ */
