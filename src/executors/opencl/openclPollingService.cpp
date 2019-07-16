/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
	
	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#include "openclPollingService.hpp"

#include <DataAccessRegistration.hpp>

#include "hardware/opencl/openclInfo.hpp"
#include "scheduling/Scheduler.hpp"
#include "lowlevel/opencl/openclErrorHandler.hpp"

#include "tasks/Task.hpp"
#include "tasks/TaskDeviceData.hpp"
#include "tasks/TaskImplementation.hpp"

#include "executors/threads/TaskFinalization.hpp"

#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/cl.hpp>
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_OPENCL_H
#include <CL/cl.hpp>
#include <CL/opencl.h>
#endif

openclPollingService::openclPollingService(openclDevice *device)
	: _device(device)
{
	std::stringstream ss;
	ss << "openclPollingService-" << _device->getIndex();
	_serviceName = ss.str();
}

openclPollingService::~openclPollingService()
{
}

void openclPollingService::start()
{
	nanos6_register_polling_service(_serviceName.c_str(), (nanos6_polling_service_t) &openclPollingService::runHelper, (void *) this);
}

void openclPollingService::stop()
{
	nanos6_unregister_polling_service(_serviceName.c_str(), (nanos6_polling_service_t) &openclPollingService::runHelper, (void *) this);
}

void openclPollingService::finishTask(Task *task)
{
	//cl_int err = cudaPeekAtLastError();
	//CUDAErrorHandler::handle(err);
	
	_device->getComputePlace()->postRunTask(task);
	_device->getMemoryPlace()->postRunTask(task);
	
	openclDeviceData *deviceData = (openclDeviceData *) task->getDeviceData();
	delete deviceData;
	
	if (task->markAsFinished(_device->getComputePlace())) {
		DataAccessRegistration::unregisterTaskDataAccesses(task, _device->getComputePlace(), _device->getComputePlace()->getDependencyData());
		
		if (task->markAsReleased()) {
			TaskFinalization::disposeOrUnblockTask(task, _device->getComputePlace());
		}
	}
}

void openclPollingService::launchTask(Task *task)
{
	assert(_device != nullptr);
	assert(task != nullptr);
	
	cudaSetDevice(_device->getIndex());
	
	CUDADeviceData *deviceData = new CUDADeviceData();
	task->setDeviceData((void *) deviceData);
	
	CUDAComputePlace *computePlace = _device->getComputePlace();
	assert(computePlace != nullptr);
	
	CUDAMemoryPlace *memoryPlace = _device->getMemoryPlace();
	assert(memoryPlace != nullptr);
	
	task->setMemoryPlace(memoryPlace);
	
	computePlace->preRunTask(task);
	memoryPlace->preRunTask(task);
	
	memoryPlace->runTask(task);
	computePlace->runTask(task);
}

void CUDAPollingService::run()
{
	// Discover finished kernels and free their dependencies
	auto finishedTasks = _device->getComputePlace()->getListFinishedTasks();
	
	auto it = finishedTasks.begin();
	while (it != finishedTasks.end()) {
		finishTask(*it);
		it = finishedTasks.erase(it);
	}
	assert(finishedTasks.empty());
	
	/* Check for ready tasks */
	Task *task = Scheduler::getReadyTask(_device->getComputePlace());
	while (task != nullptr) {
		launchTask(task);
		task = Scheduler::getReadyTask(_device->getComputePlace());
	}
}

bool CUDAPollingService::runHelper(void *service_ptr)
{
	CUDAPollingService *service = (CUDAPollingService *) service_ptr;
	service->run();
	return false;
}

