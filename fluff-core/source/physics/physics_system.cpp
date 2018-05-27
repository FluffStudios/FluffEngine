#include <physics/physics_system.h>
#include <core/debug_message.h>
#include <Physx/extensions/PxDefaultSimulationFilterShader.h>
#include <physics/physics_component.h>
#include <core/transformation_component.h>
#include <core/timer.h>

namespace luminos { namespace physics {

	PhysicsSystem::PhysicsSystem(std::shared_ptr<ECSManager> & Manager)
		: Scene_(nullptr), SystemsManager_(Manager)
	{	
		pManager_ = new PhysicsManager(Manager);
	}

	void PhysicsSystem::Configure(ecs::EntityManager & Entities, ecs::EventManager & Events)
	{
		PhysicsSceneDescriptor desc
		{
			glm::vec3(0, -9.8, 0),
			physx::PxDefaultCpuDispatcherCreate(2)
		};

		Scene_ = new PhysicsScene(pManager_->GetSDK(), desc);

		if (!Scene_)
		{
			DebugMessage(SystemsManager_, FATAL, "Could not create scene!");
		}
		Scene_->GetScene()->simulate(0.0);
	}

	void PhysicsSystem::Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double DeltaTime)
	{
		for (auto & ent : Entities.GetEntitiesWithComponents<PhysicsComponent, TransformationComponent>())
		{
			auto physics_handle = ent.GetComponent<PhysicsComponent>();

			ent.GetComponent<TransformationComponent>()->SetPosition(ent.GetComponent<TransformationComponent>()->GetPosition() + (float) DeltaTime * physics_handle->GetActor()->GetLinearVelocity());
			ent.GetComponent<TransformationComponent>()->SetRotation(ent.GetComponent<TransformationComponent>()->GetRotation() + (float) DeltaTime * physics_handle->GetActor()->GetAngularVelocity());
		}
	}

	void PhysicsSystem::FixedUpdate(ecs::EntityManager & Entities, ecs::EventManager & Events)
	{

		Scene_->GetScene()->fetchResults(true);

		for (auto & ent : Entities.GetEntitiesWithComponents<PhysicsComponent, TransformationComponent>())
		{
			auto physics_handle = ent.GetComponent<PhysicsComponent>();

			ent.GetComponent<TransformationComponent>()->SetPosition(physics_handle->GetActor()->GetPosition());
			ent.GetComponent<TransformationComponent>()->SetRotation(physics_handle->GetActor()->GetRotation());
		}

		Scene_->GetScene()->simulate(1.0 / 30.0f);
	}

	physx::PxRigidDynamic * PhysicsSystem::CreateRigidDynamic(const glm::vec3 & Position) const
	{
		return pManager_->GetSDK()->createRigidDynamic(physx::PxTransform(physx::PxVec3(Position.x, Position.y, Position.z)));
	}

	physx::PxMaterial * PhysicsSystem::CreateMaterial(const float StaticFriction, const float DynamicFriction, const float Restitution) const
	{
		return pManager_->GetSDK()->createMaterial(StaticFriction, DynamicFriction, Restitution);
	}

} }