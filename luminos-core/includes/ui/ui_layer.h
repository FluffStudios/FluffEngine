#pragma once

#include <ui/widget.h>
#include <vector>
#include <core/ecs/ecs_manager.h>

namespace luminos { namespace ui {

	struct UILayer : public ecs::Component<UILayer>
	{
		/*
			Creates new UI Layer
		*/
		UILayer() { LayerID = ID++; }
		std::vector<IWidget *> Widgets;

		/*
			Draws the UI layer

			ctx - Context to draw with
		*/
		void Draw(nk_context * ctx) const;
		
		/*
			Enables the UI layer
		*/
		void Enable(std::shared_ptr<ECSManager> Manager) const;

		size_t LayerID;
	private:
		static size_t ID;
	};

	struct UILayerEnabledEvent : public ecs::Event<UILayerEnabledEvent>
	{
		UILayer Layer;

		/*
			Enables UI layer

			Layer - Layer to render
		*/
		UILayerEnabledEvent(UILayer Layer) : Layer(Layer) { }
	};

} }