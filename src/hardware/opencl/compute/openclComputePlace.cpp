/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

				Author: Xavier Carril
*/

#include "openclComputePlace.hpp"
#include "../memory/openclMemoryPlace.hpp"

#include "tasks/Task.hpp"
#include "tasks/TaskDeviceData.hpp"

#include <nanos6/opencl_device.h>

#ifndef NDEBUG
#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/opencl.h>
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_OPENCL_H
#include <CL/opencl.h>
#include <CL/cl.hpp>
#endif
#endif

openclComputePlace::openclComputePlace(int device)
        : ComputePlace(device, nanos6_device_t::nanos6_opencl_device)
{
}

openclComputePlace::~openclComputePlace()
{
}

openclComputePlace::openclTaskList openclComputePlace::getListFinishedTasks()
{
        std::lock_guard<SpinLock> guard(_lock);

        openclTaskList finishedTasks;

        auto it = _activeEvents.begin();
        while (it != _activeEvents.end())
        {
                if ((*it)->finished()) {
                        openclEvent* evt = *it;

                        _eventPool.returnEvent(evt);
                        finishedTasks.push_back(evt->getTask());

                        it = _activeEvents.erase(it);
                } else {
                        ++it;
                }
        }

        return finishedTasks;
}

void openclComputePlace::preRunTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        task->setComputePlace(this);
        taskData->_queue = _queuePool.getQueue();
}

void openclComputePlace::runTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        nanos6_opencl_device_environment_t env;
        env.queue = taskData->_queue->getQueue();

        task->body((void *) &env);

        openclEvent *event = _eventPool.getEvent();
        event->setTask(task);
        event->record();
        _activeEvents.push_back(event);
}

void openclComputePlace::postRunTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        _queuePool.returnQueue(taskData->_queue);
}
