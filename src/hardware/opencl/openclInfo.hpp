#ifndef OPENCL_INFO_HPP
#define OPENCL_INFO_HPP

#include "openclDevice.hpp"
#include "hardware/hwinfo/DeviceInfo.hpp"

#include <CL/cl.h>
#include <vector>


class openclInfo: public DeviceInfo {
private:
        std::vector<openclDevice *> _devices;
	std::vector<openclPollingService *> _pollingServices;
public:

        void initialize();
        void shutdown();

        inline size_t getComputePlaceCount(void)
        {
                return _devices.size();
        }
        inline ComputePlace* getComputePlace(int index)
        {
                return _devices[index]->getComputePlace();
        }

        inline size_t getMemoryPlaceCount(void)
        {
                return _devices.size();
        }
	inline MemoryPlace* getMemoryPlace(int index)
        {
                return _devices[index]->getMemoryPlace();
        }

	inline openclPollingService *getPollingService(int index)
        {
                return _pollingServices[index];
        }

        static inline void setDevice(int device)
        {
		
	}
//FALTA ACABAR!!
/*
                cudaError_t err = cudaSetDevice(device);
                CUDAErrorHandler::handle(err, "When setting CUDA device environment");
        }
*/
};

#endif //OPENCL_INFO_HPP
