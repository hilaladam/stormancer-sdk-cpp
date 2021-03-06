#pragma once

#include "stormancer/Event.h"
#include "GameSession/GameSessionModels.h"
#include "GameSession/GameSession.h"
#include "stormancer/ConnectionState.h"
#include "stormancer/Logger/ILogger.h"

namespace Stormancer
{
	//Forward declare
	class IClient;
	class GameSessionContainer;

	class GameSession_Impl : public std::enable_shared_from_this<GameSession_Impl>, public GameSession
	{
	public:

		GameSession_Impl(std::weak_ptr<IClient> client, std::shared_ptr<ILogger> logger);

		pplx::task<GameSessionConnectionParameters> connectToGameSession(std::string token, std::string mapName = "", bool openTunnel = true, pplx::cancellation_token ct = pplx::cancellation_token::none()) override;
		pplx::task<void> setPlayerReady(const std::string& data, pplx::cancellation_token ct = pplx::cancellation_token::none()) override;
		pplx::task<Packetisp_ptr> postResult(const StreamWriter& streamWriter, pplx::cancellation_token ct = pplx::cancellation_token::none()) override;
		pplx::task<std::string> getUserFromBearerToken(const std::string& token) override;
		pplx::task<void> disconectFromGameSession() override;

		Subscription subscibeOnAllPlayerReady(std::function<void()> callback) override;
		Subscription subscibeOnRoleRecieved(std::function<void(GameSessionConnectionParameters)> callback) override;
		Subscription subscibeOnTunnelOpened(std::function<void(GameSessionConnectionParameters)> callback) override;
		Subscription subscribeOnGameSessionConnectionChange(std::function<void(ConnectionState)> callback) override;
		Subscription subscribeOnShutdownRecieved(std::function<void()> callback) override;
		Subscription subscribeOnPlayerChanged(std::function<void(SessionPlayer, std::string)> callback) override;

		std::shared_ptr<Scene> scene() override;

		std::shared_ptr<IP2PScenePeer>	getSessionHost() const override;
		bool							isSessionHost() const override;

	private:

		//Events
		Event<void> _onAllPlayerReady;
		Event<GameSessionConnectionParameters> _onRoleRecieved;
		Event<GameSessionConnectionParameters> _onTunnelOpened;
		Event<ConnectionState> _onGameSessionConnectionChange;
		Event<void> _onShutdownReceived;
		Event<SessionPlayer, std::string> _onPlayerChanged;

		std::string _mapName;
		std::weak_ptr<IClient> _wClient;
		pplx::task<std::shared_ptr<GameSessionContainer>> _currentGameSession;
		std::shared_ptr<ILogger> _logger;

		pplx::task<std::shared_ptr<GameSessionContainer>> connectToGameSessionImpl(std::string token, bool useTunnel, pplx::cancellation_token ct);
		pplx::task<std::shared_ptr<GameSessionContainer>> getCurrentGameSession(pplx::cancellation_token ct = pplx::cancellation_token::none());
		pplx::task<std::string> P2PTokenRequest(std::shared_ptr<GameSessionContainer> gameSessionContainer, pplx::cancellation_token ct);

		pplx::task_completion_event<void> _hostIsReadyTce;

		std::mutex _lock;
	};
}
