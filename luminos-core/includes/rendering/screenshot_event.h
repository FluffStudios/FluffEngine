#pragma once

#include <common.h>
#include <core/ecs/event.h>

namespace luminos { namespace render {

	struct ScreenshotEvent : ecs::Event<ScreenshotEvent>
	{
		uint32_t Width;
		uint32_t Height;

		/*
			Triggers a screenshot

			Width - width of screenshot
			Height - height of screenshot
		*/
		ScreenshotEvent(uint32_t Width, uint32_t Height)
			: Width(Width), Height(Height) { }
	};

} }