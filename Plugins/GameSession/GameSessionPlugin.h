#pragma once

#include "stormancer/headers.h"
#include "stormancer/IPlugin.h"

namespace Stormancer
{
	class GameSessionPlugin : public IPlugin
	{
	public:
		void sceneCreated(std::shared_ptr<Scene> scene) override;
		void sceneDisconnecting(std::shared_ptr<Scene> scene) override;
	};
};
