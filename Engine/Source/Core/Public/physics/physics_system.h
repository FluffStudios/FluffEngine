#pragma once

#include <core/ecs/system.h>
#include <physics/physics_manager.h>
#include <physics/physics_scene.h>

#include <mutex>

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
		void Configure(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events) override;
		void Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double DeltaTime) override;
		void FixedUpdate(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events) override;
		inline bool UseMainThread() override { return true; }

		void * GetSDK() const { return pManager_->GetSDK(); }
		PhysicsScene * GetScene() const { return Scene_; }
		PhysicsManager * GetManager() const { return pManager_; }
	};

} }