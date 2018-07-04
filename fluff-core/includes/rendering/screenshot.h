#pragma once

#include <rendering/screenshot_event.h>
#include <core/ecs/ecs_manager.h>
#include <core/window.h>

namespace fluff { namespace render {

	struct Screenshot
	{
		/*
			Creates a new screenshot
		*/
		FLUFF_API Screenshot(std::shared_ptr<ECSManager> & Manager)
		{
			Manager->GetEventManager()->EmitEvent<ScreenshotEvent>(Window::GetCurrentWindow()->GetWidth().GetValue(), Window::GetCurrentWindow()->GetHeight().GetValue());
		}
	};

} }