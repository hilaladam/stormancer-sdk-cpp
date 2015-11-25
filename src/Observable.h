#pragma once
#include "headers.h"
#include "IObservable.h"
#include "Action.h"

namespace Stormancer
{
	template<typename T>
	class Observable : public IObservable<T>
	{
	public:
		Observable(rxcpp::observable<T> observable)
			: _observable(observable)
		{
		}

		~Observable()
		{
		}

		ISubscription* subscribe(std::function<void(T)> onNext)
		{
			auto onError = [](std::exception_ptr ep){
				try
				{
					std::rethrow_exception(ep);
				}
				catch (const std::exception& ex)
				{
					ILogger::instance()->log(LogLevel::Error, "Observable::subscribe", "Observable error", ex.what());
				}
			};

			auto subscription = _observable.subscribe(onNext, onError);

			return new Subscription([subscription]() {
				subscription.unsubscribe();
			});
		}

		ISubscription* subscribe(std::function<void(T)> onNext, std::function<void(std::exception_ptr)> onError)
		{
			auto subscription = _observable.subscribe(onNext, onError);

			return new Subscription([subscription]() {
				subscription.unsubscribe();
			});
		}

		ISubscription* subscribe(std::function<void(T)> onNext, std::function<void(std::exception_ptr)> onError, std::function<void()> onComplete)
		{
			auto subscription = _observable.subscribe(onNext, onError, onComplete);

			return new Subscription([subscription]() {
				subscription.unsubscribe();
			});
		}

		ISubscription* subscribe(std::function<void(T)> onNext, std::function<void()> onComplete)
		{
			auto onError = [](std::exception_ptr ep){
				try
				{
					std::rethrow_exception(ep);
				}
				catch (const std::exception& ex)
				{
					ILogger::instance()->log(LogLevel::Error, "Observable::subscribe", "Observable error", ex.what());
				}
			};

			auto subscription = _observable.subscribe(onNext, onError, onComplete);

			return new Subscription([subscription]() {
				subscription.unsubscribe();
			});
		}

		void destroy()
		{
			delete this;
		}

	private:
		rxcpp::observable<T> _observable;
	};
};
