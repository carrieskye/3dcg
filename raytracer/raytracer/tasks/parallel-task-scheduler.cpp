#include "parallel-task-scheduler.h"

using namespace tasks;


namespace
{
	/// <summary>
	/// Performs tasks one after the other (i.e. not in parallel).
	/// </summary>
	class ParallelTaskScheduler : public schedulers::_private_::TaskSchedulerImplementation
	{
	public:
		void perform(std::vector<std::shared_ptr<Task>> tasks) const
		{
			#pragma omp parallel for
			for (int i = 0; i < tasks.capacity(); i++)
			{
				tasks.at(i)->perform();
			}
		}
	};
}

TaskScheduler schedulers::parallel()
{
	return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}