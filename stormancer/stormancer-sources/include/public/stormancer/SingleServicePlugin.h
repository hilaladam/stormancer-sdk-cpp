#pragma once

#include "stormancer/BuildConfig.h"


#include "stormancer/IPlugin.h"
#include "stormancer/IClient.h"

namespace Stormancer
{
	enum class ServiceContextFlags : int
	{
		None = 0,
		Client = 1,
		Scene = 2,
		CreateWithClient = 4,
		CreateWithScene = 8,
		SingleInstance = 16
	};

	using TServiceContextFlags = std::underlying_type<ServiceContextFlags>::type;

	inline ServiceContextFlags operator|(ServiceContextFlags lhs, ServiceContextFlags rhs)
	{
		return (ServiceContextFlags)(static_cast<TServiceContextFlags>(lhs) | static_cast<TServiceContextFlags>(rhs));
	}

	inline ServiceContextFlags& operator|=(ServiceContextFlags& lhs, ServiceContextFlags rhs)
	{
		lhs = (ServiceContextFlags)(static_cast<TServiceContextFlags>(lhs) | static_cast<TServiceContextFlags>(rhs));
		return lhs;
	}

	inline ServiceContextFlags operator&(ServiceContextFlags lhs, ServiceContextFlags rhs)
	{
		return (ServiceContextFlags)(static_cast<TServiceContextFlags>(lhs) & static_cast<TServiceContextFlags>(rhs));
	}

	inline ServiceContextFlags& operator&=(ServiceContextFlags& lhs, ServiceContextFlags rhs)
	{
		lhs = (ServiceContextFlags)(static_cast<TServiceContextFlags>(lhs) & static_cast<TServiceContextFlags>(rhs));
		return lhs;
	}

	struct ServiceOptions
	{
		ServiceContextFlags contextFlags;
		std::string metadataKey;
	};

	template<class T, const ServiceOptions& options>
	class SingleServicePlugin : public IPlugin
	{
	public:

#pragma region public_method

		void registerClientDependencies(ContainerBuilder& builder) override
		{
			bool enabledOnContext = (options.contextFlags & ServiceContextFlags::Client) != ServiceContextFlags::None;
			if (enabledOnContext)
			{
				bool singleInstance = (options.contextFlags & ServiceContextFlags::SingleInstance) != ServiceContextFlags::None;
				auto registration = builder.registerDependency<T>();
				if (singleInstance)
				{
					registration.singleInstance();
				}
			}
		}

		void clientCreated(std::shared_ptr<IClient> client) override
		{
			if (!client)
			{
				throw std::runtime_error("The client has been deleted.");
			}

			bool createWithClient = (options.contextFlags & ServiceContextFlags::CreateWithClient) != ServiceContextFlags::None;
			if (createWithClient)
			{
				auto service = client->dependencyResolver().resolve<T>();
				service->setClient(client);
			}
		}

		void registerSceneDependencies(ContainerBuilder& sceneBuilder, std::shared_ptr<Scene> scene) override
		{
			if (!scene)
			{
				throw std::runtime_error("The scene has been deleted.");
			}

			bool enabledOnContext = (options.contextFlags & ServiceContextFlags::Scene) != ServiceContextFlags::None;
			bool enabledOnScene = SingleServicePlugin::isEnabled(scene, options.metadataKey);
			if (enabledOnContext && enabledOnScene)
			{
				auto registration = sceneBuilder.registerDependency<T>();
				if ((options.contextFlags & ServiceContextFlags::SingleInstance) != ServiceContextFlags::None)
				{
					registration.singleInstance();
				}
			}
		}

		void sceneCreated(std::shared_ptr<Scene> scene) override
		{
			if (!scene)
			{
				throw std::runtime_error("The scene has been deleted.");
			}

			bool createWithScene = (options.contextFlags & ServiceContextFlags::CreateWithScene) != ServiceContextFlags::None;
			bool enabledOnScene = SingleServicePlugin::isEnabled(scene, options.metadataKey);
			if (createWithScene && enabledOnScene)
			{
				auto service = scene->dependencyResolver().resolve<T>();
				service->setScene(scene);
			}
		}

		void sceneConnected(std::shared_ptr<Scene> scene) override
		{
			if (SingleServicePlugin::isEnabled(scene, options.metadataKey))
			{
				auto service = scene->dependencyResolver().resolve<T>();
				service->sceneConnected(scene);
			}
		}

#pragma endregion

#pragma region public_static_methods

		static bool isEnabled(std::shared_ptr<Scene> scene, const std::string& metadataKey)
		{
			if (!scene)
			{
				throw std::runtime_error("The scene has been deleted.");
			}

			auto name = scene->getHostMetadata(metadataKey);
			return (!name.empty());
		}

#pragma endregion
	};
}
