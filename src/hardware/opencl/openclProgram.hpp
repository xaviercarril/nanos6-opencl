/*
        This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.

        Copyright (C) 2018 Barcelona Supercomputing Center (BSC)

        Author: Xavier Carril
*/

#ifndef OPENCL_PROGRAM_HPP
#define OPENCL_PROGRAM_HPP

#ifdef HAVE_OPENCL_CL_HPP
#include <OpenCL/cl.hpp>
#endif

#ifdef HAVE_CL_CL_HPP
#include <CL/cl.hpp>
#endif

#include<string>
#include<fstream>

#include<iostream>
using namespace std;

class openclProgram {
private:
	cl::Program _program;
	cl::Device _device;
	cl::Context _context;
public:

  openclProgram(/*const std::string& file,*/ cl::Device device): _device(device)
  {
		cl_int err;
		/*std::vector<cl::Platform> platforms;
		err = cl::Platform::get(&platforms);
		openclErrorHandler::handle(err, "When getting platforms");

		auto platform = platforms.front();
		std::vector<cl::Device> devices;
		//To change. It depends on the type of device that we want
		err = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		openclErrorHandler::handle(err, "When getting devices");*/

		//_device = device;

		//Temporal
		//std::string file = "kernel.cl";
		//std::istream kernelFile(&file);
		//std::istream kernelFile = new std::istream(file);
		std::filebuf file;
		file.open("kernel.cl", std::ios::in);
		std::istream kernelFile(&file);
		std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

		cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

		_context = cl::Context(_device);
		_program = cl::Program(_context, sources);
		
		cout << "Building Program" << cout;
		err = _program.build("-cl-std=CL1.2");
		openclErrorHandler::handle(err, "When building program");
	}

	~openclProgram()
	{
		cl_int err;
		cl_program pr = _program();
		err = clReleaseProgram(pr);
		openclErrorHandler::handle(err, "When destroying program");
	}

	cl::Program getOpenCLProgram()
	{
		return _program;
	}

	cl::Device getProgramDevice()
	{
		return _device;
	}

	cl::Context getProgramContext()
	{
		return _context;
	}

};
#endif //OPENCL_PROGRAM_HPP
