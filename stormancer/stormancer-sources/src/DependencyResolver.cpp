#include "stormancer/stdafx.h"
#include "stormancer/SafeCapture.h"
#include "stormancer/DependencyResolver.h"

namespace Stormancer
{
	DependencyResolver::DependencyResolver(std::weak_ptr<DependencyResolver> parent)
		: _parent(parent)
	{
	}

	DependencyResolver::~DependencyResolver()
	{
	}

	void DependencyResolver::registerDependencyInternal(const std::type_info& t, Registration& registration, bool singleInstance)
	{
		auto h = t.hash_code();
		registration.singleInstance = singleInstance;
		_factories[h] = registration;
	}

	std::shared_ptr<void> DependencyResolver::resolveInternal(const std::type_info& t)
	{
		auto h = t.hash_code();
		auto it = _factories.find(h);
		if (it != _factories.end())
		{
			auto& registration = it->second;
			if (registration.singleInstance)
			{
				if (registration.instance == nullptr)
				{
					if (registration.factory)
					{
						registration.instance = registration.factory(STRM_WEAK_FROM_THIS());
					}
				}
				if (registration.instance)
				{
					return registration.instance;
				}
			}
			else
			{
				if (registration.factory)
				{
					return registration.factory(STRM_WEAK_FROM_THIS());
				}
			}
		}

		auto parent = _parent.lock();
		if (parent)
		{
			return parent->resolveInternal(t);
		}
		else
		{
			return nullptr;
		}
	}
};