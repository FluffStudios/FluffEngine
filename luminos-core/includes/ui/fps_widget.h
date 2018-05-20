#pragma once

#include <ui/widget.h>
#include <ui/components/label_component.h>

#include <core/timer.h>
#include <ui/layout.h>

namespace luminos { namespace ui {

	class FPSWidget : public IWidget
	{
		Group Layout_;
		TextLabelComponent FPSLabel_;
		TextLabelComponent FPSCounter_;
		Row Row_;
	public:
		/*
			Creates widget displaying FPS
		*/
		FPSWidget();

		/*
			Draws widget

			ctx - Context for drawing
		*/
		void Draw(nk_context * ctx) override;
	};

} }