#pragma once
#include <headers.h>
#include <IPlugin.h>
#include <Scene.h>

namespace Stormancer
{
	class MatchmakingPlugin : public IPlugin
	{
	public:
		void sceneCreated(Scene* scene) override;
	};
};