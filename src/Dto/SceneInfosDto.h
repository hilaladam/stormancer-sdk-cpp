#pragma once
#include "headers.h"
#include "Dto/RouteDto.h"

namespace Stormancer
{
	/// Informations about a remote scene.
	struct SceneInfosDto
	{
	public:
	
		/// Constructor.
		SceneInfosDto();

		/// Destructor.
		virtual ~SceneInfosDto();

		/// deserialize by using MessagePack.
		void msgpack_unpack(msgpack::object const& o);

	public:
		
		/// The scene id.
		std::string SceneId;
		
		/// The scene metadatas.
		stringMap Metadata;
		
		/// Vector of routes declared on the scene host.
		std::vector<RouteDto> Routes;
		
		/// The serializer the client should use when communicating with the scene.
		std::string SelectedSerializer;
	};
};