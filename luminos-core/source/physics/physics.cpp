#include <physics/physics.h>
#include <Physx/foundation/PxFoundationVersion.h>

#include <core/debug_message.h>
#include <Physx/PxPhysicsVersion.h>
#include <Physx/common/PxTolerancesScale.h>

namespace luminos {	namespace physics {

	PhysicsManager::PhysicsManager(std::shared_ptr<ECSManager> & Manager)
		: Manager_(Manager)
	{
		this->Foundation_ = PxCreateFoundation(PX_FOUNDATION_VERSION, DefaultAllocatorCallback_, DefaultErrorCallback_);
		if (Foundation_ == nullptr)
		{
			DebugMessage(Manager, FATAL, "PxCreateFoundation failed!");
		}

		Physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation_, physx::PxTolerancesScale());
	}

} }