
#include "stormancer/headers.h"
#include "stormancer/RPC/service.h"
#include "Authentication/AuthenticationService.h"
#include "stormancer/utilities/taskUtilities.h"
//#include "stormancer/Debug/StackWalker.h"

namespace Stormancer
{
	AuthenticationService::AuthenticationService(std::shared_ptr<IClient> client)
		: _client(client)
		, _logger(client->dependencyResolver()->resolve<ILogger>())
	{

	}

	AuthenticationService::~AuthenticationService()
	{
		_connectionSubscription.unsubscribe();
	}

	pplx::task<void> AuthenticationService::login()
	{
		_autoReconnect = true;
		return getAuthenticationScene().then([](std::shared_ptr<Scene>) {});
	}

	pplx::task<std::shared_ptr<Scene>> AuthenticationService::loginImpl(int retry)
	{
		std::weak_ptr<AuthenticationService> wThat = this->shared_from_this();

		if (!this->getCredentialsCallback)
		{
			return pplx::task_from_exception<std::shared_ptr<Scene>>(std::runtime_error("'getCredentialsCallback' must be set before authentication."));
		}
		auto client = _client.lock();
		if (!client)
		{
			return pplx::task_from_exception<std::shared_ptr<Scene>>(std::runtime_error("'Client destroyed."));
		}
		return client->connectToPublicScene(SCENE_ID, [wThat](std::shared_ptr<Scene> scene) {

			auto that = wThat.lock();
			if (that)
			{
				that->_connectionSubscription = scene->getConnectionStateChangedObservable().subscribe([wThat](ConnectionState state) {
					auto that = wThat.lock();
					if (that)
					{
						switch (state)
						{
						case ConnectionState::Disconnecting:
							that->setConnectionState(GameConnectionState::Disconnecting);
							break;
						case ConnectionState::Disconnected:
							that->setConnectionState(GameConnectionState::Disconnected, state.reason);
							break;
						default:
							break;
						}

					}

				});
			}
			scene->dependencyResolver()->resolve<RpcService>()->addProcedure("sendRequest", [wThat](RpcRequestContext_ptr ctx) {
				OperationCtx opCtx;
				opCtx.request = ctx;
				Serializer serializer;
				serializer.deserialize(ctx->inputStream(), opCtx.operation, opCtx.originId);

				auto that = wThat.lock();
				if (!that)
				{
					throw (std::runtime_error("Authentication service destroyed"));
				}

				auto it = that->_operationHandlers.find(opCtx.operation);
				if (it == that->_operationHandlers.end())
				{
					throw (std::runtime_error("operation.notfound"));
				}

				return it->second(opCtx);

			});

		}).then([wThat](std::shared_ptr<Scene> scene)
		{
			auto that = wThat.lock();

			if (!that)
			{
				throw (std::runtime_error("Authentication service destroyed"));
			}

			if (!that->getCredentialsCallback)
			{
				throw std::runtime_error("'getCredentialsCallback' must be set before authentication.");
			}

			if (!that->_autoReconnect)
			{
				throw std::runtime_error("Auto recconnection is disable please login before");
			}

			return that->getCredentialsCallback().then([scene, wThat](std::unordered_map<std::string, std::string> ctx) {
				auto that = wThat.lock();
				if (!that)
				{
					throw std::runtime_error("destroyed");
				}
				auto rpcService = scene->dependencyResolver()->resolve<RpcService>();
				return rpcService->rpc<LoginResult>(that->LOGIN_ROUTE, ctx);

			}).then([scene, wThat](LoginResult result) {

				auto that = wThat.lock();
				if (!that)
				{
					throw std::runtime_error("destroyed");
				}

				if (!result.success)
				{
					that->_autoReconnect = false;//disable auto reconnection
					that->setConnectionState(GameConnectionState::Disconnected);
					throw std::runtime_error("Login failed : " + result.errorMsg);
				}
				else
				{
					that->_userId = result.userId;
					that->_username = result.username;
					that->setConnectionState(GameConnectionState::Authenticated);
				}

				return scene;
			});

		}).then([wThat, retry](pplx::task<std::shared_ptr<Scene>> t) {
			auto that = wThat.lock();
			if (!that)
			{
				return pplx::task_from_result<std::shared_ptr<Scene>>(nullptr);
			}
			try
			{
				return pplx::task_from_result(t.get());
			}
			catch (std::exception& ex)
			{
				that->_logger->log(LogLevel::Error, "authentication", "An error occured while trying to connect to the server.", ex.what());
				if (that->_autoReconnect && that->connectionState() != GameConnectionState::Disconnected)
				{
					return that->reconnect(retry + 1);
				}
				else
				{
					return pplx::task_from_result<std::shared_ptr<Scene>>(nullptr);
				}
			}
		});

	}

