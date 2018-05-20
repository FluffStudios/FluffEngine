#pragma once

#include <core/ecs/ecs_manager.h>
#include <ui/iui_component.h>
#include <glm/vec2.hpp>

#include <vector>

namespace luminos { namespace ui {

	struct IWidget
	{
		glm::ivec2 Position;
		glm::ivec2 Scale;
		size_t LayerID;

		/*
			Draws widget to layer

			ctx - Context to draw with
		*/
		virtual void Draw(nk_context * ctx) = 0;
	};

} }