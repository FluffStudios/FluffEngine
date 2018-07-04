#pragma once

#include <physics/physics_actor.h>
#include <glm/vec3.hpp>

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
		PhysicsScene(void * pSDK);
		PhysicsScene(void * pSDK, PhysicsSceneDescriptor Descriptor);

		void AddToScene(PhysicsActor * pActor);
		void RemoveFromScene(PhysicsActor * pActor);

		void * GetScene() const;
	};

} }