	pplx::task<std::shared_ptr<Scene>> AuthenticationService::reconnect(int retry)
	{
		using namespace std::chrono_literals;
		auto delay = retry * 1000ms;
		if (delay > 5000ms)
		{
			delay = 5000ms;
		}

		std::weak_ptr<AuthenticationService> wThat = this->shared_from_this();

		this->setConnectionState(GameConnectionState::Reconnecting);
		return taskDelay(delay).then([wThat, retry]() {
			auto that = wThat.lock();
			if (!that)
			{
				throw std::runtime_error("destroyed");
			}
			return that->loginImpl(retry);

		});
	}

	pplx::task<std::shared_ptr<Scene>> AuthenticationService::getAuthenticationScene(pplx::cancellation_token ct)
	{

		if (!_client.expired())
		{
			if (!_authTask)
			{
				if (!_autoReconnect)
				{
					return pplx::task_from_exception<std::shared_ptr<Scene>>(std::runtime_error("Authenticator disconnected. Call login before using the authenticationService."));
				}
				else
				{
					_authTask = std::make_shared<pplx::task<std::shared_ptr<Scene>>>(loginImpl());
				}
			}

			auto t = *_authTask;
			std::weak_ptr<AuthenticationService> wThat = this->shared_from_this();
			pplx::task_completion_event<std::shared_ptr<Scene>> tce;
			if (ct.is_cancelable())
			{
				ct.register_callback([tce]() {
					tce.set_exception(pplx::task_canceled());
				});
			}
			t.then([tce, wThat](pplx::task<std::shared_ptr<Scene>> t) {

				try
				{
					auto scene = t.get();
					if (scene)
					{
						tce.set(scene);
					}
					else
					{
						throw std::runtime_error("Authentication failed");
					}

				}
				catch (std::exception& ex)
				{
					tce.set_exception(ex);
				}

			});
			if (auto that = wThat.lock())
			{
				if (auto client = _client.lock())
				{
					return pplx::create_task(tce, client->dependencyResolver()->resolve<IActionDispatcher>());
				}
			}
			return pplx::create_task(tce);
		}
		else
		{
			return pplx::task_from_exception<std::shared_ptr<Scene>>(std::runtime_error("Client is invalid."));
		}
	}

	pplx::task<void> AuthenticationService::logout()
	{
		_autoReconnect = false;
		if (_currentConnectionState != GameConnectionState::Disconnected && _currentConnectionState != GameConnectionState::Disconnecting)
		{
			this->setConnectionState(GameConnectionState::Disconnecting);

			return getAuthenticationScene()
				.then([](std::shared_ptr<Scene> scene)
			{
				return scene->disconnect();
			}).then([](auto t) {

				try
				{
					t.get();
				}
				catch (std::exception&)
				{

				}
			});
		}
		else
		{
			return pplx::task_from_result();
		}
	}



	pplx::task<std::string> AuthenticationService::getBearerToken()
	{
		return getAuthenticationScene()
			.then([](std::shared_ptr<Scene> authScene)
		{
			auto rpcService = authScene->dependencyResolver()->resolve<RpcService>();
			return rpcService->rpc<std::string>("sceneauthorization.getbearertoken");
		});
	}

