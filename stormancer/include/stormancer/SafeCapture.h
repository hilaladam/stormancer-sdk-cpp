#pragma once

#include "headers.h"
#include "stormancer/CustomExceptions.h"

#if __cplusplus >= 201703L
#define STRM_WEAK_FROM_THIS() this->weak_from_this()
#else
#define STRM_WEAK_FROM_THIS() Stormancer::GetWeakFromThis(this)
#endif

namespace Stormancer
{
	template<typename T>
	std::weak_ptr<T> GetWeakFromThis(T* t)
	{
		try
		{
			return t->shared_from_this();
		}
		catch (const std::bad_weak_ptr&)
		{
			return std::weak_ptr<T>();
		}
	}

	template<typename TClass, typename TCallable>
	class SafeCapture
	{
	private:

		std::weak_ptr<TClass> _weakPtr;
		TCallable _callable;

	public:

		SafeCapture(std::weak_ptr<TClass> weakPtr, TCallable callable)
			: _weakPtr(weakPtr)
			, _callable(callable)
		{
		}

		template<class... Args>
		auto operator()(Args&&...args) const -> decltype(_callable(std::forward<Args>(args)...))
		{
			auto sharedPtr = _weakPtr.lock();
			if (sharedPtr)
			{
				return _callable(std::forward<Args>(args)...);
			}
			else
			{
				throw PointerDeletedException();
			}
		}
	};

	template<typename TClass, typename TCallable>
	class SafeCaptureNoThrow
	{
	private:

		std::weak_ptr<TClass> _weakPtr;
		TCallable _callable;

	public:

		SafeCaptureNoThrow(std::weak_ptr<TClass> weakPtr, TCallable callable)
			: _weakPtr(weakPtr)
			, _callable(callable)
		{
		}

		template<class... Args>
		auto operator()(Args&&...args) const -> decltype(_callable(std::forward<Args>(args)...))
		{
			auto sharedPtr = _weakPtr.lock();
			if (sharedPtr)
			{
				return _callable(std::forward<Args>(args)...);
			}
			else
			{
				return; // Can avoid to throw only if the callable returns void
			}
		}
	};

	template<typename TClass, typename TCallable>
	SafeCapture<TClass, TCallable> createSafeCapture(std::weak_ptr<TClass> weakPtr, TCallable callable)
	{
		return SafeCapture<TClass, TCallable>(weakPtr, callable);
	}

	template<typename TClass, typename TCallable>
	SafeCaptureNoThrow<TClass, TCallable> createSafeCaptureNoThrow(std::weak_ptr<TClass> weakPtr, TCallable callable)
	{
		return SafeCaptureNoThrow<TClass, TCallable>(weakPtr, callable);
	}
}
