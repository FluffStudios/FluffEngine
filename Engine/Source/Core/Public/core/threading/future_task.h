#pragma once

#include <future>

namespace fluff { namespace threading {

	template<typename Type>
	class FutureTask
	{
		std::future<Type> Future_;
	public:
		FutureTask(std::future<Type> && Future)
			: Future_(std::move(Future))
		{	}

		FutureTask(const FutureTask & RHS) = delete;
		FutureTask& operator= (const FutureTask & RHS) = delete;
		FutureTask(FutureTask && Other) = default;
		FutureTask& operator= (FutureTask && Other) = default;
		~FutureTask()
		{
			if (Future_.valid())
			{
				Future_.get();
			}
		}

		auto Get()
		{
			return Future_.get();
		}
	};

} }