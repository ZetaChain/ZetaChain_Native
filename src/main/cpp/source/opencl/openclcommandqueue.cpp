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
#include "opencl/openclhandle.hpp"
#include "opencl/opencldata.hpp"
#include "opencl/openclcommandqueue.hpp"

namespace ZetaChain_Native::OpenCL {
	OpenCLCommandQueue::OpenCLCommandQueue(cl_command_queue queue, cl_device_id device, cl_command_queue_properties properties, OpenCLHandle** handle) {
		this->queue = queue;
		this->device = device;
		this->properties = properties;
		this->handle = handle;
	}

	OpenCLCommandQueue::~OpenCLCommandQueue() {
		(*handle)->releaseCommandQueue(this->queue);
	}

	bool OpenCLCommandQueue::isHandleValid() {
		return OpenCLLockingData::getInstance()->handle == (*this->handle);
	}

	cl_command_queue OpenCLCommandQueue::getQueue() {
		return this->queue;
	}

	cl_device_id OpenCLCommandQueue::getDevice() {
		return this->device;
	}

	cl_command_queue_properties OpenCLCommandQueue::getProperties() {
		return this->properties;
	}

	OpenCLHandle* OpenCLCommandQueue::getHandle() {
		return *this->handle;
	}
}