#include <Logger/ConsoleLogger.h>
#include "StormancerWrapper.h"
#include "SingleServicePlugin.h"
#include "Authentication/AuthenticationPlugin.h"
#include "BugReport/BugReportPlugin.h"
#include "GameSession/GameSessionPlugin.h"
#include "GameVersion/GameVersionPlugin.h"
#include "Matchmaking/MatchmakingPlugin.h"
#include "PlayerProfile/PlayerProfilePlugin.h"
#include "PlayerProfile/PlayerProfile.h"
#include "Leaderboard/LeaderboardPlugin.h"
#include "Leaderboard/LeaderboardService.h"

namespace Stormancer
{
	std::shared_ptr<StormancerWrapper> StormancerWrapper::_instance;

	StormancerWrapper::StormancerWrapper()
		: _actionDispatcher(std::make_shared<MainThreadActionDispatcher>())
	{
	}

	StormancerWrapper::~StormancerWrapper()
	{
		shutdown();
	}

	void StormancerWrapper::init(const std::string& endpoint, const std::string& account, const std::string& application)
	{
		_endpoint = endpoint;
		_account = account;
		_application = application;
		_config = Configuration::create(_endpoint, _account, _application);
		_config->logger = _logger;
		_config->endpointSelectionMode = EndpointSelectionMode::FALLBACK;
		_config->actionDispatcher = _actionDispatcher;
		_config->addPlugin(new AuthenticationPlugin());
		_config->addPlugin(new BugReportPlugin());
		_config->addPlugin(new GameSessionPlugin());
		_config->addPlugin(new GameVersionPlugin());
		_config->addPlugin(new LeaderboardPlugin());
		_config->addPlugin(new MatchmakingPlugin());

		ServiceOptions playerProfileServiceOptions;
		playerProfileServiceOptions.contextFlags = (ServiceContextFlags::Scene & ServiceContextFlags::CreateWithScene & ServiceContextFlags::SingleInstance);
		playerProfileServiceOptions.metadataKey = "profiles";
		_config->addPlugin(new SingleServicePlugin<PlayerProfileService<PlayerProfile>>(playerProfileServiceOptions));

		_client = Client::create(_config);
	}

	void StormancerWrapper::update()
	{
		if (_actionDispatcher)
		{
			_actionDispatcher->update(std::chrono::milliseconds(10));
		}
	}

	void StormancerWrapper::shutdown()
	{
		if (_client && _client->getConnectionState() == ConnectionState::Connected)
		{
			_client->disconnect().wait();
			_authService.reset();
			_scenes.clear();
			_actionDispatcher.reset();
			_client.reset();
			_config.reset();
		}
	}

	pplx::task<void> StormancerWrapper::Authenticate(const std::string& provider, const std::string& authenticationToken)
	{
		if (!_authService)
		{
			_authService = _client->dependencyResolver()->resolve<AuthenticationService>();
		}

		const std::map<std::string, std::string> authenticationContext{
			{ "provider", provider },
			{ "ticket", authenticationToken }
		};

		return _authService->login(authenticationContext);
	}

	std::shared_ptr<IActionDispatcher> StormancerWrapper::getDispatcher()
	{
		return _client->dependencyResolver()->resolve<IActionDispatcher>();
	}

	void StormancerWrapper::setLogger(std::shared_ptr<ILogger> logger)
	{
		_logger = logger;
	}

	ILogger_ptr StormancerWrapper::getLogger()
	{
		return _logger;
	}

	pplx::task<void> StormancerWrapper::lauchMatchmaking(MatchType type)
	{
		std::string matchmakingScene;

		switch (type)
		{
			case Fast:
				matchmakingScene = "fast-match";
				break;
			case Ranked:
				matchmakingScene = "ranked";
				break;
			default:
				throw std::runtime_error("invalid match type");
				break;
		}

		return getService<MatchmakingService>(matchmakingScene).then([this](std::shared_ptr<MatchmakingService> matchmakingService)
		{
			matchmakingService->onMatchFound(_onMatchFound);
			matchmakingService->onMatchUpdate(_onMatchUpdate);
			return matchmakingService->findMatch("game");
		});
	}

	pplx::task<void> StormancerWrapper::cancelMatchmaking(MatchType type)
	{
		std::string matchmakingScene;

		switch (type)
		{
			case Fast:
				matchmakingScene = "fast-match";
				break;
			case Ranked:
				matchmakingScene = "ranked";
				break;
			default:
				//throw std::runtime_error("invalid match type"); /* RLE: avoiding crash. */
				break;
		}

		return getService<MatchmakingService>(matchmakingScene).then([this](std::shared_ptr<MatchmakingService> matchmakingService)
		{
			if (matchmakingService)
			{
				matchmakingService->cancel();
			}
		});
	}

	void StormancerWrapper::onMatchFound(const std::function<void(MatchmakingResponse)>& matchFoundCallback)
	{
		_onMatchFound = matchFoundCallback;
	}

	void StormancerWrapper::onMatchUpdate(const std::function<void(MatchState)>& matchUpdateCallback)
	{
		_onMatchUpdate = matchUpdateCallback;
	}

	std::shared_ptr<StormancerWrapper> StormancerWrapper::instance()
	{
		return StormancerWrapper::_instance;
	}

	void StormancerWrapper::setInstance(std::shared_ptr<StormancerWrapper> wrapper)
	{
		StormancerWrapper::_instance = wrapper;
	}
}
