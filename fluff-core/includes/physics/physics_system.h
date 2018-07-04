#pragma once

#include <core/ecs/system.h>
#include <physics/physics_manager.h>
#include <physics/physics_scene.h>

#include <glm/vec3.hpp>
#include <common.h>

namespace fluff { namespace physics {

	class PhysicsSystem : public ecs::System<PhysicsSystem>, public ecs::Receiver<PhysicsSystem>
	{
		PhysicsScene * Scene_;
		PhysicsManager * pManager_;
		std::shared_ptr<ECSManager> & SystemsManager_;
	public:
		FLUFF_API PhysicsSystem(std::shared_ptr<ECSManager> & Manager);
		FLUFF_API ~PhysicsSystem();
		void FLUFF_API Configure(ecs::EntityManager & Entities, ecs::EventManager & Events) override;
		void FLUFF_API Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double DeltaTime) override;
		void FLUFF_API FixedUpdate(ecs::EntityManager & Entities, ecs::EventManager & Events) override;

		void FLUFF_API * GetSDK() const { return pManager_->GetSDK(); }
		PhysicsScene FLUFF_API * GetScene() const { return Scene_; }
		PhysicsManager FLUFF_API * GetManager() const { return pManager_; }
	};

} }