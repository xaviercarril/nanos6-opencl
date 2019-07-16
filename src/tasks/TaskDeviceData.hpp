/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
	
	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)
*/

#ifndef TASK_DEVICE_DATA_HPP
#define TASK_DEVICE_DATA_HPP

#include <config.h>

#ifdef USE_CUDA

#include "hardware/cuda/compute/stream/CUDAStream.hpp"


struct CUDADeviceData {
	CUDAStream *_stream;
	
	CUDADeviceData():
		_stream(nullptr)
	{
	}
};

#endif //USE_CUDA

#ifdef USE_OPENCL

#include "hardware/opencl/compute/queue/openclQueue.hpp"


struct openclDeviceData {
	openclQueue *_queue;
	
	openclDeviceData():
		_queue(nullptr,nullptr,nullptr)
	{
	}
};

#endif //USE_OPENCL

#endif //TASK_DEVICE_DATA_HPP

