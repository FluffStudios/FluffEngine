#include <physics/physics_scene.h>
#include <Physx/extensions/PxDefaultSimulationFilterShader.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxScene.h>
#include <Physx/PxSceneDesc.h>

namespace fluff { namespace physics {

	struct PhysicsScene::PhysicsSceneImpl
	{
		physx::PxScene * Scene;
	};

	PhysicsScene::PhysicsScene(void * pSDK)
		: PhysicsScene(pSDK, PhysicsSceneDescriptor())
	{	}

	PhysicsScene::PhysicsScene(void * pSDK, PhysicsSceneDescriptor Descriptor)
		: Impl_(new PhysicsScene::PhysicsSceneImpl)
	{
		physx::PxTolerancesScale tolerances;
		physx::PxSceneDesc scene_desc(tolerances);

		scene_desc.gravity = physx::PxVec3(Descriptor.Gravity.x, Descriptor.Gravity.y, Descriptor.Gravity.z);
		scene_desc.cpuDispatcher = (physx::PxCpuDispatcher*) Descriptor.CPUDispatch;
		scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;

		Impl_->Scene = ((physx::PxPhysics*)pSDK)->createScene(scene_desc);
	}

	void PhysicsScene::AddToScene(PhysicsActor * pActor)
	{
		Impl_->Scene->addActor(*((physx::PxActor *)pActor->GetPointer()));
	}

	void PhysicsScene::RemoveFromScene(PhysicsActor * pActor)
	{
		Impl_->Scene->removeActor(*((physx::PxActor *)pActor->GetPointer()));
	}

	void * PhysicsScene::GetScene() const
	{
		return Impl_->Scene;
	}

} }