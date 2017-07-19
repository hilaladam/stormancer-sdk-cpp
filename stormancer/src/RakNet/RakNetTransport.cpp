#include "stdafx.h"
#include "RakNet/RakNetTransport.h"
#include "IScheduler.h"
#include "MessageIDTypes.h"
#include <GetTime.h>

#ifdef STORMANCER_PACKETFILELOGGER
#include <PacketFileLogger.h>
#endif

#if defined(_WIN32)
//

#else
#include <unistd.h>
#endif

namespace Stormancer
{
	RakNetTransport::RakNetTransport(DependencyResolver* resolver)
		: _dependencyResolver(resolver)
		, _logger(resolver->resolve<ILogger>())
		, _scheduler(resolver->resolve<IScheduler>())
	{
	}

	RakNetTransport::~RakNetTransport()
	{
		ILogger::instance()->log(LogLevel::Trace, "RakNetTransport", "Deleting RakNet transport...");

		if (_isRunning)
		{
			stop();
		}

		ILogger::instance()->log(LogLevel::Trace, "RakNetTransport", "RakNet transport deleted");
	}

	void RakNetTransport::start(std::string type, std::shared_ptr<IConnectionManager> handler, pplx::cancellation_token ct, uint16 serverPort, uint16 maxConnections)
	{
		_logger->log(LogLevel::Trace, "RakNetTransport", "Starting RakNet transport...");

		if (!compareExchange(_mutex, _isRunning, false, true))
		{
			throw std::runtime_error("RakNet transport is already started.");
		}

		if (handler == nullptr && maxConnections > 0)
		{
			throw std::invalid_argument("Handler is null for server.");
		}

		_type = type;
		_handler = handler;
		initialize(maxConnections, serverPort);

		_scheduler->schedulePeriodic(15, [this]() {
			std::lock_guard<std::mutex> lock(_mutex);
			if (_isRunning)
			{
				run();
			}
		}, ct);

		ct.register_callback([this]() {
			stop();
		});

		_logger->log(LogLevel::Trace, "RakNetTransport", "RakNet transport started");
	}

	void RakNetTransport::initialize(uint16 maxConnections, uint16 serverPort)
	{
		try
		{
			_logger->log(LogLevel::Trace, "RakNetTransport", "Initializing raknet transport", _type.c_str());

#ifdef STORMANCER_PACKETFILELOGGER
			RakNet::PacketFileLogger *logger = new RakNet::PacketFileLogger();
			logger->StartLog("packetLogs");
#endif

			_peer = std::shared_ptr<RakNet::RakPeerInterface>(RakNet::RakPeerInterface::GetInstance(), [
#ifdef STORMANCER_PACKETFILELOGGER
				logger
#endif
			](RakNet::RakPeerInterface* peer) {
				ILogger::instance()->log(LogLevel::Trace, "RakNetTransport", "Deleting RakPeerInterface...");
				RakNet::RakPeerInterface::DestroyInstance(peer);
#ifdef STORMANCER_PACKETFILELOGGER
				delete logger;
#endif
			});
#ifdef STORMANCER_PACKETFILELOGGER
			_peer->AttachPlugin(logger);
#endif

			_dependencyResolver->registerDependency(_peer);
			if (serverPort != 0)
			{
				_socketDescriptor = std::make_shared<RakNet::SocketDescriptor>(serverPort, (const char*)nullptr);
			}
			else
			{
				_socketDescriptor = std::make_shared<RakNet::SocketDescriptor>();
			}
			_socketDescriptor->socketFamily = AF_INET;
			DataStructures::List<RakNet::SocketDescriptor> socketDescriptorsList;
			socketDescriptorsList.Push(*_socketDescriptor, _FILE_AND_LINE_);
			auto startupResult = _peer->Startup(maxConnections, socketDescriptorsList, 1);
			if (startupResult != RakNet::StartupResult::RAKNET_STARTED)
			{
				throw std::runtime_error(std::string("RakNet peer startup failed (RakNet::StartupResult == ") + std::to_string(startupResult) + ')');
			}
			_peer->SetMaximumIncomingConnections(maxConnections);
			_logger->log(LogLevel::Trace, "RakNetTransport", "Raknet transport initialized", _type.c_str());
		}
		catch (const std::exception& ex)
		{
			throw std::runtime_error(std::string() + "Failed to initialize the RakNet peer (" + ex.what() + ")");
		}
	}

