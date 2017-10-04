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
#include <map> // std::map
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <stdexcept> // throw throw std::runtime_error()
#include "thirdparty/json.hpp"
#include "operators.hpp"
#include "conversions.hpp" // toBytes()
#include "transactions/transaction.hpp"
#include "blockdata/longblockdata.hpp"
#include "hashing.hpp"
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

extern "C" void lockBlockDataASM(unsigned long timeout);

namespace ZetaChain_Native {
	LongBlockData::LongBlockData(long data){
		this->rawData = data;
	}

	LongBlockData::~LongBlockData(){

	}

	std::string LongBlockData::computeHash(){
		return Hashing::hashVector(this->toBytes());
	}

	std::vector<unsigned char> LongBlockData::toBytes(){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(LongBlockData));

 		std::vector<std::string> values = Conversions::mapToValuesString(this->transactions, this->transactions.size());
		
		for(int i = 0; i < values.size() - 1; i++)
			bytes += Conversions::toBytes(values[i]);

		bytes += Conversions::toBytes(&this->size);
		bytes += Conversions::toBytes(&this->transactionCount);
		bytes += Conversions::toBytes(&this->bits);
		bytes += Conversions::toBytes(&this->timeCreated);
		bytes += Conversions::toBytes(&this->timeRecieved);
		bytes += Conversions::toBytes(&this->timeLocked);
		bytes += Conversions::toBytes(&this->rawData);
		return bytes;
	}

	std::string LongBlockData::toString(){
		nlohmann::json j;
		nlohmann::json transactions = nlohmann::json::array();
		std::vector<std::string> values = Conversions::mapToValuesString(this->transactions, this->transactions.size());

		for(int i = 0; i < this->transactions.size() - 1; i++) {
			if(values.size() == 0)
				break;
			nlohmann::json obj = values[i];
			j.push_back(obj);
		}
		
		j["type", "BlockData"];
		j["hash", this->hash];
		j["size", this->size];
		j["transactions"] = transactions.dump();
		j["transactionCount", this->transactionCount];
		j["bits", this->bits];
		j["timeCreated", this->timeCreated];
		j["timeRecieved", this->timeRecieved];
		j["timeLocked", this->timeLocked];
		j["rawData", this->rawData];
		return j;
	}

	bool LongBlockData::verify(){
		return this->hash == computeHash();
	}

	bool LongBlockData::lock(unsigned long timeout = 1000) {
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
				cl_program program = lockingData->handle->createProgram(lockingData->handle->loadKernel("kernels/lockblockdata.cl"));
				lockingData->currentProgram = new OpenCL::OpenCLProgram(program, "kernels/lockblockdata.cl", &lockingData->handle);
				if(!lockingData->currentProgram) {
					throw std::runtime_error("Failed to Create Program with Kernel Code kernels/lockblockdata.cl");
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
				lockingData->currentKernel = new OpenCL::OpenCLKernel(kernel, "kernels/lockblockdata.cl", &lockingData->handle);
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
			lockBlockDataASM(timeout);
		}

		time_t now;
		time(&now);
		struct tm* timeinfo;
		timeinfo = localtime(&now);
		this->timeLocked = now - (timeout / 1000);
		time_t _time = this->timeLocked;
		this->hash = computeHash();
		std::cout << "Block Data has Sucessfully been locked" << std::endl;
		return this->timeLocked != 0;;
	}


	std::string LongBlockData::getHash(){
		return this->hash;
	}

	std::map<std::string, Transaction<TransactionData*>*> LongBlockData::getTransactions(){
		return static_cast<std::map<std::string, Transaction<TransactionData*>*>>(this->transactions);
	}

	unsigned long LongBlockData::getSize(){
		return this->size;
	}

	unsigned long LongBlockData::getTransactionCount(){
		return this->transactionCount;
	}

	unsigned long LongBlockData::getBits(){
		return this->bits;
	}

	time_t LongBlockData::getTimeCreated(){
		return this->timeCreated;
	}

	time_t LongBlockData::getTimeRecieved(){
		return this->timeRecieved;
	}

	time_t LongBlockData::getTimeLocked(){
		return this->timeLocked;
	}

	long LongBlockData::getRawData(){
		return this->rawData;
	}

	void LongBlockData::setHash() {
		if(this->hash != "")
			throw std::runtime_error("Hash has already been set");
		this->hash = computeHash();
	}

	void LongBlockData::setHash(std::string hash) {
		if(this->hash != "")
			throw std::runtime_error("Hash has already been set");
		this->hash = hash;
	}

	void LongBlockData::setTransactions(std::map<std::string, Transaction<TransactionData*>*> transactions){
		if(this->transactions.size() != 0)
			throw std::runtime_error("Transactions have already been set");
		this->transactions = transactions;
	}

	void LongBlockData::setSize(unsigned long size){
		if(this->size != 0)
			throw std::runtime_error("Size has already been set");
		this->size = size;
	}

	void LongBlockData::setTransactionCount(unsigned long count){
		if(this->transactionCount != 0)
			throw std::runtime_error("Transaction Count has already been set");
		this->transactionCount = count;
	}

	void LongBlockData::setBits(unsigned long bits){
		if(this->bits != 0)
			throw std::runtime_error("Bits has already been set");
		this->bits = bits;
	}

	void LongBlockData::setTimeCreated(time_t timeCreated){
		if(this->timeCreated != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeCreated);
			time_t _time = this->timeCreated;
			throw std::runtime_error("Block Data was already created at");
		}
		this->timeCreated = timeCreated;
	}

	void LongBlockData::setTimeRecieved(time_t timeRecieved){
		if(this->timeRecieved != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeRecieved);
			time_t _time = this->timeRecieved;
			throw std::runtime_error("Block Data was already recieved");
		}
		this->timeRecieved = timeRecieved;
	}

	void LongBlockData::setTimeLocked(time_t timeLocked){
		if(this->timeLocked != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeLocked);
			time_t _time = this->timeLocked;
			throw std::runtime_error("Block Data was already locked");
		}
		this->timeLocked = timeLocked;
	}

	void LongBlockData::setRawData(long rawData) {
		this->rawData = rawData;
	}
}
