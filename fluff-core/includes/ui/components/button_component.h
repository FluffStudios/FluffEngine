#pragma once

#include <ui/iui_component.h>
#include <common.h>
#include <string>
#include <functional>

namespace luminos { namespace ui { 

	class LUMINOS_API ButtonComponent : public IUIComponent
	{
		std::string Text_ = "Button";
		std::function<void()> Function_;
	public:
		/*
			Creates default button with text value "Button"
		*/
		ButtonComponent() { }

		/*
			Creates button

			Text - Value of button
		*/
		ButtonComponent(std::string Text);

		/*
			Draws the button on the overlay

			ctx - Context to use
		*/
		void Draw(nk_context * ctx) override;
		
		/*
			Sets the function to call on button click

			Function - Function to call
		*/
		void SetFunction(std::function<void()> Function);
	};

} }