#pragma once

#include <physics/physics_allocator.h>
#include <physics/physics_error_callback.h>

#include <Physx/foundation/PxFoundation.h>
#include <Physx/foundation/PxErrorCallback.h>
#include <Physx/extensions/PxDefaultAllocator.h>
#include <Physx/extensions/PxDefaultErrorCallback.h>
#include <Physx/PxPhysics.h>
#include <Physx/pvd/PxPvd.h>
#include <Physx/pvd/PxPvdTransport.h>
#include <Physx/PxPhysics.h>
#include <Physx/task/PxCpuDispatcher.h>
#include <Physx/cooking/PxCooking.h>

#include <core/ecs/ecs_manager.h>
#include <memory>

#ifndef PVD_HOST
#define PVD_HOST "127.0.0.1"
#endif

namespace luminos { namespace physics {

	class PhysicsManager
	{
		physx::PxDefaultErrorCallback DefaultErrorCallback_;
		physx::PxDefaultAllocator DefaultAllocatorCallback_;
		physx::PxFoundation * Foundation_;
		physx::PxPhysics * Physics_;
		physx::PxPvd * PVD_;
		physx::PxCooking * Cooking_;
		physx::PxCpuDispatcher * CPUDispatch_;
		std::shared_ptr<ECSManager> & Manager_;
	public:
		PhysicsManager(std::shared_ptr<ECSManager> & Manager);
		physx::PxDefaultErrorCallback * GetErrorCallback() { return &(DefaultErrorCallback_); }
		physx::PxDefaultAllocator * GetAllocatorCallback() { return &(DefaultAllocatorCallback_); }
		physx::PxCpuDispatcher * GetCPUDispatcher() { return CPUDispatch_; }
		physx::PxPhysics * GetSDK() { return Physics_;  }
		physx::PxCooking * GetCooking() { return Cooking_; }
	};

} }