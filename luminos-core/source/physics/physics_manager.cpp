#include <physics/physics.h>
#include <Physx/foundation/PxFoundationVersion.h>

#include <core/debug_message.h>
#include <Physx/PxPhysicsVersion.h>
#include <Physx/common/PxTolerancesScale.h>
#include <Physx/extensions/PxDefaultCpuDispatcher.h>

namespace luminos {	namespace physics {

	PhysicsManager::PhysicsManager(std::shared_ptr<ECSManager> & Manager)
		: Manager_(Manager)
	{
		this->Foundation_ = PxCreateFoundation(PX_FOUNDATION_VERSION, DefaultAllocatorCallback_, DefaultErrorCallback_);
		if (Foundation_ == nullptr)
		{
			DebugMessage(Manager, FATAL, "PxCreateFoundation failed!");
		}

		PVD_ = physx::PxCreatePvd(*Foundation_);
		auto transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		PVD_->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		Physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation_, physx::PxTolerancesScale(), false, PVD_);

		CPUDispatch_ = physx::PxDefaultCpuDispatcherCreate(2);

		Cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *Foundation_, physx::PxCookingParams(physx::PxTolerancesScale()));
		if (Cooking_ == nullptr)
		{
			DebugMessage(Manager, FATAL, "PxCreateCooking failed!");
		}
	}

} }