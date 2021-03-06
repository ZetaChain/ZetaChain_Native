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
#include "opencl/openclhandle.hpp"
#include "opencl/openclprogram.hpp"
#include "opencl/openclkernel.hpp"
#include "opencl/openclbuffer.hpp"
#include "opencl/openclcommandqueue.hpp"

namespace ZetaChain_Native {
	namespace OpenCL {
		class OpenCLLockingData {
		public:
			static OpenCLLockingData* getInstance() {
				if (instance == nullptr)
					instance = new OpenCLLockingData();
				return instance;
			}
			OpenCLHandle* handle;
			OpenCLProgram* currentProgram;
			OpenCLKernel* currentKernel;

			OpenCLBuffer<unsigned long>* currentABuffer;
			OpenCLBuffer<int>* currentBBuffer;

			OpenCLCommandQueue* currentCommandQueue;

			OpenCLLockingData(OpenCLLockingData const&) = delete;
			void operator=(OpenCLLockingData const&) = delete;
		private:
			OpenCLLockingData();
			static OpenCLLockingData* instance;
		};

		class OpenCLMiningData {
		public:
			static OpenCLMiningData* getInstance() {
				if (instance == nullptr)
					instance = new OpenCLMiningData();
				return instance;
			}
			OpenCLHandle* handle;
			OpenCLProgram* currentProgram;
			OpenCLKernel* currentKernel;

			OpenCLBuffer<void*>* currentABuffer;
			OpenCLBuffer<unsigned long>* currentBBuffer;
			OpenCLBuffer<long>* currentCBuffer;

			OpenCLCommandQueue* currentCommandQueue;

			OpenCLMiningData(OpenCLMiningData const&) = delete;
			void operator=(OpenCLMiningData const&) = delete;
		private:
			OpenCLMiningData();
			static OpenCLMiningData* instance;
		};
	}
}