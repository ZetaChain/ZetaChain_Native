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
#include <string>

namespace ZetaChain_Native::OpenCL {
	class OpenCLHandle {
	public:
		OpenCLHandle(cl_platform_id defaultPlatform, cl_context context, std::vector<cl_platform_id> platforms, std::vector<cl_device_id> devices);
		~OpenCLHandle();

		cl_mem createBuffer(cl_mem_flags flags, size_t size, void* host_ptr, cl_int* errorcode);
		cl_command_queue createCommandQueue(cl_device_id device, cl_command_queue_properties properties, cl_int* error);
		cl_kernel createKernel(cl_program program, const char* kernel_name, cl_int* error);
		cl_program createProgram(const std::string& source);
		cl_int buildProgram(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options, 
			void (*pfn_notify)(cl_program, void *user_data), void *user_data);
		void checkError(cl_int error);
		std::string getPlatformName(cl_platform_id platform);
		std::string getDeviceName(cl_device_id device);
		std::string loadKernel(const char* name);
		cl_int setKernelArgument(cl_kernel kernel, cl_uint index, size_t size, const void* value);
		cl_int enqueueNDRangeKernel(cl_command_queue queue, cl_kernel kernel, cl_uint work,
			const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size,
			cl_uint events_in_wait_list, const cl_event* wait_list, cl_event* event);
		cl_int enqueueReadBuffer(cl_command_queue queue, cl_mem buffer, cl_bool blocking_read, size_t offset, 
			size_t size, void* host_mem, cl_uint events_in_wait_list, const cl_event* wait_list, cl_event* event);
		cl_int releaseCommandQueue(cl_command_queue cl_command_queue);
		cl_int releaseMemObject(cl_mem memobj);
		cl_int releaseKernel(cl_kernel kernel);
		cl_int releaseProgram(cl_program program);
		cl_int releaseContext(cl_context context);

		cl_platform_id getDefaultPlatform();
		cl_context getContext();
		std::vector<cl_platform_id> getPlatforms();
		std::vector<cl_device_id> getDevices();

	private:
		cl_platform_id defaultPlatform;
		cl_context context;
		std::vector<cl_platform_id> platforms;
		std::vector<cl_device_id> devices;
	};
}