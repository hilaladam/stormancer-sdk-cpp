#pragma once

#include "stormancer/BuildConfig.h"

#include "stormancer/StormancerTypes.h"
#include "stormancer/msgpack_define.h"
#include <unordered_map>
#include <string>

namespace Stormancer
{
	/// Information about a scene route.
	struct RouteDto
	{
	public:

		std::string Name;

		uint16 Handle;

		std::unordered_map<std::string, std::string> Metadata;

		MSGPACK_DEFINE_MAP(Name, Handle, Metadata);
	};
}
