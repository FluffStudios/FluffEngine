#pragma once

#include <ui/iui_component.h>

namespace luminos { namespace ui {

	enum TextLabelFlags
	{
		TEXT_LEFT = 1,
		TEXT_CENTER = 2,
		TEXT_RIGHT = 4,
		TEXT_TOP = 8,
		TEXT_MIDDLE = 16,
		TEXT_BOTTOM = 32
	};

	class TextLabelComponent : public IUIComponent
	{
		nk_flags Flags_ = TEXT_LEFT | TEXT_MIDDLE;
		std::string Text_ = "Label";
	public:
		/*
			Creates new default text label component
		*/
		TextLabelComponent() { }

		/*
			Creates text label component with specified text

			Text - What label should display
		*/
		TextLabelComponent(std::string Text);

		/*
			Draws label

			ctx - Context for drawing
		*/
		void Draw(nk_context * ctx) override;

		/*
			Sets label text

			Text - What label should display
		*/
		void SetText(const std::string Text);

		/*
			Set flags for alignment

			Flags - alignment flags
		*/
		void SetFlags(unsigned int Flags);

		/*
			Gets a copy of the label's value

			Returns copy of label's value
		*/
		std::string GetText() const;

		/*
			Gets a copy of the label's flags

			Returns copy of label's flags
		*/
		unsigned int GetFlags() const;
	};

} }