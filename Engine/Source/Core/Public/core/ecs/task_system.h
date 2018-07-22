#pragma once

#include <core/ecs/isystem.h>
#include <core/ecs/system.h>
#include <core/ecs/itask.h>
#include <core/ecs/task.h>

#include <core/debug/debug_log_writer.h>

#include <unordered_map>
#include <vector>

#include <cereal/access.hpp>

namespace fluff { namespace ecs {

	class TaskSystem final : public System<TaskSystem>
	{
		std::vector<std::future<void>> FreeFuncs_;
		std::vector<std::future<void>> FixedFuncs_;
		std::unordered_map<size_t, std::shared_ptr<ITask>> TaskTypes_;
		std::unordered_map<std::shared_ptr<ITask>, std::vector<Entity>> Tasks_;
		std::shared_ptr<threading::ThreadPool> Pool_;

		friend class cereal::access;

		template <typename Archive>
		void save(Archive & Ar) const
		{
			// ID -> ITask ptr
			// ID -> Entity vector
			std::unordered_map<size_t, std::vector<Entity>> TasksToEnts_;
			for (auto out_it : TaskTypes_) {
				size_t id = out_it.first;
				auto ents = Tasks_.find(out_it.second)->second;
				TasksToEnts_.insert(std::make_pair(id, ents));
			}
			Ar(TaskTypes_, TasksToEnts_);
		}

		template <typename Archive>
		void load(Archive & Ar)
		{
			std::unordered_map<size_t, std::vector<Entity>> TasksToEnts_;
			Ar(TaskTypes_, TasksToEnts_);
			for (auto it : TaskTypes_)
			{
				size_t id = it.first;
				auto ents = TasksToEnts_.find(id)->second;
				Tasks_.insert(std::make_pair(it.second, ents));
			}
		}
	public:
		FLUFF_API TaskSystem();
		void FLUFF_API Configure(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events) override;
		void FLUFF_API Update(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events, double DeltaTime) override;
		void FLUFF_API FixedUpdate(std::shared_ptr<EntityManager> & Entities, std::shared_ptr<EventManager> & Events) override;
		
		template <typename TaskType, typename ... Arguments>
		std::shared_ptr<ITask> Add(Arguments && ... Args);

		template <typename TaskType>
		std::shared_ptr<ITask> & GetTask();
		
		void FLUFF_API AddEntity(std::shared_ptr<ITask> Task, Entity Ent);

		template <typename TaskType>
		void AddEntity(Entity Ent);

		inline void AttachThreadpool(const std::shared_ptr<threading::ThreadPool> & Threads)
		{
			Pool_ = Threads;
		}
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
			FLUFF_LOG(debug::DebugErrorType::INVALID_PARAMETER, debug::DebugSeverity::ERROR, "Invalid Task Type");
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