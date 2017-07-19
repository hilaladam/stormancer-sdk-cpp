#pragma once
#include <headers.h>
#include "PlayerProfileService.h"

namespace Stormancer
{
	template<class T>
	class PlayerProfilePlugin : public IPlugin
	{
		virtual void registerSceneDependencies(Scene* scene) override
		{
			if (scene)
			{
				auto name = scene->getHostMetadata("profiles");

				if (!name.empty())
				{
					auto service = std::make_shared<PlayerProfileService<T>>(scene->dependencyResolver());
					service->setScene(scene);
					scene->dependencyResolver()->registerDependency(service);
				}
			}
		};
	};
}