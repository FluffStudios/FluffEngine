#pragma once

#include <rendering/renderable.h>
#include <vector>

namespace fluff { namespace render { 

	struct RenderableComponent
	{
		std::vector<Renderable> renderables;
		RenderableComponent() {}
		RenderableComponent(Renderable & renderable) { renderables.push_back(std::move(renderable)); }
	};

} }