#include <core/threading/threadpool.h>

namespace fluff { namespace threading {

	std::vector<std::shared_ptr<ThreadPool>> ThreadPool::Instances_;

	ThreadPool::ThreadPool(const uint32_t NumThreads)
		: Done_(false)
	{
		try
		{
			for (size_t i = 0; i < NumThreads; i++)
			{
				Threads_.emplace_back(&ThreadPool::Worker, this);
			}
		}
		catch (...)
		{
			this->Destroy();
			throw;
		}
	}

	std::shared_ptr<ThreadPool> ThreadPool::GetInstance(uint32_t ID)
	{
		if (Instances_.size() > 0) return Instances_[ID];
		return nullptr;
	}

	std::shared_ptr<ThreadPool> ThreadPool::CreateInstance(const uint32_t NumThreads, uint32_t & ID)
	{
		Instances_.push_back(std::make_shared<ThreadPool>(NumThreads));
		ID = static_cast<uint32_t>(Instances_.size()) - 1;
		return Instances_[ID];
	}

	ThreadPool::~ThreadPool()
	{
		Destroy();
	}

	void ThreadPool::Worker()
	{
		while (!Done_)
		{
			std::unique_ptr<ITask> task(nullptr);
			if (WorkQueue_.WaitAndPop(task))
			{
				task->Execute();
			}
		}
	}

	void ThreadPool::Destroy()
	{
		Done_ = true;
		WorkQueue_.Invalidate();
		for (auto & thr : Threads_)
		{
			if (thr.joinable())
			{
				thr.join();
			}
		}
	}

} }