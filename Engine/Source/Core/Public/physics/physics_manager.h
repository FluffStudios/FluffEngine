#pragma once

#include <core/ecs/ecs_manager.h>
#include <memory>

namespace fluff { namespace physics {

	class PhysicsManager
	{
		struct PhysicsManagerImpl;
		PhysicsManagerImpl * Impl_;
	public:
		PhysicsManager();
		~PhysicsManager();
		void * GetErrorCallback();
		void * GetAllocatorCallback();
		void * GetCPUDispatcher();
		void * GetSDK();
		void * GetCooking();
	};

} }