	void RakNetTransport::run()
	{
		try
		{
			RakNet::Packet* rakNetPacket = nullptr;
			while ((rakNetPacket = _peer->Receive()) != nullptr)
			{
				if (rakNetPacket->length == 0)
				{
					assert(false);
					_peer->DeallocatePacket(rakNetPacket);
					continue;
				}

				byte ID = rakNetPacket->data[0];
				//_logger->log(LogLevel::Trace, "RakNetTransport", "RakNet packet received ", std::to_string(ID));
#ifdef STORMANCER_LOG_RAKNET_PACKETS
				std::string receivedData((char*)rakNetPacket->data, rakNetPacket->length);
				auto bytes = stringifyBytesArray(receivedData, true);
				_logger->log(LogLevel::Trace, "RakNetTransport", "RakNet packet received", bytes.c_str());
#endif

				{
					/*std::string packetSystemAddressStr = rakNetPacket->systemAddress.ToString(true, ':');
					if (_serverConnected && !mapContains(_pendingConnections, packetSystemAddressStr))
					{
						_peer->DeallocatePacket(rakNetPacket);
						continue;
					}*/
				}

				try
				{
					switch (ID)
					{
						// RakNet messages types
					case DefaultMessageIDTypes::ID_CONNECTION_REQUEST_ACCEPTED:
					{
						if (!_serverConnected)
						{
							_serverConnected = true;
							_serverRakNetGUID = rakNetPacket->guid;
						}

						std::string packetSystemAddressStr = rakNetPacket->systemAddress.ToString(true, ':');
						if (mapContains(_pendingConnections, packetSystemAddressStr))
						{
							auto tce = _pendingConnections[packetSystemAddressStr];
							_logger->log(LogLevel::Trace, "RakNetTransport", "Connection request accepted", packetSystemAddressStr.c_str());
							auto c = onConnection(rakNetPacket->systemAddress, rakNetPacket->guid);
							tce.set(c);
						}
						else
						{
							_logger->log(LogLevel::Error, "RakNetTransport", "Can't get the pending connection TCE", packetSystemAddressStr.c_str());
						}
						break;
					}
					case DefaultMessageIDTypes::ID_CONNECTION_ATTEMPT_FAILED:
					{
						std::string packetSystemAddressStr = rakNetPacket->systemAddress.ToString(true, ':');
						if (mapContains(_pendingConnections, packetSystemAddressStr))
						{
							auto tce = _pendingConnections[packetSystemAddressStr];
							tce.set_exception<std::exception>(std::runtime_error("Connection attempt failed"));
						}
						break;
					}
					case DefaultMessageIDTypes::ID_ALREADY_CONNECTED:
					{
						_logger->log(LogLevel::Error, "RakNetTransport", "Peer already connected", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_NEW_INCOMING_CONNECTION:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "Incoming connection", rakNetPacket->systemAddress.ToString(true, ':'));
						onConnection(rakNetPacket->systemAddress, rakNetPacket->guid);
						break;
					}
					case DefaultMessageIDTypes::ID_NO_FREE_INCOMING_CONNECTIONS:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "The system we attempted to connect to is not accepting new connections", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_DISCONNECTION_NOTIFICATION:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "Peer disconnected", rakNetPacket->systemAddress.ToString(true, ':'));
						onDisconnection(rakNetPacket, "CLIENT_DISCONNECTED");
						break;
					}
					case DefaultMessageIDTypes::ID_CONNECTION_LOST:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "Peer lost the connection", rakNetPacket->systemAddress.ToString(true, ':'));
						onDisconnection(rakNetPacket, "CLIENT_CONNECTION_LOST");
						break;
					}
					case DefaultMessageIDTypes::ID_CONNECTION_BANNED:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "We are banned from the system we attempted to connect to", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_INVALID_PASSWORD:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "The remote system is using a password and has refused our connection because we did not set the correct password", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_INCOMPATIBLE_PROTOCOL_VERSION:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "RakNetVersion.h does not match on the remote system what we have on our system", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_IP_RECENTLY_CONNECTED:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "this IP address connected recently, and can't connect again as a security measure", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					case DefaultMessageIDTypes::ID_UNCONNECTED_PONG:
					{
						auto address = rakNetPacket->systemAddress.ToString(true, ':');
						auto it = _pendingPings.find(std::string(address));
						if (it != _pendingPings.end())
						{
							it->second.set(0);
						}
						break;
					}
					case DefaultMessageIDTypes::ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
					{
						break;
					}
					case DefaultMessageIDTypes::ID_ADVERTISE_SYSTEM:
					{
						_logger->log(LogLevel::Trace, "RakNetTransport", "Inform a remote system of our IP/Port", rakNetPacket->systemAddress.ToString(true, ':'));
						break;
					}
					// Stormancer messages types
					case (byte)MessageIDTypes::ID_CONNECTION_RESULT:
					{
						int64 sid;
						std::memcpy(&sid, (rakNetPacket->data + 1), sizeof(sid));
						_logger->log(LogLevel::Trace, "RakNetTransport", "Connection ID received", std::to_string(sid));
						onConnectionIdReceived(sid);
						break;
					}
					case DefaultMessageIDTypes::ID_TIMESTAMP:
					case (byte)MessageIDTypes::ID_SYSTEM_REQUEST:
					case (byte)MessageIDTypes::ID_REQUEST_RESPONSE_COMPLETE:
					case (byte)MessageIDTypes::ID_REQUEST_RESPONSE_ERROR:
					case (byte)MessageIDTypes::ID_REQUEST_RESPONSE_MSG:
					case (byte)MessageIDTypes::ID_SCENES:

					default:
					{
						if (ID >= DefaultMessageIDTypes::ID_USER_PACKET_ENUM || (ID >= 58 && ID <= 59))
						{
							onMessageReceived(rakNetPacket);
						}
						else
						{
							_logger->log(LogLevel::Trace, "RakNetTransport", "Unprocessed RakNet message ID", std::to_string(ID));
						}
						rakNetPacket = nullptr;
						break;
					}
					}
					if (rakNetPacket)
					{
						_peer->DeallocatePacket(rakNetPacket);
					}
				}
				catch (const std::exception& ex)
				{
					_logger->log(LogLevel::Error, "RakNetTransport", "An error occured while handling a message", ex.what());
				}
			}
		}
		catch (const std::exception& ex)
		{
			_logger->log(LogLevel::Error, "RakNetTransport", "An error occured while running the transport", ex.what());
		}
	}

	void RakNetTransport::stop()
	{
		_logger->log(LogLevel::Trace, "RakNetTransport", "Stopping RakNet transport...");

		if (!compareExchange(_mutex, _isRunning, true, false))
		{
			throw std::runtime_error("RakNet transport is not started");
		}

		if (_peer)
		{
			if (_peer->IsActive())
			{
				_peer->Shutdown(1000);
				_peer.reset();
			}
		}

		if (_socketDescriptor)
		{
			_socketDescriptor.reset();
		}

		if (_handler)
		{
			_handler.reset();
		}

		_logger->log(LogLevel::Trace, "RakNetTransport", "RakNet transport stopped");
	}

	std::vector<std::string> RakNetTransport::externalAddresses() const
	{
		std::vector<std::string> addrs;
		addrs.push_back(_peer->GetSystemAddressFromGuid(_peer->GetMyGUID()).ToString(true, ':'));

		if (isRunning())
		{
			addrs.push_back(_peer->GetExternalID(_peer->GetSystemAddressFromGuid(_serverRakNetGUID)).ToString(true, ':'));
		}
		return addrs;
	}

	void RakNetTransport::onConnectionIdReceived(uint64 p)
	{
		_id = p;
	}

	pplx::task<std::weak_ptr<IConnection>> RakNetTransport::connect(std::string endpoint)
	{
		std::lock_guard<std::mutex> lock(_mutex);


		auto split = stringSplit(endpoint, ":");

		if (split.size() < 2)
		{
			throw std::invalid_argument("Bad server endpoint, no port (" + endpoint + ')');
		}

		std::string portString = *(split.end() - 1);
		if (endpoint.size() - 1 <= portString.size())
		{
			throw std::invalid_argument("Bad server endpoint, no host (" + endpoint + ')');
		}

		_port = (uint16)std::atoi(portString.c_str());
		if (_port == 0)
		{
			throw std::runtime_error("Server endpoint port should not be 0 (" + endpoint + ')');
		}

		auto hostStr = std::string(endpoint.c_str(), endpoint.size() - portString.size() - 1);

		if (_peer == nullptr || !_peer->IsActive())
		{
			throw std::runtime_error("Transport not started. Make sure you started it.");
		}

		auto result = _peer->Connect(hostStr.c_str(), _port, 0, 0);
		if (result != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
		{
			throw std::runtime_error(std::string("Bad RakNet connection attempt result (") + std::to_string(result) + ')');
		}

		std::string addressStr = RakNet::SystemAddress(hostStr.c_str(), _port).ToString(true, ':');
		auto tce = pplx::task_completion_event<std::weak_ptr<IConnection>>();
		_pendingConnections[addressStr] = tce;
		return pplx::task<std::weak_ptr<IConnection>>(tce);
	}

	std::shared_ptr<RakNetConnection> RakNetTransport::onConnection(RakNet::SystemAddress systemAddress, RakNet::RakNetGUID guid)
	{
		auto msg = std::string() + systemAddress.ToString(true, ':') + " connected";
		_logger->log(LogLevel::Trace, "RakNetTransport", msg.c_str());

		auto connection = createNewConnection(guid, _peer);

		_handler->newConnection(connection);

		connection->sendSystem((byte)MessageIDTypes::ID_CONNECTION_RESULT, [connection](bytestream* stream) {
			int64 sid = connection->id();
			(*stream) << sid;
		});

		pplx::task<void>([connection]() {
			// Start this asynchronously because we locked the mutex in run and the user can do something that tries to lock again this mutex
			connection->setConnectionState(ConnectionState::Connecting); // we should set connecting state sooner (when the user call connect)
			connection->setConnectionState(ConnectionState::Connected);
		});

		return connection;
	}

	void RakNetTransport::onDisconnection(RakNet::Packet* packet, std::string reason)
	{
		auto msg = std::string() + packet->systemAddress.ToString(true, ':') + " disconnected";
		_logger->log(LogLevel::Trace, "RakNetTransport", msg.c_str(), reason.c_str());

		auto connection = removeConnection(packet->guid);

		_handler->closeConnection(connection, reason);

		connection->_closeAction(reason);

		pplx::task<void>([connection]() {
			// Start this asynchronously because we locked the mutex in run and the user can do something that tries to lock again this mutex
			connection->setConnectionState(ConnectionState::Disconnected);
		});
	}

	void RakNetTransport::onMessageReceived(RakNet::Packet* rakNetPacket)
	{
#if defined(STORMANCER_LOG_PACKETS) && !defined(STORMANCER_LOG_RAKNET_PACKETS)
		std::string receivedData((char*)rakNetPacket->data, rakNetPacket->length);
		auto bytes = stringifyBytesArray(receivedData, true);
		_logger->log(LogLevel::Trace, "RakNetTransport", "Packet received", bytes.c_str());
#endif

		auto connection = getConnection(rakNetPacket->guid);
		auto stream = new bytestream();
		stream->rdbuf()->pubsetbuf((char*)rakNetPacket->data, rakNetPacket->length);
		Packet_ptr packet(new Packet<>(connection.get(), stream), deleter<Packet<>>());
		auto peer = _peer;
		packet->cleanup += [stream, peer, rakNetPacket]() {
			if (stream)
			{
				stream->rdbuf()->pubsetbuf(nullptr, 0);
				delete stream;
			}
			if (peer && rakNetPacket)
			{
				peer->DeallocatePacket(rakNetPacket);
			}
		};

		_onPacketReceived(packet);
	}

	std::shared_ptr<RakNetConnection> RakNetTransport::getConnection(RakNet::RakNetGUID guid)
	{
		return _connections[guid.g];
	}

	std::shared_ptr<RakNetConnection> RakNetTransport::createNewConnection(RakNet::RakNetGUID raknetGuid, std::weak_ptr<RakNet::RakPeerInterface> peer)
	{
		auto rakPeerInterface = peer.lock();
		if (rakPeerInterface)
		{
			int64 cid = _handler->generateNewConnectionId(rakPeerInterface->GetSystemAddressFromGuid(raknetGuid).ToString(true, ':'));
			auto connection = std::make_shared<RakNetConnection>(raknetGuid, cid, peer);
			connection->onClose([this, connection](std::string reason) {
				_logger->log(LogLevel::Trace, "RakNetTransport", "On close", connection->guid().ToString());
				onRequestClose(connection);
			});
			_connections[raknetGuid.g] = connection;
			return connection;
		}
		return std::shared_ptr<RakNetConnection>();
	}

	std::shared_ptr<RakNetConnection> RakNetTransport::removeConnection(RakNet::RakNetGUID guid)
	{
		auto connection = _connections[guid.g];
		_connections.erase(guid.g);
		return connection;
	}

	void RakNetTransport::onRequestClose(std::shared_ptr<RakNetConnection> connection)
	{
		if (_peer)
		{
			_peer->CloseConnection(connection->guid(), true);
		}
	}

	bool RakNetTransport::isRunning() const
	{
		return _isRunning;
	}

	std::string RakNetTransport::name() const
	{
		return _name;
	}

	uint64 RakNetTransport::id() const
	{
		return _id;
	}

	DependencyResolver* RakNetTransport::dependencyResolver() const
	{
		return _dependencyResolver;
	}

	void RakNetTransport::onPacketReceived(std::function<void(Packet_ptr)> callback)
	{
		_onPacketReceived += callback;
	}

	std::string RakNetTransport::host() const
	{
		return _host.c_str();
	}

	uint16 RakNetTransport::port() const
	{
		return _port;
	}

	pplx::task<int> RakNetTransport::sendPing(const std::string& address)
	{
		pplx::task_completion_event<uint64> tce;
		auto els = stringSplit(address, ":");

		{
			std::lock_guard<std::mutex> lg(_pendingPingsMutex);
			_pendingPings[address] = tce;
		}
		auto time = RakNet::GetTime();

		auto port = (uint16)std::atoi(els[1].c_str());
		auto r = _peer->Ping(els[0].c_str(), port, false);
		if (!r)
		{
			return pplx::task_from_result(-1);
		}
		/*auto p2 = RakNet::RakPeerInterface::GetInstance();
		p2->Startup(10, new RakNet::SocketDescriptor(), 1);
		auto result = p2->Connect(els[0].c_str(), (uint16)std::atoi(els[1].c_str()),nullptr,0);*/

		//_peer->Connect("127.0.0.1", port, nullptr, 0);
		pplx::task<uint64> event_set(tce);
		pplx::cancellation_token_source cts;
		auto token = cts.get_token();

		return cancel_after_timeout(event_set, cts, 2000).then([this, address, time](pplx::task<uint64> t) {
			std::lock_guard<std::mutex> lg(_pendingPingsMutex);
			_pendingPings.erase(address);//destroys the tce and cancels the task
			try
			{
				t.get();
			}
			catch (...) {
				return (int)-1;
			}
			return (int)(RakNet::GetTime() - time);
		});
	}

	void RakNetTransport::openNat(const std::string& address)
	{
		auto els = stringSplit(address, ":");
		this->_peer->SendTTL(els[0].c_str(), (uint16)std::stoi(els[1]), 3);
	}

	std::vector<std::string> RakNetTransport::getAvailableEndpoints() const
	{
		auto nbIps = this->_peer->GetNumberOfAddresses();
		std::vector<std::string> ips;
		for (unsigned int i = 0; i < nbIps; i++)
		{
			auto boundAddress = _peer->GetMyBoundAddress(0);
			auto ip = std::string(_peer->GetLocalIP(i));
			if (is_ipv4_address(ip))
			{
				ips.push_back(ip + ":" + std::to_string(boundAddress.GetPort()));
			}
		}

		return ips;
	}
};