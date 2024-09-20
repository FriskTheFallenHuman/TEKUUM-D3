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

#include "NetworkSystem.h"

idNetworkSystem		networkSystemLocal;
idNetworkSystem* 	networkSystem = &networkSystemLocal;

/*
==================
idNetworkSystem::ServerSendReliableMessage
==================
*/
void idNetworkSystem::ServerSendReliableMessage( int clientNum, const idBitMsg& msg )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		idAsyncNetwork::server.SendReliableGameMessage( clientNum, msg );
	}
}

/*
==================
idNetworkSystem::ServerSendReliableMessageExcluding
==================
*/
void idNetworkSystem::ServerSendReliableMessageExcluding( int clientNum, const idBitMsg& msg )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		idAsyncNetwork::server.SendReliableGameMessageExcluding( clientNum, msg );
	}
}

/*
==================
idNetworkSystem::ServerGetClientPing
==================
*/
int idNetworkSystem::ServerGetClientPing( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientPing( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientPrediction
==================
*/
int idNetworkSystem::ServerGetClientPrediction( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientPrediction( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientTimeSinceLastPacket
==================
*/
int idNetworkSystem::ServerGetClientTimeSinceLastPacket( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientTimeSinceLastPacket( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientTimeSinceLastInput
==================
*/
int idNetworkSystem::ServerGetClientTimeSinceLastInput( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientTimeSinceLastInput( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientOutgoingRate
==================
*/
int idNetworkSystem::ServerGetClientOutgoingRate( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientOutgoingRate( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientIncomingRate
==================
*/
int idNetworkSystem::ServerGetClientIncomingRate( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientIncomingRate( clientNum );
	}
	return 0;
}

/*
==================
idNetworkSystem::ServerGetClientIncomingPacketLoss
==================
*/
float idNetworkSystem::ServerGetClientIncomingPacketLoss( int clientNum )
{
	if( idAsyncNetwork::server.IsActive() )
	{
		return idAsyncNetwork::server.GetClientIncomingPacketLoss( clientNum );
	}
	return 0.0f;
}

/*
==================
idNetworkSystem::ClientSendReliableMessage
==================
*/
void idNetworkSystem::ClientSendReliableMessage( const idBitMsg& msg )
{
	if( idAsyncNetwork::client.IsActive() )
	{
		idAsyncNetwork::client.SendReliableGameMessage( msg );
	}
	else if( idAsyncNetwork::server.IsActive() )
	{
		idAsyncNetwork::server.LocalClientSendReliableMessage( msg );
	}
}

/*
==================
idNetworkSystem::ClientGetPrediction
==================
*/
int idNetworkSystem::ClientGetPrediction()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetPrediction();
	}
	return 0;
}

/*
==================
idNetworkSystem::ClientGetTimeSinceLastPacket
==================
*/
int idNetworkSystem::ClientGetTimeSinceLastPacket()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetTimeSinceLastPacket();
	}
	return 0;
}

/*
==================
idNetworkSystem::ClientGetOutgoingRate
==================
*/
int idNetworkSystem::ClientGetOutgoingRate()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetOutgoingRate();
	}
	return 0;
}

/*
==================
idNetworkSystem::ClientGetIncomingRate
==================
*/
int idNetworkSystem::ClientGetIncomingRate()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetIncomingRate();
	}
	return 0;
}

/*
==================
idNetworkSystem::ClientGetIncomingPacketLoss
==================
*/
float idNetworkSystem::ClientGetIncomingPacketLoss()
{
	if( idAsyncNetwork::client.IsActive() )
	{
		return idAsyncNetwork::client.GetIncomingPacketLoss();
	}
	return 0.0f;
}
