#pragma once

#include <core/ecs/ecs_manager.h>
#include <memory>

namespace fluff { namespace physics {

	class PhysicsManager
	{
		struct PhysicsManagerImpl;
		PhysicsManagerImpl * Impl_;
	public:
		FLUFF_API PhysicsManager();
		FLUFF_API ~PhysicsManager();
		void FLUFF_API * GetErrorCallback();
		void FLUFF_API * GetAllocatorCallback();
		void FLUFF_API * GetCPUDispatcher();
		void FLUFF_API * GetSDK();
		void FLUFF_API * GetCooking();
	};

} }