#pragma once

#include <common.h>
#include <chrono>

namespace fluff { namespace profile {
	
	class FunctionTimer
	{
		std::chrono::time_point<std::chrono::steady_clock> StartTime_;
		double ElapsedTime_;
		bool Finished_ = false;
	public:
		FLUFF_API FunctionTimer();
		FLUFF_API ~FunctionTimer();
		double FLUFF_API TimeSinceConstruction() const;
		double FLUFF_API TimeAtFinish() const;
		void FLUFF_API Finish();
	};

} }