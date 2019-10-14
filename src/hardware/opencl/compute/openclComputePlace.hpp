/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_COMPUTE_PLACE_HPP
#define OPENCL_COMPUTE_PLACE_HPP

#ifndef NDEBUG
#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif
#endif

#include "hardware/places/ComputePlace.hpp"
#include "hardware/opencl/compute/queue/openclQueuePool.hpp"
#include "hardware/opencl/compute/synchronization/openclEventPool.hpp"

class Task;

class openclComputePlace: public ComputePlace {
private:
        using openclEventList = std::vector<openclEvent *>;

        openclQueuePool * _queuePool;
        openclEventPool * _eventPool;

        openclEventList _activeEvents;

        SpinLock _lock;

				int _index;
				cl::Program _program;
public:
        using openclTaskList = std::vector<Task*>;

        cl_int err;

				struct buffer_arg {
						size_t size;
						cl_mem_flags flags;
						DataAccessType type;
						void * startAddress;
						cl::Buffer buffer;
				};

      	openclComputePlace(int index_device, cl::Program program, cl::Context context, cl::Device device);
        ~openclComputePlace();

        //! \brief Returns a list of tasks which kernels have finished
        openclTaskList getListFinishedTasks();

        //! \brief Assign compute resources to a task
        void preRunTask(Task *task);

        //! \brief Execute the body of a task
        void runTask(Task *task);

        //! \brief Release the compute resources of the OpenCL task
        void postRunTask(Task *task);

				cl_mem_flags accessType(DataAccessType type);
};

#endif //OPENCL_COMPUTE_PLACE_HPP
