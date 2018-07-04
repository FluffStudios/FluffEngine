#pragma once

#include <core/ecs/system.h>
#include <physics/physics_manager.h>
#include <physics/physics_scene.h>

#include <glm/vec3.hpp>

namespace fluff { namespace physics {

	class PhysicsSystem : public ecs::System<PhysicsSystem>, public ecs::Receiver<PhysicsSystem>
	{
		PhysicsScene * Scene_;
		PhysicsManager * pManager_;
		std::shared_ptr<ECSManager> & SystemsManager_;
	public:
		PhysicsSystem(std::shared_ptr<ECSManager> & Manager);
		~PhysicsSystem();
		void Configure(ecs::EntityManager & Entities, ecs::EventManager & Events) override;
		void Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double DeltaTime) override;
		void FixedUpdate(ecs::EntityManager & Entities, ecs::EventManager & Events) override;

		void * GetSDK() const { return pManager_->GetSDK(); }
		PhysicsScene * GetScene() const { return Scene_; }
		PhysicsManager * GetManager() const { return pManager_; }
	};

} }