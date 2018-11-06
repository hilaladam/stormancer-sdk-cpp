#pragma once

#include "stormancer/headers.h"
#include "stormancer/IPlugin.h"
#include "stormancer/Scene.h"

namespace Stormancer
{
	class MatchmakingPlugin : public IPlugin
	{
	public:
		void sceneCreated(std::shared_ptr<Scene> scene) override;
	};
};
