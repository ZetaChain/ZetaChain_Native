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
#include <fstream>
#include "opencl/programarguments.hpp"
#include "opencl/kernelarguments.hpp"
#include "opencl/bufferarguments.hpp"
#include "opencl/commandqueuearguments.hpp"
#include "opencl/kernelargarguments.hpp"
#include "opencl/openclhandle.hpp"

namespace ZetaChain_Native::OpenCL {
	OpenCLHandle::OpenCLHandle(cl_platform_id defaultPlatform, cl_context context, std::vector<cl_platform_id> platforms, std::vector<cl_device_id> devices) {
		this->defaultPlatform = defaultPlatform;
		this->context = context;
		this->platforms = platforms;
		this->devices = devices;
	}

	OpenCLHandle::~OpenCLHandle() {
		if(this->context)
			clReleaseContext(this->context);
	}

	cl_mem OpenCLHandle::createBuffer(cl_mem_flags flags, size_t size, void* host_ptr, cl_int* errorcode) {
		return clCreateBuffer(this->context, flags, size, host_ptr, errorcode);
	}

	cl_mem OpenCLHandle::createBuffer(BufferArguments args) {
		return clCreateBuffer(this->context, args.flags, args.data_size, args.host_ptr, args.errorcode);
	}

	cl_command_queue OpenCLHandle::createCommandQueue(cl_device_id device, cl_command_queue_properties properties, cl_int* error) {
		return clCreateCommandQueue(this->context, device, properties, error);
	}

	cl_command_queue OpenCLHandle::createCommandQueue(CommandQueueArguments args) {
		return clCreateCommandQueue(this->context, args.device, args.properties, args.errcode_ret);
	}

	cl_kernel OpenCLHandle::createKernel(cl_program program, const char* kernel_name, cl_int* error) {
		return clCreateKernel(program, kernel_name, error);
	}

	cl_kernel OpenCLHandle::createKernel(KernelArguments args) {
		return clCreateKernel(args.program, args.kernel_name, args.errcode_ret);
	}
	cl_program OpenCLHandle::createProgram(const std::string& source) {
		size_t lengths[1] = { source.size() };
		const char* sources[1] = { source.data() };
	
		cl_int error = 0;
		cl_program program = clCreateProgramWithSource(this->context, 1, sources, lengths, &error);
	
		return program;
	}

	cl_int OpenCLHandle::buildProgram(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char* options, 
			void (*pfn_notify)(cl_program, void *user_data), void* user_data) {
			return clBuildProgram(program, num_devices, device_list, options, pfn_notify, user_data);
		}

	cl_int OpenCLHandle::buildProgram(ProgramArguments args) {
		return clBuildProgram(args.program, args.num_devices, args.device_list, args.options, args.pfn_notify, args.user_data);
	}

	void OpenCLHandle::checkError(cl_int error) {
		if (error != CL_SUCCESS) {
			std::cerr << "OpenCL call failed with error " << error << " Please update your drivers or restart the application with the --noOpenCL option" << std::endl;
			std::exit(1);
		}
	}

	std::string OpenCLHandle::getPlatformName(cl_platform_id platform) {
		size_t size = 0;
		clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &size);
	
		std::string result;
		result.resize(size);
		clGetPlatformInfo(platform, CL_PLATFORM_NAME, size,
			const_cast<char*>(result.data()), nullptr);
	
		return result;
	}

	std::string OpenCLHandle::getDeviceName(cl_device_id device) {
		size_t size = 0;
		clGetDeviceInfo(device, CL_DEVICE_NAME, 0, nullptr, &size);
	
		std::string result;
		result.resize(size);
		clGetDeviceInfo(device, CL_DEVICE_NAME, size,
			const_cast<char*>(result.data()), nullptr);
	
		return result;
	}

	std::string OpenCLHandle::loadKernel(const char* name) {
		std::ifstream in(name);
		std::string result(
			(std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		return result;
	}

	cl_int OpenCLHandle::setKernelArgument(cl_kernel kernel, cl_uint index, size_t size, const void* value) {
		return clSetKernelArg(kernel, index, size, value);
	}

	cl_int OpenCLHandle::setKernelArgument(KernelArgArguments args) {
		return clSetKernelArg(args.kernel, args.index, args.size, args.value);
	}

	cl_int OpenCLHandle::enqueueNDRangeKernel(cl_command_queue queue, cl_kernel kernel, cl_uint work, 
			const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size,
			cl_uint events_in_wait_list, const cl_event* wait_list, cl_event* event) {
			return clEnqueueNDRangeKernel(queue, kernel, work, global_work_offset, global_work_size, local_work_size, events_in_wait_list, wait_list, event);
			
		}

	cl_int OpenCLHandle::enqueueReadBuffer(cl_command_queue queue, cl_mem buffer, cl_bool blocking_read, size_t offset, 
			size_t size, void* host_mem, cl_uint events_in_wait_list, const cl_event* wait_list, cl_event* event) {
			return clEnqueueReadBuffer(queue, buffer, blocking_read, offset, size, host_mem, events_in_wait_list, wait_list, event);
		}

	cl_int OpenCLHandle::releaseCommandQueue(cl_command_queue cl_command_queue) {
		return clReleaseCommandQueue(cl_command_queue);
	}

	cl_int OpenCLHandle::releaseMemObject(cl_mem memobj) {
		return clReleaseMemObject(memobj);
	}

	cl_int OpenCLHandle::releaseKernel(cl_kernel kernel) {
		return clReleaseKernel(kernel);
	}

	cl_int OpenCLHandle::releaseProgram(cl_program program) {
		return clReleaseProgram(program);
	}

	cl_int OpenCLHandle::releaseContext(cl_context context) {
		return clReleaseContext(context);
	}

	cl_platform_id OpenCLHandle::getDefaultPlatform() {
		return this->defaultPlatform;
	}

	cl_context OpenCLHandle::getContext() {
		return this->context;
	}

	std::vector<cl_platform_id> OpenCLHandle::getPlatforms() {
		return this->platforms;
	}

	std::vector<cl_device_id> OpenCLHandle::getDevices() {
		return this->devices;
	}
}