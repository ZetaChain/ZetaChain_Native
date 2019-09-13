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
#include <map> // std::map
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <stdexcept> // throw throw std::runtime_error()
#include "transactions/transaction.hpp"
#include "transactions/transactiondata.hpp"

namespace ZetaChain_Native {

	class BlockData {
		public:

		virtual std::string computeHash() = 0;
		virtual std::vector<unsigned char> toBytes() = 0;
		virtual std::string toString() = 0;
		virtual bool verify() = 0;
		virtual bool lock(unsigned long timeout) = 0;

		protected:

		std::string hash = "";
		unsigned long transactionCount = 0;
		std::map<std::string, Transaction<TransactionData*>*> transactions = std::map<std::string, Transaction<TransactionData*>*>();
		unsigned long size = 0;
		unsigned long bits = 0;
		time_t timeCreated = -1;
		time_t timeRecieved = -1;
		time_t timeLocked = -1;
		
		private:
			void* rawData; // Only Exists to prevent template errors when reading and writing
	};
}