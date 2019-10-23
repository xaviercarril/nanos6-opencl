/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018-2019 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#include "openclComputePlace.hpp"
#include "../memory/openclMemoryPlace.hpp"

#include "tasks/Task.hpp"
#include "tasks/TaskDeviceData.hpp"

#include <nanos6/opencl_device.h>


openclComputePlace::openclComputePlace(int index_device, cl::Program program, cl::Context context, cl::Device device) : ComputePlace(index_device, nanos6_device_t::nanos6_opencl_device)
{
	_queuePool = new openclQueuePool(context, device);
	_eventPool = new openclEventPool();

	_index = index_device;
	_program = program;
}

openclComputePlace::~openclComputePlace()
{
}

openclComputePlace::openclTaskList openclComputePlace::getListFinishedTasks()
{
        std::lock_guard<SpinLock> guard(_lock);

        openclTaskList finishedTasks;

        auto it = _activeEvents.begin();
        while (it != _activeEvents.end())
        {
                if ((*it)->finished()) {
                        openclEvent* evt = *it;

                        _eventPool->returnEvent(evt);
                        finishedTasks.push_back(evt->getTask());

                        it = _activeEvents.erase(it);
                } else {
                        ++it;
                }
        }

        return finishedTasks;
}

void openclComputePlace::preRunTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        task->setComputePlace(this);
        taskData->_queue = _queuePool->getQueue();
}

void openclComputePlace::runTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        nanos6_opencl_device_environment_t env;
				cl::CommandQueue commandqueue = taskData->_queue->getQueue();
				//Wrapper operator ()() returns cl_type. In this case cl_command_queue
        env.queue = commandqueue();

				cl::Context context;
				err = _program.getInfo(CL_PROGRAM_CONTEXT, &context);
				openclErrorHandler::handle(err, "Getting Program Context");
				env.context = context();

				env.program = _program();

  			task->body((void *) &env);
/*
				auto it = task->getArgsBlock();
				size_t sizeBlock = task->getArgsBlockSize();
				std::cout << "Punter Arguments: " << it << std::endl;
				std::cout << "Size Args: " << sizeBlock << std::endl;

				for (auto i = (int *)it; i < it + sizeBlock; i++) {
					std::cout << "Arg(" << i << "): " << *(int *) i << std::endl;
				}
*/
/*
				TaskStatistics* taskS = task->getTaskStatistics();
				TaskPredictions* taskP = task->getTaskPredictions();
				TaskHardwareCounters* taskH = task->getTaskHardwareCounters();
				TaskHardwareCountersPredictions* taskHP = task->getTaskHardwareCountersPredictions();
				TaskDataAccesses& taskD = task->getDataAccesses();
				nanos6_task_info_t* taskI = task->getTaskInfo();
				nanos6_task_invocation_info_t* taskII =task->getTaskInvokationInfo();
*/
/*
				std::vector<std::vector<DataAccess*>> allocation_vector;
				TaskDataAccesses &accessStructures = task->getDataAccesses();
				std::lock_guard<TaskDataAccesses::spinlock_t> guard(accessStructures._lock);
				allocation_vector.resize(task->getNumSymbols());

				accessStructures._accesses.processAll(
            [&](TaskDataAccesses::accesses_t::iterator position) -> bool {
                DataAccess *dataAccess = &(*position);
                assert(dataAccess != nullptr);
                //Se que log2(data...) es muy raro, pero básicamente es pasar de la representación 2^X a X, ya que symbols es un bitset
                //Funciona siempre y cuando solo haya un bit a 1 en symbols.
                allocation_vector[log2(dataAccess->getSymbols().to_ulong())].push_back(dataAccess);
								return true;
            }
        );

				vector<buffer_arg> buf_vector;
				for (int i = 0; i < allocation_vector.size(); ++i) {
					for (int j = 0; j < allocation_vector[i].size(); ++j) {
						buffer_arg str_buff;
						str_buff.size = allocation_vector[i][j]->getAccessRegion().getSize();
						str_buff.type = allocation_vector[i][j]->getType();
						str_buff.flags = openclComputePlace::accessType(str_buff.type);
						str_buff.startAddress = allocation_vector[i][j]->getAccessRegion().getStartAddress();
						std::cout << i << ", " << j << " : " << str_buff.startAddress << " " << str_buff.size << std::endl;

						cl::Context context;
						err = _program.getInfo(CL_PROGRAM_CONTEXT, &context);
						openclErrorHandler::handle(err, "Getting Program Context");

						str_buff.buffer = cl::Buffer(context, str_buff.flags, str_buff.size);
						buf_vector.push_back(str_buff);

						if (str_buff.flags == CL_MEM_READ_ONLY || str_buff.flags == CL_MEM_READ_WRITE) {
							err = commandqueue.enqueueWriteBuffer(str_buff.buffer, CL_TRUE, 0, str_buff.size, str_buff.startAddress);
							openclErrorHandler::handle(err, "Writing Buffer");
						}
					}
				}


				cl::Kernel kernel(_program, "Muld"); // Hardcoded

				for (int i = 0; i < buf_vector.size(); ++i) {
						err = kernel.setArg(i, buf_vector[i].buffer);
						openclErrorHandler::handle(err, "Setting Kernel Arguments");
				}

				err = commandqueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1024,1024), cl::NDRange(16,16));
				openclErrorHandler::handle(err, "Executing Kernel");

				for (int i = 0; i < buf_vector.size(); ++i) {
						if (buf_vector[i].flags == CL_MEM_WRITE_ONLY || buf_vector[i].flags == CL_MEM_READ_WRITE) {
              err = commandqueue.enqueueReadBuffer(buf_vector[i].buffer, CL_TRUE, 0, buf_vector[i].size, buf_vector[i].startAddress);
							openclErrorHandler::handle(err, "Reading Buffer");
						}
				}
*/
        openclEvent *event = _eventPool->getEvent();
        event->setTask(task);
        event->record();
        _activeEvents.push_back(event);
}

void openclComputePlace::postRunTask(Task *task)
{
        openclDeviceData *taskData = (openclDeviceData *) task->getDeviceData();

        _queuePool->returnQueue(taskData->_queue);
}


cl_mem_flags openclComputePlace::accessType(DataAccessType type) {
				cl_mem_flags flags;
				switch (type)
				{
					case READ_ACCESS_TYPE :
							flags = CL_MEM_READ_ONLY;
							break;
					case WRITE_ACCESS_TYPE :
							flags = CL_MEM_WRITE_ONLY;
							break;
					case READWRITE_ACCESS_TYPE :
							flags = CL_MEM_READ_WRITE;
							break;
				}
				return flags;
}
