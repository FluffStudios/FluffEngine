#include <physics/physics_scene.h>
#include <Physx/extensions/PxDefaultSimulationFilterShader.h>

namespace luminos { namespace physics {

	PhysicsScene::PhysicsScene(physx::PxPhysics * pSDK)
		: PhysicsScene(pSDK, PhysicsSceneDescriptor())
	{	}

	PhysicsScene::PhysicsScene(physx::PxPhysics * pSDK, PhysicsSceneDescriptor Descriptor)
	{
		physx::PxTolerancesScale tolerances;
		physx::PxSceneDesc scene_desc(tolerances);

		scene_desc.gravity = physx::PxVec3(Descriptor.Gravity.x, Descriptor.Gravity.y, Descriptor.Gravity.z);
		scene_desc.cpuDispatcher = Descriptor.CPUDispatch;
		scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;

		Scene_ = pSDK->createScene(scene_desc);
	}

	void PhysicsScene::AddToScene(PhysicsActor * pActor)
	{
		pActor->AddToScene(Scene_);
	}

	void PhysicsScene::RemoveFromScene(PhysicsActor * pActor)
	{
		pActor->RemoveFromScene(Scene_);
	}

} }