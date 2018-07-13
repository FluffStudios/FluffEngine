#include <rendering/render_system.h>

namespace fluff { namespace render {
	RenderSystem::RenderSystem(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4 & ProjectionMatrix)
	{
		Scene_ = Manager->GetSystemManager()->Add<SceneRenderer>(Manager, Cam, ProjectionMatrix);
	}

	void RenderSystem::Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta)
	{
		Scene_->Render();
	}

} }