#pragma once

#include <physics/physics_actor.h>
#include <glm/vec3.hpp>
#include <common.h>

namespace fluff { namespace physics {

	struct PhysicsSceneDescriptor
	{
		glm::vec3 Gravity = glm::vec3(0, -1, 0);
		void * CPUDispatch;
	};

	class PhysicsScene
	{
		struct PhysicsSceneImpl;
		std::unique_ptr<PhysicsSceneImpl> Impl_;

	public:
		FLUFF_API PhysicsScene(void * pSDK);
		FLUFF_API PhysicsScene(void * pSDK, PhysicsSceneDescriptor Descriptor);

		void FLUFF_API AddToScene(PhysicsActor * pActor);
		void FLUFF_API RemoveFromScene(PhysicsActor * pActor);

		void FLUFF_API * GetScene() const;
	};

} }