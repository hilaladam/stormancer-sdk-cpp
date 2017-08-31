#include "stdafx.h"
#include "P2P/RelayConnection.h"
#include "MessageIDTypes.h"

namespace Stormancer
{
	RelayConnection::RelayConnection(std::shared_ptr<IConnection> serverConnection, std::string address, uint64 id)
		: _serverConnection(serverConnection)
		, _id(id)
		, _ipAddress(address)
	{
	}

	void RelayConnection::sendSystem(byte msgId, std::function<void(bytestream*)> writer, PacketPriority priority)
	{
		sendRaw(msgId, [writer](bytestream* stream) {
			writer(stream);
		}, priority, PacketReliability::RELIABLE_ORDERED);
	}

	void RelayConnection::sendToScene(byte sceneIndex, uint16 route, std::function<void(bytestream*)> writer, PacketPriority priority, PacketReliability reliability)
	{
		sendRaw(sceneIndex, [writer, route](bytestream* stream) {
			(*stream) << route;
			writer(stream);
		}, priority, reliability);
	}

	void RelayConnection::sendRaw(byte msgId, std::function<void(bytestream*)> writer, PacketPriority priority, PacketReliability reliability)
	{
		_serverConnection->sendRaw((byte)MessageIDTypes::ID_P2P_RELAY, [this, msgId, writer, reliability, priority](bytestream* s) {
			(*s) << this->id();
			(*s) << priority;
			(*s) << reliability;
			(*s) << msgId;
			writer(s);

		}, priority, reliability);
	}

	void RelayConnection::setApplication(std::string account, std::string application)
	{
		throw std::runtime_error("Not implemented");
	}

	void RelayConnection::close(std::string reason)
	{
	}

	std::string RelayConnection::ipAddress() const
	{
		return _ipAddress;
	}

	int RelayConnection::ping() const
	{
		return _serverConnection->ping();
	}

	uint64 RelayConnection::id() const
	{
		return _id;
	}

	time_t RelayConnection::connectionDate() const
	{
		return _connectionDate;
	}

	const std::string & RelayConnection::account() const
	{
		return _serverConnection->account();
	}

	const std::string & RelayConnection::application() const
	{
		return _serverConnection->application();
	}

	const std::map<std::string, std::string>& RelayConnection::metadata() const
	{
		return _metadata;
	}

	std::string RelayConnection::metadata(const std::string & key) const
	{
		auto it = _metadata.find(key);
		if (it != _metadata.end())
		{
			return (*it).second;
		}
		else
		{
			return std::string();
		}
	}

	void RelayConnection::setMetadata(const std::map<std::string, std::string>& metadata)
	{
		_metadata = metadata;
	}

	void RelayConnection::setMetadata(const std::string & key, const std::string & value)
	{
		_metadata[key] = value;
	}

	DependencyResolver* RelayConnection::dependencyResolver()
	{
		return _dependencyResolver.get();
	}

	ConnectionState RelayConnection::getConnectionState() const
	{
		return _serverConnection->getConnectionState();
	}

	rxcpp::observable<ConnectionState> RelayConnection::getConnectionStateChangedObservable() const
	{
		return _serverConnection->getConnectionStateChangedObservable();
	}

	void RelayConnection::setConnectionState(ConnectionState)
	{
		throw std::runtime_error("Not supported");
	}

	Action<std::string>::TIterator RelayConnection::onClose(std::function<void(std::string)> callback)
	{
		throw std::runtime_error("Not supported");
	}

	Action<std::string>& RelayConnection::onCloseAction()
	{
		throw std::runtime_error("Not supported");
	}
}
