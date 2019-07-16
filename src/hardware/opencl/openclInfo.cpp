
/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#include "openclInfo.hpp"

void openclInfo::initialize()
{
	cl_int err;
        std::vector<cl::Platform> platforms;
        err = cl::Platform::get(&platforms);
        openclErrorHandler::handle(err, "When getting platforms");

	auto platform = platforms.front();
        std::vector<cl::Device> devices;
        //To change. It depends on the type of device that we want
        err = platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        openclErrorHandler::handle(err, "When getting devices");

	_devices.resize(devices.size());
	_pollingServices.resize(devices.size());

	for (int i = 0; i < devices.size(); ++i) {
		_devices[i] = new openclDevice(i, devices[i]);
		_pollingServices[i] = new openclPollingService(_devices[i]);

		_pollingServices[i]->start();
	}
}

void openclInfo::shutdown()
{
        for (int i = 0; i < _devices.size(); ++i) {
                delete _devices[i];

                _pollingServices[i]->stop();
                delete _pollingServices[i];
        }
}
