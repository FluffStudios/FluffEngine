#include <core/timer.h>

#include <glfw3.h>

namespace luminos
{
	Timer Timer::Instance_;
	
	void Timer::Update(const double RefreshRate)
	{
		double current_time = glfwGetTime();
		Instance_.DeltaTime_ = current_time - Instance_.LastTime_;
		Instance_.LastTime_ = current_time;
		Instance_.numFrames++;
		if (current_time - Instance_.LastFrameTime_ >= 1.0)
		{
			Instance_.FPS_ = Instance_.numFrames;
			Instance_.numFrames = 0;
			Instance_.LastFrameTime_ += 1.0;
		}
		if (current_time - Instance_.LastFrameTime_ >= RefreshRate)
		{
			Instance_.LastUpdateTime_ = current_time;
		}
	}

	Timer::Timer()
	{
		FPS_ = 0;
		DeltaTime_ = 0;
		LastTime_ = glfwGetTime();
		LastFrameTime_ = LastTime_;
		numFrames = 0;
	}

	Property<uint16_t>& Timer::FPS()
	{
		return Instance_.FPS_;
	}

	double Timer::DeltaTime()
	{
		return Instance_.DeltaTime_;
	}

	double Timer::CurrentTime()
	{
		return glfwGetTime();
	}

	double Timer::TimeSinceUpdate()
	{
		return glfwGetTime() - Instance_.LastUpdateTime_;
	}

}