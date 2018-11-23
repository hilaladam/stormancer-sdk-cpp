#include "Profile/ProfilePlugin.h"
#include "Profile/ProfileService.h"
#include "Profile/Profiles.h"

namespace Stormancer
{
	ProfilePlugin::ProfilePlugin()
	{
	}

	ProfilePlugin::~ProfilePlugin()
	{
	}

	void ProfilePlugin::sceneCreated(std::shared_ptr<Scene> scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.profiles");
			if (!name.empty())
			{
				std::shared_ptr<ProfileService> service = std::make_shared<ProfileService>(scene);
				scene->dependencyResolver()->registerDependency<ProfileService>(service);
			}
		}
	}
	
	void ProfilePlugin::clientCreated(std::shared_ptr<IClient> client)
	{
		if (client)
		{
			client->dependencyResolver()->registerDependency<Profiles>([](std::weak_ptr<DependencyResolver> dr) {
				return std::make_shared<Profiles>(dr.lock()->resolve<AuthenticationService>()); },true);
		}
	}
};