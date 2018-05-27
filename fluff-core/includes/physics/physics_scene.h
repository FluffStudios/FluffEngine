#pragma once

#include <Physx/PxScene.h>
#include <Physx/PxSceneDesc.h>

#include <Physx/extensions/PxDefaultCpuDispatcher.h>

#include <physics/physics_actor.h>

#include <glm/vec3.hpp>

namespace luminos { namespace physics {

	struct PhysicsSceneDescriptor
	{
		glm::vec3 Gravity = glm::vec3(0, -1, 0);
		physx::PxCpuDispatcher * CPUDispatch = physx::PxDefaultCpuDispatcherCreate(1);
	};

	class PhysicsScene
	{
		physx::PxScene * Scene_;
	public:
		PhysicsScene(physx::PxPhysics * pSDK);
		PhysicsScene(physx::PxPhysics * pSDK, PhysicsSceneDescriptor Descriptor);

		void AddToScene(PhysicsActor * pActor);
		void RemoveFromScene(PhysicsActor * pActor);

		physx::PxScene * GetScene() const {
			return Scene_;
		}
	};

} }