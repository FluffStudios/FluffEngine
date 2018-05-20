#include <ui/components/label_component.h>

namespace luminos { namespace ui { 

	TextLabelComponent::TextLabelComponent(std::string Text)
		: Text_(Text)
	{ }

	void TextLabelComponent::Draw(nk_context * ctx)
	{
		nk_label(ctx, Text_.c_str(), Flags_);
	}

	void TextLabelComponent::SetText(const std::string Text)
	{
		Text_ = Text;
	}

	void TextLabelComponent::SetFlags(unsigned int Flags)
	{
		Flags_ = Flags;
	}

	std::string TextLabelComponent::GetText() const
	{
		return Text_;
	}

	unsigned int TextLabelComponent::GetFlags() const
	{
		return Flags_;
	}

} }