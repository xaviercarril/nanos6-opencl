/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_QUEUE_HPP
#define OPENCL_QUEUE_HPP

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif

#include "lowlevel/opencl/openclErrorHandler.hpp"

//class openclComputePlace;

class openclQueue {

private:
	size_t _index;
	//cl_command_queue _queue;
	cl::CommandQueue _queue;

public:
	openclQueue(cl::Context context, cl::Device device, size_t index): _index(index)
	{
		cl_int err;
		//_queue = clCreateCommandQueue(context, device, 0, &err);
		_queue = cl::CommandQueue(context, device, 0, &err);
		openclErrorHandler::handle(err, "When creating queue");
	}

	//Disable copy constructor
	openclQueue(openclQueue const &) = delete;
	openclQueue operator=(openclQueue const &) = delete;

	~openclQueue()
	{
		//cl_int err = clReleaseCommandQueue(_queue);
		//openclErrorHandler::handle(err, "When destroying queue");
	}

	// Get context
	cl::Context getContextQueue()
	{
		cl::Context ret;
		//cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_CONTEXT, sizeof(cl_context), &ret, NULL);
		cl_int err = _queue.getInfo(CL_QUEUE_CONTEXT, &ret);
		openclErrorHandler::handle(err, "When getting context info queue");
		return ret;
	}

	// Get device
	cl::Device getDeviceQueue()
	{
		cl::Device ret;
		//cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_DEVICE, sizeof(cl_device_id), &ret, NULL);
		cl_int err = _queue.getInfo(CL_QUEUE_DEVICE, &ret);
		openclErrorHandler::handle(err, "When getting device info queue");
		return ret;
	}

	// Get Reference Count
	cl_uint getReferenceCountQueue()
	{
		cl_uint ret;
		//cl_int err = clGetCommandQueueInfo(_queue, CL_QUEUE_REFERENCE_COUNT, sizeof(cl_uint), &ret, NULL);
		cl_int err = _queue.getInfo(CL_QUEUE_REFERENCE_COUNT, &ret);
		openclErrorHandler::handle(err, "When getting reference count info queue");
		return ret;
	}

	//! \brief Get the underlying openclQueue object
	cl::CommandQueue getQueue() const
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
