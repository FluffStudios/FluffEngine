#include <rendering/render_system.h>

namespace fluff { namespace render {
	RenderSystem::RenderSystem(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4 & ProjectionMatrix)
	{
		Manager->GetSystemManager()->Add<SceneRenderer>(Manager, Cam, ProjectionMatrix);

		Scene_ = Manager->GetSystemManager()->GetSystem<SceneRenderer>();
	}

	void RenderSystem::Update(ecs::EntityManager & EntityManager, ecs::EventManager & EventManager, double Delta)
	{
		Scene_->Render();
	}

} }