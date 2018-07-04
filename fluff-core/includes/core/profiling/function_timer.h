#pragma once

#include <common.h>
#include <chrono>

namespace fluff { namespace profile {
	
	class FLUFF_API FunctionTimer
	{
		std::chrono::time_point<std::chrono::steady_clock> StartTime_;
		double ElapsedTime_;
		bool Finished_ = false;
	public:
		FunctionTimer();
		~FunctionTimer();
		double TimeSinceConstruction() const;
		double TimeAtFinish() const;
		void Finish();
	};

} }