	pplx::task<std::string> AuthenticationService::getUserFromBearerToken(std::string token)
	{
		return getAuthenticationScene()
			.then([token](std::shared_ptr<Scene> authScene)
		{
			auto rpcService = authScene->dependencyResolver()->resolve<RpcService>();
			return rpcService->rpc<std::string, std::string>("sceneauthorization.getuserfrombearertoken", token);
		});
	}

	std::string& AuthenticationService::username()
	{
		return _username;
	}

	std::string& AuthenticationService::userId()
	{
		return _userId;
	}

	pplx::task<std::string> AuthenticationService::getUserIdByPseudo(std::string pseudo)
	{
		return getAuthenticationScene()
			.then([pseudo](std::shared_ptr<Scene> authScene)
		{
			auto rpcService = authScene->dependencyResolver()->resolve<RpcService>();
			return rpcService->rpc<std::string, std::string>("users.getuseridbypseudo", pseudo);
		});
	}

	pplx::task<std::shared_ptr<Scene>> AuthenticationService::connectToPrivateScene(const std::string& sceneId, std::function<void(std::shared_ptr<Scene>)> builder)
	{
		std::weak_ptr<AuthenticationService> wThat = this->shared_from_this();
		return getAuthenticationScene()
			.then([sceneId](std::shared_ptr<Scene> authScene)
		{
			auto rpcService = authScene->dependencyResolver()->resolve<RpcService>();
			return rpcService->rpc<std::string, std::string>("sceneauthorization.gettoken", sceneId);
		})
			.then([wThat, builder](std::string token)
		{
			auto that = wThat.lock();

			if (that)
			{
				if (auto client = that->_client.lock())
				{
					return client->connectToPrivateScene(token, builder);
				}
			}

			throw std::runtime_error("Client is invalid.");

		});
	}

	pplx::task<std::shared_ptr<Scene>> AuthenticationService::getSceneForService(const std::string& serviceType, const std::string& serviceName)
	{
		std::weak_ptr<AuthenticationService> wThat = this->shared_from_this();
		return getAuthenticationScene()
			.then([serviceType, serviceName](std::shared_ptr<Scene> authScene)
		{
			auto rpcService = authScene->dependencyResolver()->resolve<RpcService>();
			return rpcService->rpc<std::string>("Locator.GetSceneConnectionToken", serviceType, serviceName);
		})
			.then([wThat](std::string token)
		{
			auto that = wThat.lock();

			if (that)
			{
				if (auto client = that->_client.lock())
				{
					return client->connectToPrivateScene(token);
				}
			}

			throw std::runtime_error("Client is invalid.");

		});
	}


	GameConnectionState AuthenticationService::connectionState() const
	{
		return _currentConnectionState;
	}

	void AuthenticationService::setConnectionState(GameConnectionState state, std::string reason)
	{
		if (_currentConnectionState != state)
		{
			this->_logger->log(LogLevel::Info, "connection", "Connection state changed", std::to_string((int)state) + "reason " + reason);



			if (state == GameConnectionState::Disconnected || state == GameConnectionState::Disconnecting)
			{
				_authTask = nullptr;
				if (reason == "User connected elsewhere" || reason == "Authentication failed")
				{
					_autoReconnect = false;
				}
				if (_autoReconnect)
				{
					setConnectionState(GameConnectionState::Reconnecting);
				}
				else
				{
					_currentConnectionState = state;
					connectionStateChanged(state);
				}


			}
			else if (state == GameConnectionState::Reconnecting && _currentConnectionState != GameConnectionState::Reconnecting)
			{
				_currentConnectionState = state;
				connectionStateChanged(state);
				this->getAuthenticationScene().then([](pplx::task<std::shared_ptr<Scene>> t) {
					try
					{
						t.get();
					}
					catch (std::exception)
					{

					}

				});
			}
			else
			{
				_currentConnectionState = state;
				connectionStateChanged(state);
			}

		}
	}

	void AuthenticationService::SetOperationHandler(std::string operation, std::function<pplx::task<void>(OperationCtx&)> handler)
	{
		_operationHandlers[operation] = handler;
	}


};
