#pragma once

#include <core/concurrent_queue.h>
#include <core/threading/future_task.h>
#include <core/threading/itask.h>

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

namespace fluff { namespace threading {

	class ThreadPool
	{
		std::atomic_bool Done_;
		ConcurrentQueue<std::unique_ptr<ITask>> WorkQueue_;
		std::vector<std::thread> Threads_;

		static std::vector<std::shared_ptr<ThreadPool>> Instances_;
	public:
		static std::shared_ptr<ThreadPool> GetInstance(uint32_t ID = 0);
		static std::shared_ptr<ThreadPool> CreateInstance(const uint32_t NumThreads, uint32_t & ID);

		explicit FLUFF_API ThreadPool(const uint32_t NumThreads);
		FLUFF_API ThreadPool(const ThreadPool & RHS) = delete;
		ThreadPool FLUFF_API & operator= (const ThreadPool & RHS) = delete;
		FLUFF_API ~ThreadPool();

		template <typename Func, typename ... Args>
		auto Submit(Func && Function, Args && ... Arguments)
		{
			auto boundTask = std::bind(std::forward<Func>(Function), std::forward<Args>(Arguments)...);
			using RType = std::result_of_t<decltype(boundTask)()>;
			using PTask = std::packaged_task<RType()>;
			using TType = Task<PTask>;

			PTask task(std::move(boundTask));
			FutureTask<RType> result(task.get_future());
			WorkQueue_.Push(std::make_unique<TType>(std::move(task)));
			return result;
		}
	private:
		void FLUFF_API Worker();
		void FLUFF_API Destroy();
	};

} }