#include <ui/debug_widget.h>

#include <core/timer.h>
#include <glew.h>

namespace luminos { namespace ui {

	DebugWidget::DebugWidget()
	{
		FPS_.State = STATIC;
		Renderer_.State = STATIC;
		Vendor_.State = STATIC;
		Version_.State = STATIC;

		FPSLabel_.SetText("FPS: ");

		RowComponent fps_label_rc;
		fps_label_rc.ElementWidth = 75;
		fps_label_rc.RComponent = &FPSLabel_;

		RowComponent fps_value_rc;
		fps_value_rc.ElementWidth = 100;
		fps_value_rc.RComponent = &FPSValue_;

		RowComponent frametime_value_rc;
		frametime_value_rc.ElementWidth = 100;
		frametime_value_rc.RComponent = &FrameTimeValue_;

		FPS_.Components = {
			fps_label_rc,
			fps_value_rc,
			frametime_value_rc
		};

		RendererLabel_.SetText("Renderer: ");
		RendererValue_.SetText(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

		RowComponent renderer_label_rc;
		renderer_label_rc.ElementWidth = 75;
		renderer_label_rc.RComponent = &RendererLabel_;

		RowComponent renderer_value_rc;
		renderer_value_rc.ElementWidth = 200;
		renderer_value_rc.RComponent = &RendererValue_;

		Renderer_.Components = {
			renderer_label_rc,
			renderer_value_rc
		};

		VendorLabel_.SetText("Vendor: ");
		VendorValue_.SetText(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

		RowComponent vendor_label_rc;
		vendor_label_rc.ElementWidth = 75;
		vendor_label_rc.RComponent = &VendorLabel_;

		RowComponent vendor_value_rc;
		vendor_value_rc.ElementWidth = 200;
		vendor_value_rc.RComponent = &VendorValue_;

		Vendor_.Components = {
			vendor_label_rc,
			vendor_value_rc
		};

		VersionLabel_.SetText("Version: ");
		VersionValue_.SetText(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		RowComponent version_label_rc;
		version_label_rc.ElementWidth = 75;
		version_label_rc.RComponent = &VersionLabel_;

		RowComponent version_value_rc;
		version_value_rc.ElementWidth = 200;
		version_value_rc.RComponent = &VersionValue_;

		Version_.Components = {
			version_label_rc,
			version_value_rc
		};

		Layout_.Components = {
			&FPS_,
			&Renderer_,
			&Vendor_,
			&Version_
		};

		Layout_.GroupName = "debug_widget";
	}

	void DebugWidget::Draw(nk_context * ctx)
	{
		FPSValue_.SetText(std::to_string(Timer::FPS().GetValue()));
		FrameTimeValue_.SetText(std::to_string(1.0f / (float)Timer::FPS().GetValue() * 1000).append(" ms"));

		nk_layout_row_static(ctx, 250, 380, 1);
		Layout_.Flags = NK_WINDOW_BACKGROUND;
		Layout_.Draw(ctx);
	}

} }