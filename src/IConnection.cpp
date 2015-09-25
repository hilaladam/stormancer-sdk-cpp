#include "stormancer.h"

namespace Stormancer
{
	int64 IConnection::id()
	{
		return _id;
	}

	time_t IConnection::connectionDate()
	{
		return _connectionDate;
	}

	std::string IConnection::account()
	{
		return _account;
	}

	std::string IConnection::application()
	{
		return _application;
	}

	ConnectionState IConnection::state()
	{
		return _state;
	}
};
