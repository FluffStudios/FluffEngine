#pragma once

#include <core/ecs/isystem.h>
#include <core/ecs/system.h>
#include <core/ecs/itask.h>
#include <core/ecs/task.h>

#include <unordered_map>
#include <vector>

namespace fluff { namespace ecs {

	class TaskSystem final : public System<TaskSystem>
	{
		std::unordered_map<size_t, std::shared_ptr<ITask>> TaskTypes_;
		std::unordered_map<std::shared_ptr<ITask>, std::vector<Entity>> Tasks_;
		std::shared_ptr<threading::ThreadPool> & Pool_;
	public:
		TaskSystem(std::shared_ptr<threading::ThreadPool> & ThreadPool);
		void Configure(EntityManager & Entities, EventManager & Events) override;
		void Update(EntityManager & Entities, EventManager & Events, double DeltaTime) override;
		void FixedUpdate(EntityManager & Entities, EventManager & Events) override;
		
		template <typename TaskType, typename ... Arguments>
		std::shared_ptr<ITask> Add(Arguments && ... Args);

		template <typename TaskType>
		std::shared_ptr<ITask> & GetTask();
		
		void AddEntity(std::shared_ptr<ITask> Task, Entity Ent);

		template <typename TaskType>
		void AddEntity(Entity Ent);
	};

	template<typename TaskType, typename ...Arguments>
	inline std::shared_ptr<ITask> TaskSystem::Add(Arguments && ...Args)
	{
		std::shared_ptr<ITask> ptr = std::make_shared<TaskType>(std::forward(Arguments)...);
		std::vector<Entity> ents;
		Tasks_.insert(std::make_pair(ptr, ents));
		TaskTypes_.insert(std::make_pair(Task<TaskType>::GetFamilyID(), ptr));
		return ptr;
	}

	template<typename TaskType>
	inline std::shared_ptr<ITask> & TaskSystem::GetTask()
	{
		auto it = TaskTypes_.find(Task<TaskType>::GetFamilyID());
		if (it == TaskTypes_.end()) {
			return std::shared_ptr<ITask>(nullptr);
		}
		return it->second;
	}

	template<typename TaskType>
	inline void TaskSystem::AddEntity(Entity Ent)
	{
		std::shared_ptr<ITask> & task = GetTask<TaskType>();
		FLUFF_ASSERT(task.get())
		Tasks_.find(task)->second.push_back(Ent);
	}

}	}