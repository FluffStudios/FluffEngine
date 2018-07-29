#include <rendering/render_system.h>
#include "rendering/ui/backend_imgui_wrapper.h"

namespace fluff { namespace render {
	RenderSystem::RenderSystem(std::shared_ptr<ECSManager> & Manager, ecs::Entity Cam, glm::mat4 & ProjectionMatrix)
	{
		Scene_ = Manager->GetSystemManager()->Add<SceneRenderer>(Manager, Cam, ProjectionMatrix);
	}

	void RenderSystem::Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta)
	{
        // Update imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		Scene_->Render();
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
} }
