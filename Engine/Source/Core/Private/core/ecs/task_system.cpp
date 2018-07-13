#include <core/ecs/task_system.h>

namespace fluff { namespace ecs {

	TaskSystem::TaskSystem(std::shared_ptr<threading::ThreadPool> & ThreadPool)
		: Pool_(ThreadPool)
	{	}

	void TaskSystem::Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{

	}

	void TaskSystem::Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime)
	{
		std::vector<std::future<void>> funcs;
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
					funcs.push_back(Pool_->PushTask([i, chunks, it, task](size_t) {
						for (size_t j = it.second.size() / chunks * i; j < it.second.size() / chunks * (i + 1) && j < it.second.size(); j++)
						{
							Entity ent = it.second[j];
							task->SetEntity(&ent);
							task->Update();
						}
					}));
					std::this_thread::sleep_for(std::chrono::microseconds(1));
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
					funcs.push_back(Pool_->PushTask([i, chunks, it, task](size_t) {
						for (size_t j = it.second.size() / chunks * i; j < it.second.size() / chunks * (i + 1) && j < it.second.size(); j++)
						{
							Entity ent = it.second[j];
							task->SetEntity(&ent);
							task->LateUpdate();
						}
					}));
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			}
		}

		for (auto & func : funcs)
		{
			func.get();
		}
	}

	void TaskSystem::FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events)
	{
		
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