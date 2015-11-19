#pragma once
#include "headers.h"
#include "RpcClientPlugin.h"
#include "RpcRequest.h"
#include "RpcRequestContext.h"
#include "Observable.h"
#include "Scene.h"

namespace Stormancer
{
	class RpcService
	{
	public:
		RpcService(Scene* scene);
		~RpcService();

	public:
		IObservable<Packetisp_ptr>* rpc(const char* route, Action<bytestream*> writer, PacketPriority priority)
		{
			if (!_scene)
			{
				throw std::runtime_error("The scene ptr is invalid");
			}

			auto observable = rxcpp::observable<>::create<Packetisp_ptr>([this, writer, route, priority](rxcpp::subscriber<Packetisp_ptr> subscriber) {
				auto rr = _scene->remoteRoutes();
				Route_ptr relevantRoute;

				auto sz = rr.Size();
				for (uint32 i = 0; i < sz; ++i)
				{
					auto r = rr[i];
					if (r->name() == route)
					{
						relevantRoute = r;
						break;
					}
				}

				if (!relevantRoute)
				{
					throw std::runtime_error("The target route does not exist on the remote host.");
				}

				if (relevantRoute->metadata()[RpcClientPlugin::pluginName] != RpcClientPlugin::version)
				{
					throw std::runtime_error(std::string("The target remote route does not support the plugin RPC version ") + RpcClientPlugin::version);
				}

				RpcRequest_ptr request(new RpcRequest(subscriber));
				auto id = reserveId();
				request->id = id;

				_pendingRequestsMutex.lock();
				_pendingRequests[id] = request;
				_pendingRequestsMutex.unlock();

				_scene->sendPacket(route, [id, writer](bytestream* bs) {
					*bs << id;
					writer(bs);
				}, priority, PacketReliability::RELIABLE_ORDERED);

				subscriber.add([this, id]() {
					RpcRequest_ptr request;

					_pendingRequestsMutex.lock();
					if (mapContains(_pendingRequests, id))
					{
						request = _pendingRequests[id];
						_pendingRequests.erase(id);
					}
					_pendingRequestsMutex.unlock();

					if (request)
					{
						_scene->sendPacket(RpcClientPlugin::cancellationRouteName, [this, id](bytestream* bs) {
							*bs << id;
						});
					}
				});
			});
			return new Observable<Packetisp_ptr>(observable.as_dynamic());
		}
		
		STORMANCER_DLL_API uint16 pendingRequests();
		STORMANCER_DLL_API void addProcedure(const char* route, std::function<pplx::task<void>(RpcRequestContex_ptr)> handler, bool ordered);
		void next(Packetisp_ptr packet);
		void error(Packetisp_ptr packet);
		void complete(Packetisp_ptr packet);
		void cancel(Packetisp_ptr packet);
		void disconnected();

	private:
		STORMANCER_DLL_API uint16 reserveId();
		RpcRequest_ptr getPendingRequest(Packetisp_ptr packet);

	private:
		std::map<uint16, RpcRequest_ptr> _pendingRequests;
		std::mutex _pendingRequestsMutex;
		std::map<uint16, pplx::cancellation_token_source> _runningRequests;
		std::mutex _runningRequestsMutex;
		Scene* _scene;
	};
};