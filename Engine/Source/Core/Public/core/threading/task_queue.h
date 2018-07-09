#pragma once

#include <mutex>
#include <queue>

namespace fluff { namespace threading {

	template <typename Type>
	class TaskQueue
	{
		std::queue<Type> Impl_;
		std::mutex Mutex_;
	public:
		bool push_back(const Type & Value)
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			this->Impl_.push(Value);
			return true;
		}

		bool push_back(Type && Value)
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			this->Impl_.push(std::move(Value));
			return true;
		}

		bool pop_front(Type & Value)
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			if (Impl_.empty())
			{
				return false;
			}
			Value = Impl_.front();
			Impl_.pop();
			return true;
		}

		bool empty()
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			return Impl_.empty();
		}
	};

} }