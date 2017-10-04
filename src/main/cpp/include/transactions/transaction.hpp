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
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <stdexcept> // throw throw std::runtime_error()
#include <map> // std::map
#include <vector> // std::vector
#include "transactions/transactiondata.hpp"
#include "transactions/transactioninput.hpp"
#include "transactions/transactionoutput.hpp"
#include "conversions.hpp"
#include "operators.hpp"
#include "hashing.hpp"
#include "thirdparty/json.hpp"
#include "opencl/init.hpp"
#include "opencl/openclhandle.hpp"
#include "opencl/opencldata.hpp"
#include "opencl/programarguments.hpp"
#include "opencl/kernelarguments.hpp"
#include "opencl/commandqueuearguments.hpp"
#include "opencl/ndrangekernelarguments.hpp"
#include "opencl/opencldata.hpp"
#include "opencl/readbufferarguments.hpp"

extern bool __noOpenCL;

extern "C" void lockTransactionASM(unsigned long timeout);

namespace ZetaChain_Native {
	
	template <class T>
	
	class Transaction {
		public:
			
			std::vector<unsigned char> toBytes() {
				std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(Transaction<T>));
				bytes += Conversions::toBytes(this->hash);
				std::map<std::string, TransactionInput*>::iterator itr;
				for(itr = this->inputs.begin(); itr != this->inputs.end(); itr++){
					bytes += itr->second->toBytes();
				}
				std::map<std::string, TransactionOutput*>::iterator itr2;
				for(itr2 = this->outputs.begin(); itr2 != this->outputs.end(); itr2++){
					bytes += itr2->second->toBytes();
				}
				bytes += Conversions::toBytes(&this->value);
				bytes += Conversions::toBytes(&this->timeCreated);
				bytes += Conversions::toBytes(&this->timeLocked);
				bytes += Conversions::toBytes(&this->timeConfirmed);
				bytes += data->toBytes();
				return bytes;
			}

			std::string toString() {
				nlohmann::json j;
				nlohmann::json inputArr = nlohmann::json::array();
				nlohmann::json outputArr = nlohmann::json::array();

				std::vector<std::string> inputValues = Conversions::mapToValuesString(this->inputs, this->inputs.size());
				std::vector<std::string> outputValues = Conversions::mapToValuesString(this->outputs, this->outputs.size());
				
				for(int i = 0; i < inputValues.size() - 1; i++) {
					nlohmann::json obj = inputValues[i];
					inputArr.push_back(obj);
				}

				for(int i = 0; i < outputValues.size() - 1; i++){
					nlohmann::json obj = outputValues[i];
					outputArr.push_back(obj);
				}

				nlohmann::json rawData = this->data->toString();
				
				j["hash", this->hash];
				j["inputCount", this->inputCount];
				j["outputCount", this->outputCount];
				j["inputs", inputArr.dump()];
				j["outputs", outputArr.dump()];
				j["value", this->value];
				j["timeCreated", this->timeCreated];
				j["timeLocked", this->timeLocked];
				j["timeConfirmed", this->timeConfirmed];
				j["data", rawData.dump()];
				return j;
			}
			
			bool verify() {
				return this->computeHash() == this->hash;
			}

			bool lock(unsigned long timeout = 1000) {
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
						cl_program program = lockingData->handle->createProgram(lockingData->handle->loadKernel("kernels/locktransaction.cl"));
						lockingData->currentProgram = new OpenCL::OpenCLProgram(program, "kernels/locktransaction.cl", &lockingData->handle);
						if(!lockingData->currentProgram) {
							throw std::runtime_error("Failed to Create Program with Kernel Code kernels/locktransaction.cl");
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
						lockingData->currentKernel = new OpenCL::OpenCLKernel(kernel, "kernels/locktransaction.cl", &lockingData->handle);
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
					lockTransactionASM(timeout);
				}
		
				time_t now;
				time(&now);
				struct tm* timeinfo;
				timeinfo = localtime(&now);
				this->timeLocked = now - (timeout / 1000);
				time_t _time = this->timeLocked;
				this->hash = computeHash();
				std::cout << "Transaction has Sucessfully been locked" << std::endl;
				return this->timeLocked != 0;;
			}

			std::string getHash() {
				return hash;
			}

			void setHash(std::string hash) {
				this->hash = hash;
			}

			std::map<std::string, TransactionInput*> getInputs() {
				return inputs;
			}

			int getInputCount() {
				return inputCount;
			}

			void setInputs(std::map<std::string, TransactionInput*> inputs) {
				this->inputCount = inputs.size();
				this->inputs = inputs;
			}

			std::map<std::string, TransactionOutput*> getOutputs() {
				return outputs;
			}

			int getOutputCount() {
				return outputCount;
			}

			void setOutputs(std::map<std::string, TransactionOutput*> outputs) {
				this->outputCount = outputs.size();
				this->outputs = outputs;
			}

			double getValue() {
				return value;
			}

			void setValue(double value) {
				this->value = value;
			}

			time_t getTimeCreated() {
				return timeCreated;
			}

			void setTimeCreated(time_t time_created) {
				timeCreated = time_created;
			}

			time_t getTimeLocked() {
				return timeLocked;
			}

			void setTimeLocked(time_t time_locked) {
				timeLocked = time_locked;
			}

			time_t getTimeConfirmed() {
				return timeConfirmed;
			}

			void setTimeConfirmed(time_t time_confirmed) {
				timeConfirmed = time_confirmed;
			}

			T getData() {
				return data;
			}

			void setData(T data) {
				this->data = data;
			}


			friend bool operator==(const Transaction& lhs, const Transaction& rhs) {
				return lhs.hash == rhs.hash
					&& lhs.inputs == rhs.inputs
					&& lhs.outputs == rhs.outputs
					&& lhs.value == rhs.value
					&& lhs.timeCreated == rhs.timeCreated
					&& lhs.timeLocked == rhs.timeLocked
					&& lhs.timeConfirmed == rhs.timeConfirmed
					&& lhs.data == rhs.data;
			}

			friend bool operator!=(const Transaction& lhs, const Transaction& rhs) {
				return !(lhs == rhs);
			}

		protected:

		private:

			std::string computeHash() {
				return Hashing::hashString(this->toBytes());
			}

			std::string hash;
			int inputCount = 0;
			int outputCount = 0;
			std::map<std::string, TransactionInput*> inputs;
			std::map<std::string, TransactionOutput*> outputs;
			double value;
			time_t timeCreated;
			time_t timeLocked;
			time_t timeConfirmed;
			T data;
	};
}