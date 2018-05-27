#pragma once

#include <ui/nk_ui.h>
#include <core/ecs/ecs_manager.h>
#include <ui/ui_layer.h>

#ifndef UI_MAX_VERTEX_MEM
#define UI_MAX_VERTEX_MEM 1024 * 512
#endif

#ifndef UI_MAX_ELEMENT_MEM
#define UI_MAX_ELEMENT_MEM 1024 * 128
#endif

namespace luminos { namespace render {

	class UIRenderer : public ecs::System<UIRenderer>, public ecs::Receiver<UIRenderer>
	{
		std::vector<ui::UILayer> ActiveLayers_;
		struct nk_context * ctx_;
		struct nk_font_atlas * font_atlas_;
		std::shared_ptr<ECSManager> & Manager_;
	public:
		UIRenderer(std::shared_ptr<ECSManager> & Manager)
			: Manager_(Manager)
		{	}
		/*
			Configures listener for UILayer enable events

			Events - Event Manager to listen to
		*/
		void Configure(ecs::EventManager & Events) override;

		/*
			Receives UI Layer enable evets

			Events - Event to listen to
		*/
		void Receive(const ui::UILayerEnabledEvent & Event);

		/*
			Render active UI Layers
		*/
		void Render();
	};

} }