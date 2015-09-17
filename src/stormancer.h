#pragma once

#define Packet_ptr std::shared_ptr<Packet<>>
#define Packetisp_ptr std::shared_ptr<Packet<IScenePeer>>
#define Request_ptr std::shared_ptr<Request>
#define Route_ptr std::shared_ptr<Route>
#define Scene_ptr std::shared_ptr<Scene>
#define Subscription_ptr std::shared_ptr<Subscription>

#define PacketObservable rxcpp::observable<Packet_ptr>
#define PacketObserver rxcpp::observer<Packet_ptr>

#define handlerFunction std::function<bool(Packet_ptr)>
#define processorFunction std::function<bool(byte, Packet_ptr)>

#include "headers.h"

#include "basic_bytebuf.h"
#include "basic_bytestream.h"

#include "Action.h"

#include "ConnectionData.h"
#include "ConnectToSceneMsg.h"
#include "ConnectionResult.h"
#include "DisconnectFromSceneDto.h"
#include "EmptyDto.h"
#include "RouteDto.h"
#include "SceneInfosDto.h"
#include "SceneInfosRequestDto.h"

#include "Helpers.h"
#include "ApiClient.h"
#include "Client.h"
#include "Configuration.h"
#include "ConnectionState.h"
#include "ILogger.h"
#include "NullLogger.h"
#include "FileLogger.h"
#include "DefaultPacketDispatcher.h"
#include "IConnection.h"
#include "IConnectionManager.h"
#include "IPacketProcessor.h"
#include "IRequest.h"
#include "IScenePeer.h"
#include "ITransport.h"
#include "MessageIDTypes.h"
#include "SystemRequestIDTypes.h"
#include "Packet.h"
#include "PacketProcessorConfig.h"
#include "RequestContext.h"
#include "RequestModuleBuilder.h"
#include "RequestProcessor.h"
#include "Route.h"
#include "Scene.h"
#include "SceneDispatcher.h"
#include "SceneEndpoint.h"
#include "ScenePeer.h"
#include "IPacketDispatcher.h"
#include "IRequestModule.h"
#include "ITokenHandler.h"
#include "TokenHandler.h"
#include "Subscription.h"
#include "IScheduler.h"
#include "DefaultScheduler.h"

#include "RakNetConnection.h"
#include "RakNetTransport.h"
