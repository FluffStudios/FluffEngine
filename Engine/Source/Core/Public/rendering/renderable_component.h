#pragma once

#include <rendering/renderable.h>
#include <vector>
#include <core/ecs/entity.h>
#include <common.h>

namespace fluff { namespace render { 

	struct RenderableComponent : public ecs::Component<RenderableComponent>
	{
		std::vector<Renderable> renderables;
		FLUFF_API RenderableComponent() {}
		FLUFF_API RenderableComponent(Renderable & renderable) { renderables.push_back(std::move(renderable)); }
	};

} }