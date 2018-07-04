#pragma once

namespace fluff { namespace threading {

	class ITask 
	{
	public:
		ITask() = default;
		virtual ~ITask() = default;
		ITask(const ITask & RHS) = delete;
		ITask& operator= (const ITask & RHS) = delete;
		ITask(ITask && Other) = default;
		ITask& operator = (ITask && Other) = default;

		virtual void Execute() = 0;
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