/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

	Copyright (C) 2018 Barcelona Supercomputing Center (BSC)
*/

#ifndef OPENCL_FATAL_ERROR_HANDLER_HPP
#define OPENCL_FATAL_ERROR_HANDLER_HPP

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

//#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
//#endif

#include "lowlevel/FatalErrorHandler.hpp"

class openclErrorHandler: public FatalErrorHandler {
private:

		static std::string getErrorString(cl_int error)
		{
			switch(error) {
    		// run-time and JIT compiler errors
    		case 0: return "CL_SUCCESS";
    		case -1: return "CL_DEVICE_NOT_FOUND";
    		case -2: return "CL_DEVICE_NOT_AVAILABLE";
    		case -3: return "CL_COMPILER_NOT_AVAILABLE";
    		case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    		case -5: return "CL_OUT_OF_RESOURCES";
    		case -6: return "CL_OUT_OF_HOST_MEMORY";
				case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    		case -8: return "CL_MEM_COPY_OVERLAP";
    		case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    		case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    		case -11: return "CL_BUILD_PROGRAM_FAILURE";
    		case -12: return "CL_MAP_FAILURE";
    		case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    		case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    		case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    		case -16: return "CL_LINKER_NOT_AVAILABLE";
    		case -17: return "CL_LINK_PROGRAM_FAILURE";
    		case -18: return "CL_DEVICE_PARTITION_FAILED";
    		case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    		// compile-time errors
    		case -30: return "CL_INVALID_VALUE";
    		case -31: return "CL_INVALID_DEVICE_TYPE";
	   		case -32: return "CL_INVALID_PLATFORM";
    		case -33: return "CL_INVALID_DEVICE";
    		case -34: return "CL_INVALID_CONTEXT";
    		case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    		case -36: return "CL_INVALID_COMMAND_QUEUE";
    		case -37: return "CL_INVALID_HOST_PTR";
    		case -38: return "CL_INVALID_MEM_OBJECT";
    		case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    		case -40: return "CL_INVALID_IMAGE_SIZE";
    		case -41: return "CL_INVALID_SAMPLER";
    		case -42: return "CL_INVALID_BINARY";
    		case -43: return "CL_INVALID_BUILD_OPTIONS";
    		case -44: return "CL_INVALID_PROGRAM";
    		case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    		case -46: return "CL_INVALID_KERNEL_NAME";
   			case -47: return "CL_INVALID_KERNEL_DEFINITION";
    		case -48: return "CL_INVALID_KERNEL";
    		case -49: return "CL_INVALID_ARG_INDEX";
    		case -50: return "CL_INVALID_ARG_VALUE";
    		case -51: return "CL_INVALID_ARG_SIZE";
    		case -52: return "CL_INVALID_KERNEL_ARGS";
    		case -53: return "CL_INVALID_WORK_DIMENSION";
    		case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    		case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    		case -56: return "CL_INVALID_GLOBAL_OFFSET";
    		case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    		case -58: return "CL_INVALID_EVENT";
    		case -59: return "CL_INVALID_OPERATION";
    		case -60: return "CL_INVALID_GL_OBJECT";
    		case -61: return "CL_INVALID_BUFFER_SIZE";
    		case -62: return "CL_INVALID_MIP_LEVEL";
    		case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    		case -64: return "CL_INVALID_PROPERTY";
    		case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    		case -66: return "CL_INVALID_COMPILER_OPTIONS";
    		case -67: return "CL_INVALID_LINKER_OPTIONS";
    		case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

   		 // extension errors
    		case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    		case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
   	 		case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
 	   		case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    		case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    		case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    		default: return "Unknown OpenCL error";
    	}
	}
	static inline void printOpenCL_Error(cl_int err, std::ostringstream &oss)
	{
		std::string errName = getErrorString(err);
		oss << errName << " ";
	}

public:

	template<typename... TS>
	static inline void handle(cl_int err, TS... reasonParts)
	{
		if (__builtin_expect(err == CL_SUCCESS, 1)) {
			return;
		}

		std::ostringstream oss;

		printOpenCL_Error(err, oss);
		emitReasonParts(oss, reasonParts...);
		oss << std::endl;

		{
			std::lock_guard<SpinLock> guard(_lock);
			std::cerr << oss.str();
		}

#ifndef NDEBUG
		abort();
#else
		exit(1);
#endif
	}

	template<typename... TS>
	static inline bool handleEvent(cl_int err, TS... reasonParts)
	{
		// std::string errName = getErrorString(err);
		if (__builtin_expect(err == CL_QUEUED || err == CL_SUBMITTED || err == CL_RUNNING || err == CL_SUCCESS, 1)) {
			if (err == CL_SUCCESS) {
				return true;
			} else {
				return false;
			}
		}

		std::ostringstream oss;

		printOpenCL_Error(err, oss);
		emitReasonParts(oss, reasonParts...);
		oss << std::endl;

		{
			std::lock_guard<SpinLock> guard(_lock);
			std::cerr << oss.str();
		}

#ifndef NDEBUG
		abort();
#else
		exit(1);
#endif
	}

	template<typename... TS>
	static inline void warnIf(bool failure, TS... reasonParts)
	{
		if (__builtin_expect(!failure, 1)) {
			return;
		}

		std::ostringstream oss;
		oss << "Warning: ";
		emitReasonParts(oss, reasonParts...);
		oss << std::endl;

		{
			std::lock_guard<SpinLock> guard(_lock);
			std::cerr << oss.str();
		}
	}
};

#endif //OPENCL_FATAL_ERROR_HANDLER
