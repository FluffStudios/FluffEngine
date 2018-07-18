#include <core/ecs/task_system.h>

#include <core/debug/debug_log_writer.h>

namespace fluff { namespace ecs {

	TaskSystem::TaskSystem()
	{	}

	void TaskSystem::Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
		FLUFF_LOG(fluff::debug::DebugErrorType::ILLEGAL_STATE, fluff::debug::FATAL, "Needs Threadpool Attached");
	}

	void TaskSystem::Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime)
	{
		if (!FreeFuncs_.empty())
		{
			return;
		}

		for (auto & it : Tasks_)
		{
			auto task = it.first;
			if (task->ShouldRunOnMain())
			{
				for (auto ent : it.second)
				{
					task->SetEntity(&ent);
					task->Update();
				}
			}
			else
			{
				size_t entCount = it.second.size();
				size_t chunks = entCount / task->SplitSize() + 1;
				
				for (size_t i = 0; i < chunks + 1; i++) {
					FreeFuncs_.push_back(Pool_->PushTask([i, chunks, it, task](size_t) {
						for (size_t j = it.second.size() / chunks * i; j < it.second.size() / chunks * (i + 1) && j < it.second.size(); j++)
						{
							Entity ent = it.second[j];
							task->SetEntity(&ent);
							task->Update();
						}
					}));
					std::this_thread::sleep_for(std::chrono::nanoseconds(1));
				}
			}
		}

		for (auto & it : Tasks_)
		{
			auto task = it.first;
			if (task->ShouldRunOnMain())
			{
				for (auto ent : it.second)
				{
					task->SetEntity(&ent);
					task->LateUpdate();
				}
			}
			else
			{
				size_t entCount = it.second.size();
				size_t chunks = entCount / task->SplitSize() + 1;

				for (size_t i = 0; i < chunks + 1; i++) {
					FreeFuncs_.push_back(Pool_->PushTask([i, chunks, it, task](size_t) {
						for (size_t j = it.second.size() / chunks * i; j < it.second.size() / chunks * (i + 1) && j < it.second.size(); j++)
						{
							Entity ent = it.second[j];
							task->SetEntity(&ent);
							task->LateUpdate();
						}
					}));
					std::this_thread::sleep_for(std::chrono::nanoseconds(1));
				}
			}
		}

		for (auto & func : FreeFuncs_)
		{
			func.get();
		}

		for (auto it = Tasks_.begin() ; it != Tasks_.end() ; )
		{
			if (it->first->IsComplete())
			{
				it = Tasks_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void TaskSystem::FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
		if (!FixedFuncs_.empty()) 
		{
			return;
		}

		for (auto & it : Tasks_)
		{
			auto task = it.first;
			if (task->ShouldRunOnMain())
			{
				for (auto ent : it.second)
				{
					task->SetEntity(&ent);
					task->FixedUpdate();
				}
			}
			else
			{
				size_t entCount = it.second.size();
				size_t chunks = entCount / task->SplitSize() + 1;

				for (size_t i = 0; i < chunks + 1; i++) {
					FixedFuncs_.push_back(Pool_->PushTask([i, chunks, it, task](size_t) {
						for (size_t j = it.second.size() / chunks * i; j < it.second.size() / chunks * (i + 1) && j < it.second.size(); j++)
						{
							Entity ent = it.second[j];
							task->SetEntity(&ent);
							task->FixedUpdate();
						}
					}));
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			}
		}

		for (auto & func : FixedFuncs_)
		{
			func.get();
		}

		for (auto it = Tasks_.begin(); it != Tasks_.end(); )
		{
			if (it->first->IsComplete())
			{
				it = Tasks_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void TaskSystem::AddEntity(std::shared_ptr<ITask> Task, Entity Ent)
	{
		auto it = Tasks_.find(Task);
		if (it != Tasks_.end())
		{
			it->second.push_back(Ent);
		}
	}

} }