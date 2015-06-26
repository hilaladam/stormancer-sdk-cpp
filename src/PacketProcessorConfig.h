#pragma once
#include "headers.h"
#include "Packet.h"
#include "IPacketProcessor.h"

namespace Stormancer
{
	using handlerFunction = std::function < bool(std::shared_ptr<Packet<>>) >;
	using processorFunction = std::function < bool(byte, std::shared_ptr<Packet<>>) >;

	/// Contains methods to register messages handlers based on message type.
	/// Pass it to the IPacketProcessor::RegisterProcessor method.
	class PacketProcessorConfig
	{
	public:
		PacketProcessorConfig(std::map<byte, handlerFunction*>& handlers, std::vector<processorFunction*>& defaultProcessors);
		virtual ~PacketProcessorConfig();

		void addProcessor(byte msgId, handlerFunction* handler);     
		void addCatchAllProcessor(processorFunction* processor);

	private:
		std::map<byte, handlerFunction*>& _handlers;
		std::vector<processorFunction*>& _defaultProcessors;
	};
};