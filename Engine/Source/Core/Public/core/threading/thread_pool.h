#pragma once

#include <common.h>
#include <core/threading/task_queue.h>

#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <cereal/access.hpp>

namespace fluff { namespace threading {

	class ThreadPool
	{
		TaskQueue<std::function<void(size_t ID)> *> Queue_;
		std::vector<std::unique_ptr<std::thread>> Threads_;
		std::vector<std::shared_ptr<std::atomic<bool>>> Flags_;

		std::atomic<bool> Completed_;
		std::atomic<bool> Stop_;
		std::atomic<bool> NeedsToSync_;
		std::atomic<size_t> Waiting_;

		std::mutex Mutex_;
		std::condition_variable Condition_;

		friend class cereal::access;

		template <typename Archive>
		void save(Archive & Ar) const
		{
			Ar(Threads_.size())
		}

		template <typename Archive>
		void load(Archive & Ar)
		{
			size_t size;
			Ar(size);
		}
	public:
		FLUFF_API ThreadPool(size_t NumThreads = 4);
		FLUFF_API ~ThreadPool();

		size_t FLUFF_API Size() const;
		size_t FLUFF_API IdleThreadCount() const;
		std::thread FLUFF_API & GetThreadByIdx(size_t Idx);
		
		void FLUFF_API SetThreadCount(const size_t Count);
		void FLUFF_API ClearTaskQueue();
		void FLUFF_API Stop(bool Wait = false);
		void FLUFF_API Sync(bool ShouldSync = true);

		std::function<void(size_t)> FLUFF_API PopTask();

		template <typename Func, typename ... Arguments>
		auto PushTask(Func && F, Arguments && ... Args);

		template <typename Func>
		auto PushTask(Func && F);
	private:
		ThreadPool(const ThreadPool &) = delete;
		ThreadPool(ThreadPool &&) = delete;
		ThreadPool & operator = (const ThreadPool &) = delete;
		ThreadPool & operator = (ThreadPool &&) = delete;

		void FLUFF_API SetThread(size_t Thread);
		void FLUFF_API Initialize();
	};

	template<typename Func, typename ...Arguments>
	inline auto ThreadPool::PushTask(Func && F, Arguments && ...Args)
	{
		auto pack = std::make_shared<std::packaged_task<decltype(F(0, Args...))(size_t)>>(
			std::bind(std::forward<Func>(F), std::placeholders::_1, std::forward<Arguments>(Args)...)
		);

		auto func = new std::function<void(size_t id)>([pack](size_t id) {
			(*pack)(id);
		});

		Queue_.push_back(func);
		std::unique_lock<std::mutex> lock(Mutex_);
		Condition_.notify_one();
		return pack->get_future();
	}

	template<typename Func>
	inline auto ThreadPool::PushTask(Func && F)
	{
		auto pack = std::make_shared<std::packaged_task<decltype(F(0))(size_t)>>(std::forward<Func>(F));
		auto func = new std::function<void(size_t id)>([pack](size_t id) {
			(*pack)(id);
		});
		Queue_.push_back(func);
		std::unique_lock<std::mutex> lock(Mutex_);
		Condition_.notify_one();
		return pack->get_future();
	}

}	}

namespace cereal
{
	template <> struct LoadAndConstruct<fluff::threading::ThreadPool>
	{
		template <class Archive>
		static void load_and_construct(Archive & ar, cereal::construct<fluff::threading::ThreadPool> & construct)
		{
			size_t count;
			ar(count);
			construct(count);
		}
	};
}