#pragma once

#include <rendering/renderable.h>
#include <vector>

namespace fluff { namespace render { 

	struct RenderableComponent
	{
		std::vector<Renderable> renderables;
		FLUFF_API RenderableComponent() {}
		FLUFF_API RenderableComponent(Renderable & renderable) { renderables.push_back(std::move(renderable)); }
	};

} }