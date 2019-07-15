/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_QUEUE_POOL_HPP
#define OPENCL_QUEUE_POOL_HPP

#include <queue>

#define OPENCL_STARTING_QUEUE_NUM 0

#include "openclQueue.hpp"

class openclQueuePool {
private:
        std::queue<openclQueue *> _pool;
        int _size;

public:

	openclQueuePool(cl_context context, cl_device_id device)
        {
                for (int i = 0; i < OPENCL_STARTING_QUEUE_NUM; ++i) {
                        _pool.push(new openclQueue(context, device, (cl_int) i));
                }
                _size = OPENCL_STARTING_QUEUE_NUM;
        }

	~openclQueuePool()
        {
                assert(_pool.size() == _size);

                while (!_pool.empty()) {
                        delete _pool.front();
                        _pool.pop();
                }
        }

        //!     \!brief Get a Queue
        //!     Get a OpenCL Queue from the pool.
        //!     If no queues are available a new queue is returned, which will be eventually returned to the pool instead of released.
        openclQueue *getQueue()
        {
                if (_pool.empty()) {
                        /*We need to create a program*/
                        // ++_size;
                        // return new openclQueue(_size - 1);
                } else {
                        openclQueue *queue = _pool.front();
                        _pool.pop();
                        return queue;
                }
        }
        //!     \!brief Return a opencl queue to the pool
        void returnQueue(openclQueue *queue)
        {
                _pool.push(queue);
        }
};
