/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_EVENT
#define OPENCL_EVENT

#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/cl.hpp>
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_OPENCL_H
#include <CL/cl.hpp>
#include <CL/opencl.h>
#endif

#include <iostream>

#include "lowlevel/opencl/openclErrorHandler.hpp"
#include "tasks/TaskDeviceData.hpp"
#include "tasks/Task.hpp"

class openclEvent {

private:
        Task *_task;
        cl_event _event;

public:
        openclEvent()
        {
		/*cl_int err;
                _event = clCreateUserEvent(((openclDeviceData *) _task->getDeviceData())->_queue->getContextQueue(), &err);
                openclErrorHandler::handle(err, "When creating event");*/
        }

        ~openclEvent()
        {
                cl_int err = clReleaseEvent(_event);
                openclErrorHandler::handle(err, "When destroying event");
        }

        void setTask(Task *task)
        {
                _task = task;
        }
	Task *getTask()
        {
                return _task;
        }

        void record()
        {
		cl_int err;
                _event = clCreateUserEvent(((openclDeviceData *) _task->getDeviceData())->_queue->getContextQueue(), &err);
                openclErrorHandler::handle(err, "When recording event");
        }

        bool finished()
        {
		cl_int status;
                cl_int err = clGetEventInfo(_event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &status, nullptr);
                openclErrorHandler::handle(err, "When checking event status");
		return openclErrorHandlerEvent::handleEvent(status, "Status Error");
        }

};

#endif //OPENCL_EVENT
