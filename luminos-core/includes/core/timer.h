#pragma once

#include <common.h>
#include <core/beans/property.h>

namespace luminos { namespace ecs {
	class SystemManager;
} }

namespace luminos
{
	class Timer
	{
		friend class ecs::SystemManager;
		Property<uint16_t> FPS_;
		double DeltaTime_;

		double LastTime_;
		double LastFrameTime_;
		double LastUpdateTime_;

		uint16_t numFrames;

		static void Update(const double RefreshRate);
		Timer();
		static Timer Instance_;
	public:
		/*
			Gets the FPS property for the engine

			Returns FPS property
		*/
		static Property<uint16_t>& FPS();

		/*
			Gets time between updates

			Returns time between updates
		*/
		static double DeltaTime();

		/*
			Gets the current time in the engine

			Returns current time
		*/
		static double CurrentTime();

		/*
			Gets the time since the update

			Returns the time since the last update
		*/
		static double TimeSinceUpdate();
	};
}