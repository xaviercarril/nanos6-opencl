/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)
*/

#ifndef NANOS6_OPENCL_DEVICE_H
#define NANOS6_OPENCL_DEVICE_H

#include "major.h"


#pragma GCC visibility push(default)


// NOTE: The full version depends also on nanos6_major_api
//       That is:   nanos6_major_api . nanos6_opencl_device_api
enum nanos6_opencl_device_api_t { nanos6_opencl_device_api = 1 };

//#ifdef HAVE_OPENCL_OPENCL_H
//#include <OpenCL/opencl.h>
//#include <OpenCL/cl.hpp>
//#endif

//#ifdef HAVE_CL_OPENCL_H
#include <CL/opencl.h>
//#include <CL/cl.hpp>
//#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	cl_command_queue queue;
	cl_context context;
	cl_program program;
} nanos6_opencl_device_environment_t;

void nanos6_execute_opencl(struct node * args, nanos6_opencl_device_environment_t * device_env);

#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop

#endif /* NANOS6_OPENCL_DEVICE_H */
