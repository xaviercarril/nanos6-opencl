#ifndef OPENCL_INFO_HPP
#define OPENCL_INFO_HPP

#include "openclDevice.hpp"
#include "hardware/hwinfo/DeviceInfo.hpp"
#include "executors/opencl/openclPollingService.hpp"

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif

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
/*
        static inline void setDevice(int device)
        {

        }
*/
};

#endif //OPENCL_INFO_HPP
