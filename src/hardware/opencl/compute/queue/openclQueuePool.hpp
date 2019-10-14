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
#include "../../openclProgram.hpp"

class openclQueuePool {
private:
        std::queue<openclQueue *> _pool;
        int _size;

	cl::Context _context;
	cl::Device _device;
public:

	openclQueuePool(cl::Context context, cl::Device device)
        {
		_context = context;
		_device = device;

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
            ++_size;
			      openclQueue *queue = new openclQueue(_context, _device, _size - 1);
            return queue;
          }
          else {
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

#endif //OPENCL_QUEUE_POOL_HPP
