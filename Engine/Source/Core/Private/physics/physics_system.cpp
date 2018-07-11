#include <physics/physics_system.h>
#include <core/debug_message.h>
#include <Physx/extensions/PxDefaultSimulationFilterShader.h>
#include <physics/physics_component.h>
#include <core/transformation_component.h>
#include <core/timer.h>
#include <Physx/extensions/PxDefaultCpuDispatcher.h>
#include <Physx/PxScene.h>
#include <Physx/PxPhysics.h>
#include <thread>

namespace fluff { namespace physics {

	PhysicsSystem::PhysicsSystem(std::shared_ptr<ECSManager> & Manager)
		: Scene_(nullptr), SystemsManager_(Manager)
	{	
	}

	PhysicsSystem::~PhysicsSystem()
	{
		delete pManager_;
	}

	void PhysicsSystem::Configure(ecs::EntityManager & Entities, ecs::EventManager & Events)
	{
		pManager_ = new PhysicsManager;

		PhysicsSceneDescriptor desc
		{
			glm::vec3(0, -9.8, 0),
			physx::PxDefaultCpuDispatcherCreate(2)
		};

		Scene_ = new PhysicsScene((physx::PxPhysics*)pManager_->GetSDK(), desc);

		if (!Scene_)
		{
			DebugMessage(SystemsManager_, FATAL, "Could not create scene!");
		}
		((physx::PxScene *) Scene_->GetScene())->simulate(1.0f / 30.0f);
	}

	void PhysicsSystem::Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double DeltaTime)
	{
		for (auto & ent : Entities.GetEntitiesWithComponents<PhysicsComponent, TransformationComponent>())
		{
			auto physics_handle = ent.GetComponent<PhysicsComponent>();

			ent.GetComponent<TransformationComponent>()->SetPosition(ent.GetComponent<TransformationComponent>()->GetPosition() + 
                static_cast<float>(DeltaTime) * physics_handle->GetActor()->GetLinearVelocity());

			ent.GetComponent<TransformationComponent>()->SetRotation(ent.GetComponent<TransformationComponent>()->GetRotation() + 
                static_cast<float>(DeltaTime) * physics_handle->GetActor()->GetAngularVelocity());
		}
	}

	void PhysicsSystem::FixedUpdate(ecs::EntityManager & Entities, ecs::EventManager & Events)
	{
		((physx::PxScene *) Scene_->GetScene())->fetchResults(true);

		for (auto & ent : Entities.GetEntitiesWithComponents<PhysicsComponent, TransformationComponent>())
		{
			auto physics_handle = ent.GetComponent<PhysicsComponent>();

			ent.GetComponent<TransformationComponent>()->SetPosition(physics_handle->GetActor()->GetPosition());
			ent.GetComponent<TransformationComponent>()->SetRotation(physics_handle->GetActor()->GetRotation());
		}

		((physx::PxScene *) Scene_->GetScene())->simulate(1.0f / 30.0f);
	}

} }