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
#include <string> // std::string
#include <vector>
#include <ctime>
#include "conversions.hpp"
#include "operators.hpp"
#include "hashing.hpp"
#include "thirdparty/json.hpp"
#include "opencl/init.hpp"
#include "opencl/openclhandle.hpp"
#include "opencl/opencllockingdata.hpp"
#include "opencl/programarguments.hpp"
#include "opencl/kernelarguments.hpp"
#include "opencl/commandqueuearguments.hpp"
#include "opencl/ndrangekernelarguments.hpp"
#include "opencl/opencllockingdata.hpp"
#include "opencl/readbufferarguments.hpp"
#include "transactions/transactiondata.hpp"

extern bool __noOpenCL;

extern "C" void lockTransactionDataASM(unsigned long timeout);

namespace ZetaChain_Native {

	TransactionData::TransactionData() {

	}

	TransactionData::~TransactionData() {

	}

	std::string TransactionData::computeHash() {
		return Hashing::hashVector(this->toBytes());
	}

	std::vector<unsigned char> TransactionData::toBytes() {
		std::vector<unsigned char> bytes = std::vector<unsigned char>();
		bytes += Conversions::toBytes(this->hash);
		bytes += Conversions::toBytes(&this->size);
		bytes += Conversions::toBytes(&this->timeLocked);
		return bytes;
	}

	std::string TransactionData::toString() {
		nlohmann::json j;

		j["hash", this->hash];
		j["size", this->size];
		j["timeLocked", this->timeLocked];

		return j;
	}

	bool TransactionData::lock(unsigned long timeout = 1000) {
		if(this->timeLocked != 0)
		return true;
	
		if(!__noOpenCL) {
			OpenCL::OpenCLLockingData* lockingData = OpenCL::OpenCLLockingData::getInstance();
			if(!lockingData->handle){
				lockingData->handle = OpenCL::init();
				if(!lockingData->handle){
					throw std::runtime_error("Failed to Initialise OpenCL Handle please update your drivers or restart the application with the --noOpenCL option");
				}
			}
			else {
				cl_program program = lockingData->handle->createProgram(lockingData->handle->loadKernel("kernels/locktransactiondata.cl"));
				lockingData->currentProgram = new OpenCL::OpenCLProgram(program, "kernels/locktransactiondata.cl", &lockingData->handle);
				if(!lockingData->currentProgram) {
					throw std::runtime_error("Failed to Create Program with Kernel Code kernels/locktransactiondata.cl");
				}
				OpenCL::ProgramArguments pArgs = {
					program,
					lockingData->handle->getDevices().size(),
					lockingData->handle->getDevices().data(),
					nullptr,
					nullptr,
					nullptr
				};
				lockingData->handle->checkError(lockingData->handle->buildProgram(pArgs));
				cl_int error = CL_SUCCESS;
				OpenCL::KernelArguments kArgs = {
					lockingData->currentProgram->getProgram(),
					"lockblock.cl",
					&error
				};
				cl_kernel kernel = lockingData->handle->createKernel(kArgs);
				lockingData->currentKernel = new OpenCL::OpenCLKernel(kernel, "kernels/locktransactiondata.cl", &lockingData->handle);
				lockingData->handle->checkError(error);
				unsigned long kernelData = timeout + 1; // Adding 1 because of OpenCL control thread
				const size_t KERNEL_DATA_SIZE = sizeof(kernelData);
				OpenCL::BufferArguments aBufferArgs = {
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					KERNEL_DATA_SIZE,
					reinterpret_cast<void*>(&kernelData),
					&error
				};
				cl_mem aBuffer = lockingData->handle->createBuffer(aBufferArgs);
				lockingData->currentABuffer = new OpenCL::OpenCLBuffer<unsigned long>(aBuffer, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, kernelData, &lockingData->handle);
				int result = -1;
				OpenCL::BufferArguments bBufferArgs = {
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					sizeof(int),
					reinterpret_cast<void*>(&result),
					&error
				};
				cl_mem bBuffer = lockingData->handle->createBuffer(bBufferArgs);
				lockingData->currentBBuffer = new OpenCL::OpenCLBuffer<int>(bBuffer, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, kernelData, &lockingData->handle);
				OpenCL::CommandQueueArguments commandArgs = {
					lockingData->handle->getDevices()[0],
					NULL,
					&error
				};
				cl_command_queue commandQueue = lockingData->handle->createCommandQueue(commandArgs);
				lockingData->currentCommandQueue = new OpenCL::OpenCLCommandQueue(commandQueue, lockingData->handle->getDevices()[0], NULL, &lockingData->handle);
				OpenCL::KernelArgArguments arg_iterations = {
					kernel,
					0,
					sizeof(cl_mem),
					&aBuffer
				};
				lockingData->handle->setKernelArgument(arg_iterations);
				OpenCL::KernelArgArguments arg_result = {
					kernel,
					1,
					sizeof(cl_mem),
					&bBuffer
				};
				lockingData->handle->setKernelArgument(arg_result);
				const size_t globalWorkSize[] = { KERNEL_DATA_SIZE, 0, 0 };
				OpenCL::NDRangeKernelArguments ndArgs = {
					commandQueue,
					kernel,
					1,
					nullptr,
					0,
					nullptr,
					NULL
				};
				lockingData->handle->checkError(lockingData->handle->enqueueNDRangeKernel(ndArgs));
				OpenCL::ReadBufferArguments readArgs = {
					commandQueue,
					bBuffer,
					CL_TRUE,
					0,
					sizeof(int),
					reinterpret_cast<void*>(&result),
					0,
					nullptr,
					nullptr
				};
				lockingData->handle->checkError(lockingData->handle->enqueueReadBuffer(readArgs));
				lockingData->handle->releaseCommandQueue(commandQueue);
				lockingData->handle->releaseMemObject(bBuffer);
				lockingData->handle->releaseMemObject(aBuffer);
				lockingData->handle->releaseKernel(kernel);
				lockingData->handle->releaseProgram(program);

				delete lockingData->currentCommandQueue;
				delete lockingData->currentBBuffer;
				delete lockingData->currentABuffer;
				delete lockingData->currentKernel;
				delete lockingData->currentProgram;
				delete lockingData->handle;
			}
		}
		else {
			lockTransactionDataASM(timeout);
		}

		time_t now;
		time(&now);
		struct tm* timeinfo;
		timeinfo = localtime(&now);
		this->timeLocked = now - (timeout / 1000);
		time_t _time = this->timeLocked;
		this->hash = computeHash();
		std::cout << "Transaction Data has Sucessfully been locked" << std::endl;
		return this->timeLocked != 0;;
	}
}