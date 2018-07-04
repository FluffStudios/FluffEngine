#pragma once

#include <common.h>

namespace fluff { namespace threading {

	class ITask 
	{
	public:
		FLUFF_API ITask() = default;
		virtual FLUFF_API ~ITask() = default;
		FLUFF_API ITask(const ITask & RHS) = delete;
		ITask FLUFF_API & operator= (const ITask & RHS) = delete;
		FLUFF_API ITask(ITask && Other) = default;
		ITask FLUFF_API & operator = (ITask && Other) = default;

		virtual void FLUFF_API Execute() = 0;
	};

	template <typename Func>
	class Task : public ITask
	{
		Func Function_;
	public:
		Task(Func && Function)
			: Function_(Function)
		{	}

		virtual ~Task() override = default;
		Task(const Task & RHS) = delete;
		Task& operator=(const Task& RHS) = delete;
		Task(Task && Other) = default;
		Task& operator=(Task && Other) = default;

		inline void Execute() override
		{
			Function_();
		}
	};

} }