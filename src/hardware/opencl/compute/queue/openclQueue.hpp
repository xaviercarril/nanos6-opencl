/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_QUEUE_HPP
#define OPENCL_QUEUE_HPP

#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_OPENCL_H
#include <CL/opencl.h>
#endif

#include "lowlevel/opencl/openclErrorHandler.hpp"

class openclComputePlace;

class openclQueue {

private:
	size_t _index;
	cl_command_queue _queue;

public:
	openclQueue(cl_context context, cl_device_id device, size_t index): _index(index)
	{
		cl_int err;
		_queue = clCreateCommandQueue(context, device_id, 0, &err);
		openclErrorHandler::handle(err, "When creating queue");
	}

	//Disable copy constructor
	openclQueue(openclQueue const &) = delete;
	openclQueue operator=(openclQueue const &) = delete;

	~openclQueue()
	{
		cl_int err = clReleaseCommandQueue(_queue);
		openclErrorHandler::handle(err, "When destroying queue");
	}

	// Get context
	cl_context getContextQueue()
	{
		cl_context ret;
		cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_CONTEXT, sizeof(cl_context), &ret, NULL);
		openclErrorHandler::handle(err, "When getting context info queue");
		return ret;
	}

	// Get device
	cl_device_id getDeviceQueue()
	{
		cl_device_id ret;
		cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_DEVICE, sizeof(cl_device_id), &ret, NULL);
		openclErrorHandler::handle(err, "When getting device info queue");
		return ret;
	}

	// Get Reference Count
	cl_uint getReferenceCountQueue()
	{
		cl_uint ret;
		cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_REFERENCE_COUNT, sizeof(cl_uint), &ret, NULL);
		openclErrorHandler::handle(err, "When getting reference count info queue");
		return ret;
	}

	//! \brief Get the underlying openclQueue object
	cl_command_queue getQueue() const
	{
		return _queue;
	}
	
	//! \brief Get the assigned index of the queue
        size_t getIndex() const
        {
                return _index;
        }
};

#endif //OPENCL_QUEUE_HPP
