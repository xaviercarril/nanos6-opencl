/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2019 Barcelona Supercomputing Center (BSC)

	      Author: Xavier Carril
*/

#include <nanos6.h>
#include <utility>
#include <vector>
#include <nanos6/opencl_device.h>
#include <stdlib.h>
#include <unistd.h>
#include "lowlevel/opencl/openclErrorHandler.hpp"

//#ifdef HAVE_OPENCL_CL_HPP
//#include <OpenCL/cl.hpp>
//#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif

struct node {
  void * arg;
  int size_arg;
  int order;
  int in_out_inout;
  node * next;
};

cl_mem_flags accessTypeFlag(int type) {
				cl_mem_flags flags;
				switch (type)
				{
					case 1 :
							flags = CL_MEM_READ_ONLY;
							break;
					case 2 :
							flags = CL_MEM_WRITE_ONLY;
							break;
					case 3 :
							flags = CL_MEM_READ_WRITE;
							break;
					default :
							flags = CL_MEM_READ_ONLY;
							break;
				}
				return flags;
}

void nanos6_execute_opencl(struct node * args, nanos6_opencl_device_environment_t * device_env) {
  std::vector<struct node> parameters;
  while (args->arg != nullptr) {
    parameters.push_back(*args);
    args = args->next;
  }

  cl_int err;
	cl_kernel kernel = clCreateKernel(device_env->program, "Muld", &err);
  //cl::Kernel kernel(device_env->program, "Muld");
  cl_mem buffers[parameters.size()];

  for (int i = 0; i < parameters.size(); i++) {
		printf("It: %d Size: %ld Content: %ld Index:%d\n", i, parameters[i].size_arg, parameters[i].arg, parameters[i].order);
		if (parameters[i].in_out_inout == 0) {
			err = clSetKernelArg(kernel, parameters[i].order, parameters[i].size_arg, parameters[i].arg);
			openclErrorHandler::handle(err, "Setting Escalar Argument");
		}
		else {
    	cl_mem_flags flag = accessTypeFlag(parameters[i].in_out_inout);
    	buffers[i] = clCreateBuffer(device_env->context, flag, parameters[i].size_arg, NULL, &err);
    	openclErrorHandler::handle(err, "Creating Buffer");

    	if (flag == CL_MEM_READ_ONLY || flag == CL_MEM_READ_WRITE) {
        printf("WRITE BUFFER -> It: %d Size: %ld Content: %ld Index:%d\n", i, parameters[i].size_arg, parameters[i].arg, parameters[i].order);
        err = clEnqueueWriteBuffer(device_env->queue, buffers[i], CL_TRUE, 0, (size_t) parameters[i].size_arg, parameters[i].arg, 0, NULL, NULL);
        openclErrorHandler::handle(err, "Writing Buffer");
    	}
			err = clSetKernelArg(kernel, parameters[i].order, sizeof(cl_mem), (void *) &buffers[i]);
    	openclErrorHandler::handle(err, "Set Argument");
		}
  }

  size_t global[2] = {1024,1024};
  size_t local[2] = {16,16};
  err = clEnqueueNDRangeKernel(device_env->queue, kernel, 2, nullptr, &global[0], &local[0], 0, NULL, NULL);
  openclErrorHandler::handle(err, "Executing Kernel");

  for (int i = 0; i < parameters.size(); i++) {
    cl_mem_flags flag = accessTypeFlag(parameters[i].in_out_inout);
    if (flag == CL_MEM_WRITE_ONLY || flag == CL_MEM_READ_WRITE) {
      printf("Vaig a llegir del buffer %d\n",i);
			printf("buffers[i]:%p, parameters[i].size_arg:%ld, parameters[i].arg:%p\n",buffers[i], parameters[i].size_arg, parameters[i].arg);
			err = clEnqueueReadBuffer(device_env->queue, buffers[i], CL_TRUE, 0, parameters[i].size_arg, parameters[i].arg, 0, NULL, NULL);
      openclErrorHandler::handle(err, "Reading Buffer");
    }
  }

}
