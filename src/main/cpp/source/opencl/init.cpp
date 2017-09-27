#pragma once
/*
MIT License

Copyright (c) 2017 ZetaChain_Native

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "platform.hpp" // Platform Specific Stuff NOTE: Must Always be the first include in a file
#include <vector>
#include <stdexcept>
#include <iostream>
#include "opencl/init.hpp"

namespace ZetaChain_Native::OpenCL {
	void init(){
		std::vector<cl_platform_id> platforms = getPlatforms();
		std::cout << "Found " << platforms.size() << " Usable OpenCL Platforms" << std::endl;
		cl_platform_id defaultPlatform = platforms[0];
		std::cout << "Using OpenCL Platform ID: " << reinterpret_cast<cl_uint>(defaultPlatform) << std::endl;
		std::vector<cl_device_id> devices = getDevices(defaultPlatform);
		std::cout << "Found " << devices.size() << " Usable OpenCL Devices" << std::endl;
		cl_context context = createContext(devices.size(), platforms, devices);
		if(context)
			std::cout << "Successfully Initialised OpenCL Context At: " << "0x" << reinterpret_cast<void*>(context) << std::endl;
		else
			throw std::runtime_error("Failed to Initialise OpenCL Context please update your drivers or restart the application with the --noOpenCL option");
	}

	std::vector<cl_platform_id> getPlatforms() {
		cl_uint platformCount = 0;
		clGetPlatformIDs(NULL, nullptr, &platformCount);
		if(platformCount == 0)
			throw std::runtime_error("No OpenCL platforms available please update your drivers or restart the application with the --noOpenCL option");
		std::vector<cl_platform_id> platforms = std::vector<cl_platform_id>(platformCount);
		clGetPlatformIDs(platformCount, platforms.data(), nullptr);
		return platforms;
	}

	std::vector<cl_device_id> getDevices(cl_platform_id platform) {
		cl_uint deviceCount = 0;
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, NULL, nullptr, &deviceCount);
		if(deviceCount == 0)
			throw std::runtime_error("No OpenCL devices available please update your drivers or restart the application with the --noOpenCL option");
		std::vector<cl_device_id> devices = std::vector<cl_device_id>(deviceCount);
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);
		return devices;
	}

	cl_context createContext(cl_uint deviceCount, std::vector<cl_platform_id> platforms, std::vector<cl_device_id> devices) {
		const cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(platforms[0]),
			0, 0
		};

		cl_int error = CL_SUCCESS;

		cl_context context = clCreateContext(properties, deviceCount, devices.data(), nullptr, nullptr, &error);
		checkError(error);
		return context;
	}

	void checkError(cl_int error) {
		if (error != CL_SUCCESS) {
			std::cerr << "OpenCL call failed with error " << error << " Please try and update your drivers or restart the application with the --noOpenCL option" << std::endl;
			std::exit(1);
		}
	}
}