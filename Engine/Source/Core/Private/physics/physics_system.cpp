#include <physics/physics_system.h>
#include <core/debug/debug_message.h>
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
		: Scene_(nullptr), SystemsManager_(Manager), Ready_(true)
	{	
	}

	PhysicsSystem::~PhysicsSystem()
	{
		delete pManager_;
	}

	void PhysicsSystem::Configure(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events)
	{
		pManager_ = new PhysicsManager;

		PhysicsSceneDescriptor desc
		{
			glm::vec3(0, -9.8, 0),
			physx::PxDefaultCpuDispatcherCreate(4)
		};

		Scene_ = new PhysicsScene((physx::PxPhysics*)pManager_->GetSDK(), desc);

		if (!Scene_)
		{
			FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, "Could not create PhysX Scene.");
		}
		((physx::PxScene *) Scene_->GetScene())->simulate(1.0f / 30.0f);
	}

	void PhysicsSystem::Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double DeltaTime)
	{

	}

	void PhysicsSystem::FixedUpdate(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events)
	{
		if (((physx::PxScene *) Scene_->GetScene())->fetchResults(false))
		{
			for (auto & ent : Entities->GetEntitiesWithComponents<PhysicsComponent>())
			{
				ent.GetComponent<PhysicsComponent>()->GetActor()->Update();
			}

			((physx::PxScene *) Scene_->GetScene())->simulate(1.0f / 30.0f);
		}
	}

} }