#pragma once
#include "stormancer/headers.h"
#include "pplx/pplxtasks.h"
#include "stormancer/scene.h"
#include "stormancer/Configuration.h"
namespace Stormancer
{
	class IClient
	{
	public:
		using SceneInitializer = std::function<void(std::shared_ptr<Scene>)>;

		/// Factory.
		/// \param config A pointer to a Configuration.
		static std::shared_ptr<IClient> create(Configuration_ptr config);


		virtual pplx::task<std::shared_ptr<Scene>> connectToPublicScene(const std::string& sceneId, const SceneInitializer& initializer = SceneInitializer(), pplx::cancellation_token ct = pplx::cancellation_token::none()) = 0;

		virtual pplx::task<std::shared_ptr<Scene>> connectToPrivateScene(const std::string& sceneToken, const SceneInitializer& initializer = SceneInitializer(), pplx::cancellation_token ct = pplx::cancellation_token::none()) = 0;


		virtual pplx::task<void> disconnect() = 0;

		/// Get sync clock value
		virtual int64 clock() const = 0;

		/// Get last ping from sync clock
		virtual int64 lastPing() const = 0;

		/// Get dependency resolver
		virtual std::shared_ptr<DependencyResolver> dependencyResolver() = 0;



		/// Set a metadata
		virtual void setMedatata(const std::string& key, const std::string& value) = 0;

	};
}