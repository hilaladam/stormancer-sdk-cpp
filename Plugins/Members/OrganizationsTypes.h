#pragma once
#include <string>
#include "stormancer/msgpack_define.h"

namespace Stormancer
{
	enum class MemberStatus : int8
	{
		Accepted = 0,
		InvitationSent = 1,
		WaitingAccept = 2,
		Removed = 3
	};

	struct Role
	{
		std::string name;
		std::map<std::string, bool> rights;
		std::string document = "{}";

		MSGPACK_DEFINE(name, rights, document);
	};

	struct Organization
	{
		std::string id;
		std::string name;
		std::string ownerId;
		std::map<std::string, Role> roleDefinitions;
		std::string document = "{}";

		MSGPACK_DEFINE(id, name, ownerId, roleDefinitions, document);
	};

	struct Member
	{
		std::string id;
		std::string organizationId;
		std::string userId;
		std::vector<std::string> roles;
		MemberStatus status;

		MSGPACK_DEFINE(id, organizationId, userId, roles, status);
	};
}

MSGPACK_ADD_ENUM(Stormancer::MemberStatus);