/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)
*/

#ifndef OPENCL_EVENT_POOL_HPP
#define OPENCL_EVENT_POOL_HPP

#include "openclEvent.hpp"

#include <queue>

class openclEventPool {

private:
        std::queue<openclEvent *> _pool;

#ifndef NDEBUG
        int _poolSize;
#endif

public:

        openclEventPool()
        {
#ifndef NDEBUG
                _poolSize = 0;
#endif
        }

        ~openclEventPool()
        {
                assert(_pool.size() == _poolSize);

                while (!_pool.empty()) {
                        delete _pool.front();
                        _pool.pop();
                }
        }
	openclEvent *getEvent()
        {
                if (_pool.empty()) {
#ifndef NDEBUG
                        ++_poolSize;
#endif
                        return new openclEvent();
                } else {
                        openclEvent *event = _pool.front();
                        _pool.pop();
                        return event;
                }
        }

        void returnEvent(openclEvent *event)
        {
                _pool.push(event);
        }
};

#endif //OPENCL_EVENT_POOL_HPP
