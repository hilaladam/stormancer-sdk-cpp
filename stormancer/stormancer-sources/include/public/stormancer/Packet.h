#pragma once

#include "stormancer/BuildConfig.h"

#include "stormancer/IConnection.h"
#include "stormancer/IScenePeer.h"
#include "stormancer/Serializer.h"
#include "rxcpp/rx.hpp"

namespace Stormancer
{
	/// Expose a stream for reading data received from the network.
	template<typename T = IConnection>
	class Packet
	{
	public:

#pragma region public_methods

		/// Constructor
		/// \param source Generic source of the packets.
		Packet(std::shared_ptr<T> source, const std::unordered_map<std::string, std::string>& metadata = std::unordered_map<std::string, std::string>())
			: connection(source)
			, metadata(metadata)
		{
		}

		/// Constructor
		/// \param source Generic source of the packets.
		/// \param stream Data stream attached to the packet.
		/// \param metadata Metadata attached to this packet.
		Packet(std::shared_ptr<T> source, byte* data, std::streamsize dataSize, const std::unordered_map<std::string, std::string>& metadata = std::unordered_map<std::string, std::string>())
			: connection(source)
			, stream(data, dataSize)
			, metadata(metadata)
		{
		}

		/// Destructor
		virtual ~Packet()
		{
		}

		/// Read a serialized object
		template<typename TOut>
		TOut readObject()
		{
			Serializer serializer;
			return serializer.deserializeOne<TOut>(stream);
		}

		/// Read many serialized objects
		template<typename... Args>
		void readObjects(Args&... args)
		{
			Serializer serializer;
			serializer.deserialize(stream, args...);
		}

#pragma endregion

#pragma region public_members

		/// Source
		std::shared_ptr<T> connection;

		/// Data stream
		ibytestream stream;

		/// Metadata
		std::unordered_map<std::string, std::string> metadata;

		/// Original packet
		/// Used to keep the original packet alive (in case of async process)
		std::shared_ptr<Packet<IConnection>> originalPacket;

#pragma endregion

	private:

#pragma region private_methods

		/// Copy constructor deleted.
		Packet(const Packet<T>&) = delete;

		/// Move constructor deleted.
		Packet(Packet<T>&&) = delete;

		/// Copy operator deleted.
		Packet<T>& operator=(const Packet<T>&) = delete;

#pragma endregion
	};

	using Packet_ptr = std::shared_ptr<Packet<IConnection>>;
	using Packetisp_ptr = std::shared_ptr<Packet<IScenePeer>>;
	using PacketObservable = rxcpp::observable<Packet_ptr>;
	using HandlerFunction = std::function<bool(Packet_ptr)>;
	using ProcessorFunction = std::function<bool(byte, Packet_ptr)>;
}
