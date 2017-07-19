#include "stdafx.h"
#include "P2P/P2PPacketDispatcher.h"
#include "MessageIDTypes.h"
#include "P2P/RakNet/P2PTunnels.h"

namespace Stormancer
{
	P2PPacketDispatcher::P2PPacketDispatcher(std::shared_ptr<P2PTunnels> tunnels, std::shared_ptr<IConnectionManager> connections)
		: _tunnels(tunnels)
		, _connections(connections)
	{
	}

	void P2PPacketDispatcher::registerProcessor(PacketProcessorConfig& config)
	{
		config.addProcessor((byte)MessageIDTypes::ID_P2P_RELAY, new handlerFunction([this](Packet_ptr p) {
			/*(*s) << this->id();
			(*s) << priority;
			(*s) << reliability;
			(*s) << msgId;*/

			uint64 peerId;
			*p->stream >> peerId;
			auto connection = _connections->getConnection(peerId);
			if (connection)
			{
				p->connection = connection.get();
			}
			return false;
		}));
		config.addProcessor((byte)MessageIDTypes::ID_P2P_TUNNEL, new handlerFunction([this](Packet_ptr p) {
			_tunnels->receiveFrom(p->connection->id(), p->stream);
			return true;
		}));
	}
};