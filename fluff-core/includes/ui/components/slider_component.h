#pragma once

#include <common.h>

#include <ui/iui_component.h>
#include <core/beans/property.h>
#include <ui/nk_ui.h>

namespace luminos { namespace ui { 

	class FloatSliderComponent : public IUIComponent
	{
		Property<float32_t> Value_;
		float32_t Min_ = 0.0f;
		float32_t Max_ = 0.0f;
		float32_t Step_ = 1.0f;
	public:
		/*
			Creates a slider
		*/
		FloatSliderComponent() { }

		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
		*/
		FloatSliderComponent(float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step = 1.0f);

		/*
			Draws slider to UI

			ctx - Context to draw with
		*/
		void Draw(nk_context * ctx) override;

		/*
			Gets the slider's value

			Return slider value
		*/
		float32_t GetSliderValue() const;
	};

	class FloatPropertySliderComponent : public IUIComponent
	{
		Property<float32_t> Value_;
		float32_t Min_ = 0.0f;
		float32_t Max_ = 0.0f;
		float32_t Step_ = 1.0f;
		float32_t IncPP_ = 0.1f;
		std::string Name_ = "";
	public:
		/*
			Creates a slider
		*/
		FloatPropertySliderComponent() { }

		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
			IncrementPerPixel - Increment between pixels
		*/
		FloatPropertySliderComponent(float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step = 1.0f, float32_t IncrementPerPixel = 0.1f);
		
		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
			IncrementPerPixel - Increment between pixels
			Name - Name of slider
		*/
		FloatPropertySliderComponent(std::string Name, float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step = 1.0f, float32_t IncrementPerPixel = 0.1f);
		
		/*
			Draws slider to UI

			ctx - Context to draw with
		*/
		void Draw(nk_context * ctx) override;

		/*
			Gets the slider's value

			Return slider value
		*/
		float32_t GetSliderValue() const;
	};

	class IntegerSliderComponent : public IUIComponent
	{
		Property<int32_t> Value_;
		int32_t Min_ = 0;
		int32_t Max_ = 0;
		int32_t Step_ = 1;
	public:

		/*
			Creates a slider
		*/
		IntegerSliderComponent() { }

		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
		*/
		IntegerSliderComponent(int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step);

		/*
			Draws slider to UI

			ctx - Context to draw with
		*/
		void Draw(nk_context * ctx) override;

		/*
			Gets the slider's value

			Return slider value
		*/
		int32_t GetSliderValue() const;
	};

	class IntegerPropertySliderComponent : public IUIComponent
	{
		Property<int32_t> Value_;
		int32_t Min_ = 0;
		int32_t Max_ = 0;
		int32_t Step_ = 1;
		int32_t IncPP_ = 1;
		std::string Name_ = "";
	public:
		/*
			Creates a slider
		*/
		IntegerPropertySliderComponent() { };
		
		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
			IncrementPerPixel - Increment between pixels
		*/
		IntegerPropertySliderComponent(int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step = 1, int32_t IncrementPerPixel = 1);
		
		/*
			Creates a slider

			InitialValue - value to start slider at
			Minimum - Minimum value possible
			Maximum - Maximum value possible
			Step - Step between values
			IncrementPerPixel - Increment between pixels
			Name - Name of slider
		*/
		IntegerPropertySliderComponent(std::string Name, int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step = 1, int32_t IncrementPerPixel = 1);
		
		/*
			Draws slider to UI

			ctx - Context to draw with
		*/
		void Draw(nk_context * ctx) override;

		/*
			Gets the slider's value

			Return slider value
		*/
		int32_t GetSliderValue() const;
	};

} }