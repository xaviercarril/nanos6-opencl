/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef CUDA_COMPUTE_PLACE_HPP
#define CUDA_COMPUTE_PLACE_HPP

#include "hardware/places/ComputePlace.hpp"
#include "hardware/opencl/compute/queue/openclQueuePool.hpp"
#include "hardware/opencl/compute/synchronization/openclEventPool.hpp"

class Task;

class openclComputePlace: public ComputePlace {
private:
        using openclEventList = std::vector<openclEvent *>;

        openclQueuePool _queuePool;
        openclEventPool _eventPool;

        openclEventList _activeEvents;

        SpinLock _lock;
public:
        using openclTaskList = std::vector<Task*>;

      	openclComputePlace(int device);
        ~openclComputePlace();

        //! \brief Returns a list of tasks which kernels have finished
        openclTaskList getListFinishedTasks();

        //! \brief Assign compute resources to a task
        void preRunTask(Task *task);

        //! \brief Execute the body of a task
        void runTask(Task *task);

        //! \brief Release the compute resources of the CUDA task
        void postRunTask(Task *task);
};

