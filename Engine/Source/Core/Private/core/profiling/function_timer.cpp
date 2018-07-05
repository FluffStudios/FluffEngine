#include <chrono>
#include <core/profiling/function_timer.h>

namespace fluff { namespace profile {

	FunctionTimer::FunctionTimer()
		: ElapsedTime_(0), StartTime_(std::chrono::high_resolution_clock::now())
	{	}

	FunctionTimer::~FunctionTimer()
	{
		Finish();
	}

	double FunctionTimer::TimeSinceConstruction() const 
	{
		if (Finished_) return -1;
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - StartTime_).count();
		return static_cast<double>(elapsed);
	}

	double FunctionTimer::TimeAtFinish() const
	{
		if (!Finished_) return -1;
		return ElapsedTime_;
	}

	void FunctionTimer::Finish()
	{
		ElapsedTime_ = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - StartTime_).count());
		Finished_ = true;
	}

} }