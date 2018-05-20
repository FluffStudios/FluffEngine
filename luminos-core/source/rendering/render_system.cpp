#include <rendering/render_system.h>

namespace luminos { namespace render {
	RenderSystem::RenderSystem(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4 & ProjectionMatrix)
	{
		Manager->GetSystemManager()->Add<SceneRenderer>(Manager, Cam, ProjectionMatrix);
		Manager->GetSystemManager()->Add<UIRenderer>(Manager);

		Scene_ = Manager->GetSystemManager()->GetSystem<SceneRenderer>();
		UI_ = Manager->GetSystemManager()->GetSystem<UIRenderer>();
	}

	void RenderSystem::Update(ecs::EntityManager & EntityManager, ecs::EventManager & EventManager, double Delta)
	{
		Scene_->Render();
		UI_->Render();
	}

} }