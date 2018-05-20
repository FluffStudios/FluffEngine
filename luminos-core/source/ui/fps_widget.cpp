#include <ui/fps_widget.h>

namespace luminos { namespace ui {

	FPSWidget::FPSWidget()
		: IWidget()
	{
		Timer::FPS().SetListener([&](uint16_t old_value, uint16_t new_value) {
			FPSCounter_.SetText(std::to_string(new_value));
		});
		FPSLabel_.SetText("FPS: ");

		RowComponent compLabel;
		compLabel.RComponent = &FPSLabel_;
		compLabel.ElementWidth = 50;

		RowComponent compFPS;
		compFPS.RComponent = &FPSCounter_;
		compFPS.ElementWidth = 75;

		Row_.State = STATIC;
		Row_.Components = {
			compLabel,
			compFPS
		};

		Layout_.Components.push_back(&Row_);
		Layout_.GroupName = "FPSCounter";
	}

	void FPSWidget::Draw(nk_context * ctx)
	{
		nk_layout_row_static(ctx, 50, 125, 1);
		Layout_.Flags = NK_WINDOW_BACKGROUND;
		Layout_.Draw(ctx);
	}

} }