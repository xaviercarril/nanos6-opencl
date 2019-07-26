/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_MEMORY_PLACE_HPP
#define OPENCL_MEMORY_PLACE_HPP

#include "hardware/places/MemoryPlace.hpp"

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#include <CL/cl.hpp>
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
