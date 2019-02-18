#include "stormancer/stdafx.h"
#include "stormancer/utilities/TaskUtilities.h"

namespace Stormancer
{

	pplx::task<void> taskDelay(std::chrono::milliseconds milliseconds, pplx::cancellation_token ct)
	{
		pplx::task<void> sleepTask = pplx::task<void>([=]() {
			std::this_thread::sleep_for(milliseconds);
		}, pplx::task_options(ct));

		if (ct.is_cancelable())
		{
			pplx::task_completion_event<void> tce;
			ct.register_callback([=]() {
				tce.set();
			});
			pplx::task<void> cancellationTask = pplx::create_task(tce);

			return (cancellationTask || sleepTask);
		}
		else
		{
			return sleepTask;
		}
	}

	pplx::task<void> taskIf(bool condition, std::function<pplx::task<void>()> action)
	{
		if (condition)
		{
			return action();
		}
		else
		{
			return pplx::task_from_result();
		}
	}
}

namespace pplx
{
	pplx::cancellation_token operator||(pplx::cancellation_token token1, pplx::cancellation_token token2)
	{
		std::vector<pplx::cancellation_token> tokens;
		if (token1.is_cancelable())
		{
			tokens.push_back(token1);
		}
		if (token2.is_cancelable())
		{
			tokens.push_back(token2);
		}
		return pplx::cancellation_token_source::create_linked_source(tokens.begin(), tokens.end()).get_token();
	}
}