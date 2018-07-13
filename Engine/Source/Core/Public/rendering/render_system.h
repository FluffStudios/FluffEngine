#pragma once

#include <rendering/scene_renderer.h>
#include <core/ecs/ecs_manager.h>

namespace fluff { namespace render {

	class RenderSystem : public ecs::System<RenderSystem>
	{
		std::shared_ptr<SceneRenderer> Scene_;
	public:
		/*
			Deleted default contstructor
		*/
		RenderSystem() = delete;

		/*
			Creates new Render System

			Cam - Viewpoint of scene
			ProjectionMatrix - Performs viewspace to screenspace transform
		*/
		RenderSystem(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4 & ProjectionMatrix);

		/*
			Updates the scene

			EntityManager - EntityManager associated with render system
			EventManager - EventManager associated with render system
			Delta - time since last update
		*/
		void Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta) override;

		inline bool UseMainThread() override { return true; }

		std::shared_ptr<SceneRenderer> & GetSceneRenderer() { return Scene_; }
	};

} }