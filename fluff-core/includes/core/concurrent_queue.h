#pragma once

#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace fluff
{

	template <typename Type>
	class ConcurrentQueue
	{
		std::atomic_bool Valid_;
		mutable std::mutex Mutex_;
		std::queue<Type> InternalQueue_;
		std::condition_variable Condition_;
	public:
		ConcurrentQueue() 
			: Valid_(true)
		{	}

		~ConcurrentQueue()
		{
			this->Invalidate();
		}

		bool TryPop(Type & Result)
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			if (InternalQueue_.empty() || !Valid_)
			{
				return false;
			}
			Result = std::move(InternalQueue_.front());
			InternalQueue_.pop();
			return true;
		}

		bool WaitAndPop(Type & Result)
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			Condition_.wait(lock, [this]() {
				return !InternalQueue_.empty() || !Valid_;
			});

			if (!Valid_)
			{
				return false;
			}
			Result = std::move(InternalQueue_.front());
			InternalQueue_.pop();
			return true;
		}

		void Push(Type Value)
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			InternalQueue_.push(std::move(Value));
			Condition_.notify_one();
		}

		bool IsEmpty() const
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			return InternalQueue_.empty();
		}

		void Clear()
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			while (!InternalQueue_.empty())
			{
				InternalQueue_.pop();
			}
			Condition_.notify_all();
		}

		void Invalidate()
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			Valid_ = false;
			Condition_.notify_all();
		}

		bool IsValid() const
		{
			std::lock_guard<std::mutex> lock(Mutex_);
			return Valid_;
		}
	};

}