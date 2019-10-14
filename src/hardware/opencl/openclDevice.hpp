#ifndef OPENCL_DEVICE_HPP
#define OPENCL_DEVICE_HPP

#include "tasks/Task.hpp"

#include "compute/openclComputePlace.hpp"
#include "memory/openclMemoryPlace.hpp"

#include "hardware/places/NUMAPlace.hpp"

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif

#include "openclProgram.hpp"
#include <vector>


class openclDevice {
private:
  openclComputePlace  * _computePlace;               //!< ComputePlace associated to this device
  openclMemoryPlace * _memoryPlace;                 //!< MemoryPlace associated to this device

  int _index;                                                     //!< Index of the OpenCL device
  NUMAPlace *_numaPlace;                                  //!< NUMA node where this device is located
  cl::Device _device;
  openclProgram * _program;

public:
  openclDevice(int index, cl::Device device) : _index(index), _device(device)
  {
    _program = new openclProgram(device);

    _computePlace = new openclComputePlace(index, _program->getOpenCLProgram(), _program->getProgramContext(), _program->getProgramDevice());
    _memoryPlace = new openclMemoryPlace(index);
  }

	~openclDevice()
	{
		delete _program;
		delete _computePlace;
		delete _memoryPlace;
	}

        openclComputePlace *getComputePlace() const
        {
                return _computePlace;
        }

        openclMemoryPlace *getMemoryPlace() const
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

        std::string getDeviceName() const
        {
          std::string name;
          cl_int err = _device.getInfo(CL_DEVICE_NAME, &name);
          return name;
        }
};

#endif //OPENCL_DEVICE_HPP
