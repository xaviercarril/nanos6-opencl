/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)
*/

#include "openclNaiveScheduler.hpp"

#include "tasks/Task.hpp"
#include "hardware/HardwareInfo.hpp"
#include "hardware/opencl/compute/openclComputePlace.hpp"

#include <algorithm>
#include <cassert>
#include <mutex>


openclNaiveScheduler::openclNaiveScheduler(__attribute__((unused)) int numaNodeIndex)
{
	// Populate idle queues
	for (unsigned int i = 0; i < HardwareInfo::getComputePlaceCount(nanos6_device_t::nanos6_opencl_device); ++i) {
		opeclComputePlace *gpu = (openclComputePlace *) HardwareInfo::getComputePlace(nanos6_device_t::nanos6_opencl_device, i);
		_idleGpus.push_back(gpu);
	}
}


openclNaiveScheduler::~openclNaiveScheduler()
{
}


ComputePlace * openclNaiveScheduler::addReadyTask(Task *task, __attribute__((unused)) ComputePlace *hardwarePlace, __attribute__((unused)) ReadyTaskHint hint, bool doGetIdle)
{
	assert(task->getDeviceType() == nanos6_device_t::nanos6_opencl_device);

	std::lock_guard<SpinLock> guard(_globalLock);
	_readyTasks.push_front(task);

	return nullptr;
}


Task *openclNaiveScheduler::getReadyTask(ComputePlace *computePlace, __attribute__((unused)) Task *currentTask, __attribute__((unused)) bool canMarkAsIdle, __attribute__((unused)) bool doWait)
{
	assert(computePlace->getType() == nanos6_device_t::nanos6_opencl_device);

	Task *task = nullptr;

	std::lock_guard<SpinLock> guard(_globalLock);

	// 1. Get a ready task
	if( !_readyTasks.empty() ){
		task = _readyTasks.front();
		_readyTasks.pop_front();

		assert(task != nullptr);

		return task;
	}

	return nullptr;
}


ComputePlace *openclNaiveScheduler::getIdleComputePlace(bool force)
{
	return nullptr;
}

std::string openclNaiveScheduler::getName() const
{
	return "opencl-naive";
}
