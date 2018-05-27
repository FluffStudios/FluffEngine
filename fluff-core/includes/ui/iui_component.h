#pragma once

#include <core/transformation_component.h>
#include <ui/nk_ui.h>

namespace luminos { namespace ui {

	struct IUIComponent
	{
		/*
			Draw component

			ctx - Context to draw with
		*/
		virtual void Draw(nk_context * ctx) = 0;
	};

} }