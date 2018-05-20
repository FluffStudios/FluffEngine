#include <ui/components/button_component.h>

namespace luminos { namespace ui {

	ButtonComponent::ButtonComponent(std::string Text)
		: Text_(Text)
	{	}

	void ButtonComponent::Draw(nk_context * ctx)
	{
		if (nk_button_label(ctx, Text_.c_str()))
		{
			Function_();
		}
	}

	void ButtonComponent::SetFunction(std::function<void()> Function)
	{
		Function_ = Function;
	}

} }