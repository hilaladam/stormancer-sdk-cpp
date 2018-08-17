#include "GameFinderService.h"

namespace Stormancer
{
	Internal::ReadyVerificationRequest::operator Stormancer::ReadyVerificationRequest()
	{
		Stormancer::ReadyVerificationRequest readyUpdate;
		readyUpdate.matchId = matchId;
		readyUpdate.timeout = timeout;
		readyUpdate.membersCountTotal = (int32)members.size();
		readyUpdate.membersCountReady = 0;
		for (auto it : members)
		{
			readyUpdate.members[it.first] = (Readiness)it.second;
		}
		return readyUpdate;
	}

	GameFinderService::GameFinderService(Scene_ptr scene)
		: _scene(scene)
		, _rpcService(scene->dependencyResolver().lock()->resolve<RpcService>())
	{

		
	}

	GameFinderService::~GameFinderService()
	{
	}
	void GameFinderService::initialize()
	{
		std::weak_ptr<GameFinderService> wThat = this->shared_from_this();
		_scene.lock()->addRoute("match.update", [wThat](Packetisp_ptr packet) {
			byte matchStateByte;
			packet->stream->read((char*)&matchStateByte, 1);
			int32 matchState = matchStateByte;

			if (auto that = wThat.lock())
			{
				that->_currentState = (GameFinderStatus)matchState;

				auto ms = std::to_string(matchState);

				that->GameFinderStatusUpdated(that->_currentState);

				if (that->_currentState == GameFinderStatus::Success)
				{
					Serializer serializer;
					auto dto = serializer.deserializeOne<GameFinderResponseDto>(packet->stream);
					
					GameFinderResponse response;
					response.connectionToken = dto.gameToken;
					response.optionalParameters = dto.optionalParameters;
					
					that->GameFound(response);
					that->_currentState = GameFinderStatus::Idle;
					that->GameFinderStatusUpdated(that->_currentState);
				}
				else if(that->_currentState == GameFinderStatus::Canceled)
				{
					that->_currentState = GameFinderStatus::Idle;
					that->GameFinderStatusUpdated(that->_currentState);
				}
			}
		});

		_scene.lock()->addRoute("match.parameters.update", [wThat](Packetisp_ptr packet) {
			Serializer serializer;
			
			std::string provider = serializer.deserializeOne<std::string>(packet->stream);

			//_onMatchParametersUpdate();
		});

		_scene.lock()->addRoute("match.ready.update", [wThat](Packetisp_ptr packet) {
			Serializer serializer;
			Internal::ReadyVerificationRequest readyUpdateTmp = serializer.deserializeOne<Internal::ReadyVerificationRequest>(packet->stream);
			ReadyVerificationRequest readyUpdate = readyUpdateTmp;
			readyUpdate.membersCountReady = 0;

			for (auto it : readyUpdateTmp.members)
			{
				Readiness ready = (Readiness)it.second;
				if (ready == Readiness::Ready)
				{
					readyUpdate.membersCountReady++;
				}
			}

			
		});
	}

	GameFinderStatus GameFinderService::currentState() const
	{
		return _currentState;
	}

	pplx::task<void> GameFinderService::findMatch(const std::string &provider, const GameFinderRequest &mmRequest)
	{
		if (currentState() != GameFinderStatus::Idle)
		{
			return pplx::task_from_exception<void>(std::runtime_error("Already matching !"));
		}

		_currentState = GameFinderStatus::Searching;
		_matchmakingCTS = pplx::cancellation_token_source();
		auto matchmakingToken = _matchmakingCTS.get_token();

		return _rpcService->rpc<void>("match.find", provider, mmRequest).then([=](pplx::task<void> res) {
			if (matchmakingToken.is_canceled())
			{
				return pplx::task_from_exception<void>(std::runtime_error("GameFinder canceled"));
			}
			else
			{
				return res;
			}
		});
	}

	pplx::task<void> GameFinderService::findMatch(const std::string &provider, std::string json)
	{
		if (currentState() != GameFinderStatus::Idle)
		{
			return pplx::task_from_exception<void>(std::runtime_error("Already matching !"));
		}
		
		_currentState = GameFinderStatus::Searching;
		_matchmakingCTS = pplx::cancellation_token_source();
		auto matchmakingToken = _matchmakingCTS.get_token();

		return _rpcService->rpc<void>("match.find", provider, json).then([=](pplx::task<void> res) {
			if (matchmakingToken.is_canceled())
			{
				return pplx::task_from_exception<void>(std::runtime_error("Operation cancelled"));
			}
			else
			{
				
				return res;
			}
		});
	}

	void GameFinderService::resolve(bool acceptMatch)
	{
		auto scene = _scene.lock();
		scene->send("match.ready.resolve", [=](obytestream* stream) {
			*stream << acceptMatch;
		}, PacketPriority::MEDIUM_PRIORITY, PacketReliability::RELIABLE_ORDERED);
	}

	void GameFinderService::cancel()
	{
		if (currentState() != GameFinderStatus::Idle)
		{
			auto scene = _scene.lock();
			_matchmakingCTS.cancel();
			scene->send("match.cancel", [](obytestream*) {}, PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE_ORDERED);
			
		}
	}
};