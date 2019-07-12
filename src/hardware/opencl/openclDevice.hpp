#ifndef OPENCL_DEVICE_HPP
#define OPENCL_DEVICE_HPP

#include "tasks/Task.hpp"

#include "compute/openclComputePlace.hpp"
#include "memory/openclMemoryPlace.hpp"

#include "hardware/places/NUMAPlace.hpp"

#include <vector>


class openclDevice {
private:
        openclComputePlace * _computePlace;               //!< ComputePlace associated to this device
        openclMemoryPlace * _memoryPlace;                 //!< MemoryPlace associated to this device

        int _index;                                                     //!< Index of the OpenCL device
        NUMAPlace *_numaPlace;                                  //!< NUMA node where this device is located
public:

        openclDevice(int index) :
                _index(index)
        {
                _computePlace = new openclComputePlace(index);
                _memoryPlace = new openclMemoryPlace(index);
        }

        CUDAComputePlace *getComputePlace() const
        {
                return _computePlace;
        }

        CUDAMemoryPlace *getMemoryPlace() const
        {
                return _memoryPlace;
        }

        int getIndex() const
        {
                return _index;
        }

        NUMAPlace *getNUMAPlace() const
        {
                return _numaPlace;
        }
};

#endif //OPENCL_DEVICE_HPP
