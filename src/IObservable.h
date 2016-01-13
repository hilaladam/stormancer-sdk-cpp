#pragma once
#include "Action.h"

namespace Stormancer
{
	template<typename T>
	class IObservable
	{
	public:
		virtual std::weak_ptr<ISubscription> subscribe(std::function<void(T)> onNext, std::function<void(const char* errorMessage)> onError, std::function<void()> onComplete) = 0;
		virtual std::weak_ptr<ISubscription> subscribe(std::function<void(T)> onNext, std::function<void(const char* errorMessage)> onError) = 0;
		virtual std::weak_ptr<ISubscription> subscribe(std::function<void(T)> onNext) = 0;
		virtual std::weak_ptr<ISubscription> subscribe(std::function<void(T)> onNext, std::function<void()> onComplete) = 0;
		virtual void destroy() = 0;
	};
}
