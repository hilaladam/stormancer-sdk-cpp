#pragma once
#include "stormancer/headers.h"
#include "stormancer/IPlugin.h"

namespace Stormancer
{
	class BugReportPlugin : public IPlugin
	{
	public:
		void sceneCreated(std::shared_ptr<Scene> scene) override;
	};
};
