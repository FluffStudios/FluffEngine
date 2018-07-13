#pragma once

#include <core/ecs/system.h>
#include <core/ecs/entity.h>
#include <core/ecs/event_manager.h>
#include <core/ecs/ecs_manager.h>
#include <rendering/renderable_component.h>
#include <core/transformation_component.h>
#include <gfx/context.h>
#include <gfx/frame_buffer.h>
#include <gfx/model_submit_event.h>
#include <core/core.h>
#include <rendering/scene_lighting.h>

namespace fluff { namespace render {

	class SceneRenderer : public ecs::System<SceneRenderer>, public ecs::Receiver<SceneRenderer>
	{
	private:
		struct ViewProj
		{
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
		private:
			glm::mat4 buffer[2];
		};

		enum STATE_CHANGE
		{
			NONE,
			MODL,
			MATL,
			PIPE
		};

		std::vector<ecs::ID> Entities_;
		std::vector<STATE_CHANGE> StateChanges_;
		std::vector<size_t> RenderableIDs_;

		std::vector<ecs::ID> TEntities_;
		std::vector<STATE_CHANGE> TStateChanges_;
		std::vector<size_t> TRenderableIDs_;

		gfx::FrameBuffer * DeferredPass_;
		gfx::FrameBuffer * ForwardPass_;
		gfx::FrameBuffer * PBuffer_;

		Material * DeferredMaterial_;
		gfx::Model *	DeferredModel_;

		glm::mat4 ProjectionMatrix_;
		ViewProj ViewProjMatrix_;
		gfx::ShaderBuffer * ViewProjBuffer_;
		gfx::ShaderBuffer * LightingBuffer_;
		SceneLighting SceneLights_;
		
		std::shared_ptr<ecs::Entity> Cam;

		Uniform * camera_position;
		std::shared_ptr<ECSManager> & Manager_;
	public:
		/*
			Creates new Scene Renderer

			ProjectionMatrix - Matrix computing linear transformation from view space to screenspace
		*/
		SceneRenderer(std::shared_ptr<ECSManager> & Manager, glm::mat4& ProjectionMatrix = glm::mat4(1.0));

		/*
			Creates new Scene Renderer

			Cam - Camera to calculate viewport for
			ProjectionMatrix - Matrix computing linear transformation from view space to screenspace
		*/
		SceneRenderer(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4& ProjectionMatrix);

		/*
			Destructor
		*/
		~SceneRenderer();
		
		/*
			Configures event listeners

			Events - Event manager to subscribe with
		*/
		void Configure(std::shared_ptr<ecs::EventManager> & Events) override;
		
		/*
			Reaction to a model submitted event

			ModelSubmission - Event to react to
		*/
		void Receive(const gfx::ModelSubmittedEvent & ModelSubmission);

		/*
			Reaction to entity destroyed event

			EntityDestroyed - Event to react to
		*/
		void Receive(const ecs::EntityDestroyedEvent & EntityDestroyed);

		/*
			Reaction to renderable component removed event

			RenderableComponent - Event to react to
		*/
		void Receive(const ecs::ComponentRemovedEvent<RenderableComponent> & RenderableComponent);

		/*
			Renders the scene
		*/
		void Render();

		Material * GetDeferredMaterial() const { return DeferredMaterial_; }

		inline void SetProjectionMatrix(const glm::mat4& ProjectionMatrix)
		{
			ViewProjMatrix_.ProjectionMatrix = ProjectionMatrix;
		}
	private:
		void DeferredPass();
		void LightingPass();
		void TransparencyPass();
		std::vector<ecs::ID> ClosestLights(IVec3 & LightCount);
	};

} }