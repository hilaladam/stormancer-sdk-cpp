#pragma once
#include "headers.h"
#include "Dto/ConnectionData.h"

namespace Stormancer
{
	/// Informations to connect to a scene.
	class SceneEndpoint
	{
	public:
		SceneEndpoint();
		SceneEndpoint(std::string& token, ConnectionData& tokenData);
		virtual ~SceneEndpoint();

	public:
		ConnectionData tokenData;
		std::string token;
	};
};