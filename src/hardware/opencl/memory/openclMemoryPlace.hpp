/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_MEMORY_PLACE_HPP
#define OPENCL_MEMORY_PLACE_HPP

#include "hardware/places/MemoryPlace.hpp"

#ifdef HAVE_OPENCL_OPENCL_H
#include <OpenCL/cl.hpp>
#include <OpenCL/opencl.h>
#endif

#ifdef HAVE_CL_OPENCL_H
#include <CL/cl.hpp>
#include <CL/opencl.h>
#endif

class Task;

class openclMemoryPlace: public MemoryPlace {
public:
	openclMemoryPlace(int device)
		: MemoryPlace(device, nanos6_device_t::nanos6_opencl_device)
	{}

	~openclMemoryPlace()
	{}

	void preRunTask(Task *task);
	void runTask(Task *task);
	void postRunTask(Task *task);

};

#endif //OPENCL_MEMORY_PLACE_HPP
