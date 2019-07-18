/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef NAIVE_OPENCL_SCHEDULER_HPP
#define NAIVE_OPENCL_SCHEDULER_HPP

#include <deque>
#include <vector>

#include "scheduling/SchedulerInterface.hpp"
#include "lowlevel/SpinLock.hpp"

class Task;
class openclComputePlace;

class openclNaiveScheduler: public SchedulerInterface {
	SpinLock _globalLock;

	//opencl Tasks
	std::deque<Task *> _readyTasks;
	std::deque<Task *> _blockedTasks;

	//opencl devices
	std::deque<openclComputePlace *> _idleGpus;

public:
	
	openclNaiveScheduler(int numaNodeIndex);
	~openclNaiveScheduler();
	ComputePlace *addReadyTask(Task *task, ComputePlace *hardwarePlace, ReadyTaskHint hint, bool doGetIdle = true);

	Task *getReadyTask(ComputePlace *hardwarePlace, Task *currentTask = nullptr, bool canMarkAsIdle = true, bool doWait = false);

	ComputePlace *getIdleComputePlace(bool force=false);

	std::string getName() const;
};


#endif // NAIVE_OPENCL_SCHEDULER_HPP
