/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
	
	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

	Author: Xavier Carril
*/

#ifndef OPENCL_HELPER_HPP
#define OPENCL_HELPER_HPP

#include <string>

#include "lowlevel/SpinLock.hpp" 

#include "hardware/opencl/openclDevice.hpp"

class Task;

class openclPollingService {

private:
	std::string _serviceName;
	openclDevice *_device;
	SpinLock _lock;
	
	void finishTask(Task *task);
	void launchTask(Task *task);	
public:	
	openclPollingService(openclDevice *device);
	
	openclPollingService(openclPollingService const &) = delete;
	openclPollingService operator=(openclPollingService const &) = delete;
	
	~openclPollingService();
	
	//! \brief Register the polling service 
	void start();
	
	//! \brief Unregister the polling service 
	void stop();
	
	//! \brief Main function of the helper 	
	void run();
	
	//! \brief Static function used by the PollingAPI 
	static bool runHelper(void *helper_ptr);
};


#endif // OPENCL_HELPER_HPP

