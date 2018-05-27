#pragma once

#include <rendering/scene_renderer.h>
#include <rendering/ui_renderer.h>

#include <core/ecs/ecs_manager.h>

namespace luminos { namespace render {

	class RenderSystem : public ecs::System<RenderSystem>
	{
		std::shared_ptr<SceneRenderer> Scene_;
		std::shared_ptr<UIRenderer> UI_;
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
		void Update(ecs::EntityManager & EntityManager, ecs::EventManager & EventManager, double Delta) override;

		std::shared_ptr<SceneRenderer> & GetSceneRenderer() { return Scene_; }
		std::shared_ptr<UIRenderer> & GetUIRenderer() { return UI_; }
	};

} }