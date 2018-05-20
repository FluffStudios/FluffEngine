#include <rendering/ui_renderer.h>
#include <core/ecs/dependency.h>
#include <core/window.h>
#include <ui/nk_ui.h>

namespace luminos { namespace render {

	void UIRenderer::Configure(ecs::EventManager & Events)
	{
		ctx_ = ui::nk_glfw3_init(Manager_, Window::GetCurrentWindow()->GetHandle());
		ui::nk_glfw3_font_stash_begin(&font_atlas_);
		nk_font * open_sans = nk_font_atlas_add_from_file(font_atlas_, "OpenSans-Regular.ttf", 18, 0);
		font_atlas_->fonts[0].scale = 2.0f;
		ui::nk_glfw3_font_stash_end();
		nk_style_set_font(ctx_, &open_sans->handle);

		nk_style * style = &ctx_->style;
		style->window.fixed_background.data.color.a = 0;

		Events.SubscribeToEvent<ui::UILayerEnabledEvent>(*this);
	}

	void UIRenderer::Receive(const ui::UILayerEnabledEvent & Event)
	{
		ActiveLayers_.push_back(Event.Layer);
	}

	void UIRenderer::Render()
	{
		ui::nk_glfw3_new_frame();
		if (nk_begin(ctx_, "UI", nk_rect(0, 0, Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue()), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND))
		{
			for (auto layer : ActiveLayers_)
			{
				layer.Draw(ctx_);
			}
			nk_end((nk_context *)ctx_);
		}
		ui::nk_glfw3_render(UI_MAX_VERTEX_MEM, UI_MAX_ELEMENT_MEM);
	}

} }