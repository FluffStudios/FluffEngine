#include <core/threading/thread_pool.h>

namespace fluff { namespace threading {

	ThreadPool::ThreadPool(size_t NumThreads)
	{
		Initialize();
		SetThreadCount(NumThreads);
	}

	ThreadPool::~ThreadPool()
	{
		Stop(true);
	}

	size_t ThreadPool::Size() const
	{
		return Threads_.size();
	}
	
	size_t ThreadPool::IdleThreadCount() const
	{
		return Waiting_;
	}

	std::thread & ThreadPool::GetThreadByIdx(size_t Idx)
	{
		FLUFF_ASSERT(Idx < Threads_.size())
		return *(Threads_[Idx]);
	}

	void ThreadPool::SetThreadCount(size_t NumThreads)
	{
		if (!Stop_ && !Completed_) {
			size_t old = Threads_.size();
			if (old <= NumThreads)
			{
				Threads_.resize(NumThreads);
				Flags_.resize(NumThreads);

				for (size_t i = old; i < NumThreads; i++)
				{
					Flags_[i] = std::make_shared<std::atomic<bool>>(false);
					SetThread(i);
				}
			} 
			else
			{
				for (size_t i = old; i >= NumThreads; --i)
				{
					*(Flags_[i]) = true;
					Threads_[i]->detach();
				}
				{
					std::unique_lock<std::mutex> lock(Mutex_);
					Condition_.notify_all();
				}
				Threads_.resize(NumThreads);
				Flags_.resize(NumThreads);
			}
		}
	}

	void ThreadPool::ClearTaskQueue()
	{
		std::function<void(size_t id)> * func;
		while (Queue_.pop_front(func))
		{
			delete func;
		}
	}

	void ThreadPool::Stop(bool Wait)
	{
		if (!Wait)
		{
			if (Stop_)
			{
				return;
			}
			Stop_ = true;
			for (size_t i = 0, n = Size(); i < n; ++i)
			{
				*(Flags_[i]) = true;
			}
			ClearTaskQueue();
		}
		else
		{
			if (Completed_ || Stop_)
			{
				return;
			}
			Completed_ = true;
		}
		{
			std::unique_lock<std::mutex> lock(Mutex_);
			Condition_.notify_all();
		}
		for (size_t i = 0; i < Threads_.size(); ++i)
		{
			if (Threads_[i]->joinable())
			{
				Threads_[i]->join();
			}
		}
		ClearTaskQueue();
		Threads_.clear();
		Flags_.clear();
	}

	void FLUFF_API ThreadPool::Sync(bool ShouldSync)
	{
		NeedsToSync_ = ShouldSync;
	}

	std::function<void(size_t)> ThreadPool::PopTask()
	{
		std::function<void(size_t id)> * func = nullptr;
		Queue_.pop_front(func);
		std::unique_ptr<std::function<void(size_t id)>> res(func);
		std::function<void(size_t)> f;
		if (func) {
			f = *func;
		}
		return f;
	}

	void ThreadPool::SetThread(size_t ThreadIDX)
	{
		std::shared_ptr<std::atomic<bool>> flag(Flags_[ThreadIDX]);
		auto func = [this, ThreadIDX, flag]() {
			std::atomic<bool> & fg = *flag;
			std::function<void(size_t id)> * f;
			bool isPop = Queue_.pop_front(f);

			while (true)
			{
				while (isPop)
				{
					while (NeedsToSync_) 
					{
						// TODO: Replace with Condition Variable
					}

					std::unique_ptr<std::function<void(size_t id)>> func(f);
					(*f)(ThreadIDX);
					if (fg)
					{
						return;
					}
					else
					{
						isPop = Queue_.pop_front(f);
					}
				}

				std::unique_lock<std::mutex> lock(Mutex_);
				++Waiting_;
				Condition_.wait(lock, [this, &f, &isPop, &fg] {
					isPop = Queue_.pop_front(f);
					return isPop || Completed_ || fg;
				});
				--Waiting_;
				if (!isPop)
				{
					return;
				}
			}
		};

		Threads_[ThreadIDX].reset(new std::thread(func));
	}

	void ThreadPool::Initialize()
	{
		Waiting_ = 0; 
		Stop_ = false; 
		Completed_ = false;
		NeedsToSync_ = false;
	}

}	}