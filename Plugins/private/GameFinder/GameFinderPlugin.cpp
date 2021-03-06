#if defined(STORMANCER_CUSTOM_PCH)
#include STORMANCER_CUSTOM_PCH
#endif
#include "GameFinder/GameFinderPlugin.h"
#include "GameFinderService.h"
#include "GameFinder/GameFinder.h"
#include "GameFinder_Impl.h"
#include "Users/Users.hpp"

namespace Stormancer
{
	void GameFinderPlugin::registerSceneDependencies(ContainerBuilder& builder, std::shared_ptr<Scene> scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.plugins.gamefinder");
			if (!name.empty())
			{
				builder.registerDependency<GameFinderService, Scene>().singleInstance();
			}
		}
	}

	void GameFinderPlugin::sceneCreated(std::shared_ptr<Scene> scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.plugins.gamefinder");
			if (!name.empty())
			{
				scene->dependencyResolver().resolve<GameFinderService>()->initialize();
			}
		}
	}

	void GameFinderPlugin::registerClientDependencies(ContainerBuilder& builder)
	{
		builder.registerDependency<GameFinder_Impl, Users::UsersApi>().as<GameFinder>().singleInstance();
	}

	void GameFinderPlugin::sceneDisconnecting(std::shared_ptr<Scene> scene)
	{
		auto name = scene->getHostMetadata("stormancer.plugins.gamefinder");
		if (!name.empty())
		{
			scene->dependencyResolver().resolve<GameFinderService>()->onSceneDisconnecting();
		}
	}
}
