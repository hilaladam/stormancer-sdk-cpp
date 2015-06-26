#pragma once
#include "headers.h"
#include "SceneEndpoint.h"

namespace Stormancer
{
	/// Interface for a token handler
	class ITokenHandler
	{
	public:
		ITokenHandler();
		virtual ~ITokenHandler();

		virtual SceneEndpoint decodeToken(std::string& token) = 0;
	};
};