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

#ifndef __NETWORKSYSTEM_H__
#define __NETWORKSYSTEM_H__


/*
===============================================================================

  Network System.

===============================================================================
*/

class idNetworkSystem
{
public:
	virtual					~idNetworkSystem() {}

	virtual void			ServerSendReliableMessage( int clientNum, const idBitMsg& msg );
	virtual void			ServerSendReliableMessageExcluding( int clientNum, const idBitMsg& msg );
	virtual int				ServerGetClientPing( int clientNum );
	virtual int				ServerGetClientPrediction( int clientNum );
	virtual int				ServerGetClientTimeSinceLastPacket( int clientNum );
	virtual int				ServerGetClientTimeSinceLastInput( int clientNum );
	virtual int				ServerGetClientOutgoingRate( int clientNum );
	virtual int				ServerGetClientIncomingRate( int clientNum );
	virtual float			ServerGetClientIncomingPacketLoss( int clientNum );

	virtual void			ClientSendReliableMessage( const idBitMsg& msg );
	virtual int				ClientGetPrediction();
	virtual int				ClientGetTimeSinceLastPacket();
	virtual int				ClientGetOutgoingRate();
	virtual int				ClientGetIncomingRate();
	virtual float			ClientGetIncomingPacketLoss();
};

extern idNetworkSystem* 	networkSystem;

#endif /* !__NETWORKSYSTEM_H__ */
