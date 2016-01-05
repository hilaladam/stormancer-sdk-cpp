#pragma once
#include "headers.h"

namespace Stormancer
{
	class Client;

	class Scene;

	class IPlugin
	{
	public:
		STORMANCER_DLL_API virtual ~IPlugin();

		/// Event fired when a client object is created.
		STORMANCER_DLL_API virtual void clientCreated(Client* client);

		/// Event fired when a scene object is created.
		STORMANCER_DLL_API virtual void sceneCreated(Scene* scene);

		/// Event fired when a scene is connecting to the server.
		STORMANCER_DLL_API virtual void sceneConnecting(Scene* scene);

		/// Event fired when a scene is connected to the server.
		STORMANCER_DLL_API virtual void sceneConnected(Scene* scene);

		/// Event fired when a scene is disconnected.
		STORMANCER_DLL_API virtual void sceneDisconnected(Scene* scene);

		/// Event fired when a packet is received from a remote peer.
		STORMANCER_DLL_API virtual void packetReceived(Packet_ptr packet);

		/// Event fired when a packet is received from a remote peer.
		STORMANCER_DLL_API virtual void destroy() = 0;
	};
};