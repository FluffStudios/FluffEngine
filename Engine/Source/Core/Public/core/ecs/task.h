#pragma once

#include <core/ecs/itask.h>

namespace fluff { namespace ecs {

	template <typename TaskType>
	class Task : public ITask
	{
		friend class TaskSystem;

		static size_t GetFamilyID()
		{
			static size_t family_id = FamilyIDCounter++;
			return family_id;
		}
	public:
		inline virtual ~Task() 
		{	};
	};

} }

#define REGISTER_TASK(TaskName) REGISTER_POLYMORPHIC_CLASS_CRTP(fluff::ecs::ITask, fluff::ecs::Task<TaskName>, TaskName)