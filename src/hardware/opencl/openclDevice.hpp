#ifndef OPENCL_DEVICE_HPP
#define OPENCL_DEVICE_HPP

#include "tasks/Task.hpp"

#include "compute/openclComputePlace.hpp"
#include "memory/openclMemoryPlace.hpp"

#include "hardware/places/NUMAPlace.hpp"

#include "openclProgram.hpp"
#include <vector>


class openclDevice {
private:
        openclComputePlace * _computePlace;               //!< ComputePlace associated to this device
        openclMemoryPlace * _memoryPlace;                 //!< MemoryPlace associated to this device

        int _index;                                                     //!< Index of the OpenCL device
        NUMAPlace *_numaPlace;                                  //!< NUMA node where this device is located
	cl::Device _device;
	openclProgram _program;
public:

        openclDevice(int index, cl::Device device) :
                _index(index)
        {
                _computePlace = new openclComputePlace(index);
                _memoryPlace = new openclMemoryPlace(index);

		_device = device;
		_program = new openclProgram(this);
        }

	~openclDevice()
	{
		delete _program;
		delete _computePlace;
		delete _memoryPlace;
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
		
	cl::Device getDevice() const
	{
		return _device;
	}
};

#endif //OPENCL_DEVICE_HPP
