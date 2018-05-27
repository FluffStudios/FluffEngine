#include <ui/components/slider_component.h>

namespace luminos { namespace ui {

	FloatSliderComponent::FloatSliderComponent(float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step)
		: Value_(InitialValue), Min_(Minimum), Max_(Maximum), Step_(Step)
	{	}

	void FloatSliderComponent::Draw(nk_context * ctx)
	{
		float value = Value_.GetValue();
		nk_slider_float(ctx, Min_, &value, Max_, Step_);
		Value_ = value;
	}

	float32_t FloatSliderComponent::GetSliderValue() const
	{
		return Value_.GetValue();
	}

	FloatPropertySliderComponent::FloatPropertySliderComponent(float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step, float32_t IncrementPerPixel)
		: Value_(InitialValue), Min_(Minimum), Max_(Maximum), Step_(Step), IncPP_(IncrementPerPixel)
	{	}

	FloatPropertySliderComponent::FloatPropertySliderComponent(std::string Name, float32_t InitialValue, float32_t Minimum, float32_t Maximum, float32_t Step, float32_t IncrementPerPixel)
		: FloatPropertySliderComponent(InitialValue, Minimum, Maximum, Step, IncrementPerPixel)
	{	
		Name_ = Name;
	}

	void FloatPropertySliderComponent::Draw(nk_context * ctx)
	{
		Value_ = nk_propertyf(ctx, Name_.c_str(), Min_, Value_.GetValue(), Max_, Step_, IncPP_);
	}

	float32_t FloatPropertySliderComponent::GetSliderValue() const
	{
		return Value_.GetValue();
	}

	IntegerSliderComponent::IntegerSliderComponent(int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step)
		: Value_(InitialValue), Min_(Minimum), Max_(Maximum), Step_(Step)
	{	}

	void IntegerSliderComponent::Draw(nk_context * ctx)
	{
		int32_t value = Value_.GetValue();
		nk_slider_int(ctx, Min_, &value, Max_, Step_);
		Value_ = value;
	}

	int32_t IntegerSliderComponent::GetSliderValue() const
	{
		return Value_.GetValue();
	}

	IntegerPropertySliderComponent::IntegerPropertySliderComponent(int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step, int32_t IncrementPerPixel)
		: Value_(InitialValue), Min_(Minimum), Max_(Maximum), Step_(Step), IncPP_(IncrementPerPixel)
	{	}

	IntegerPropertySliderComponent::IntegerPropertySliderComponent(std::string Name, int32_t InitialValue, int32_t Minimum, int32_t Maximum, int32_t Step, int32_t IncrementPerPixel)
		: IntegerPropertySliderComponent(InitialValue, Minimum, Maximum, Step, IncrementPerPixel)
	{
		Name_ = Name;
	}

	void IntegerPropertySliderComponent::Draw(nk_context * ctx) 
	{
		Value_ = nk_propertyi(ctx, Name_.c_str(), Min_, Value_.GetValue(), Max_, Step_, IncPP_);
	}

	int32_t IntegerPropertySliderComponent::GetSliderValue() const
	{
		return Value_.GetValue();
	}

} }