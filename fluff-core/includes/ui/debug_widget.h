#pragma once

#include <common.h>
#include <ui/widget.h>
#include <ui/layout.h>
#include <ui/components/label_component.h>

namespace luminos { namespace ui {

	class LUMINOS_API DebugWidget : public IWidget
	{
		Group Layout_;
		
		TextLabelComponent FPSLabel_;
		TextLabelComponent FPSValue_;
		TextLabelComponent FrameTimeValue_;
		Row FPS_;

		TextLabelComponent RendererLabel_;
		TextLabelComponent RendererValue_;
		Row Renderer_;

		TextLabelComponent VendorLabel_;
		TextLabelComponent VendorValue_;
		Row Vendor_;

		TextLabelComponent VersionLabel_;
		TextLabelComponent VersionValue_;
		Row Version_;
	public:
		/*
			Creates widget showing OpenGL status
			and frametime information
		*/
		DebugWidget();

		/*
			Draws widget on UI layer

			ctx - Nuklear context
		*/
		void Draw(nk_context * ctx) override;
	};

} }