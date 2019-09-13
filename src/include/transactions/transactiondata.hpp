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
#include <vector>
#include <ctime>

namespace ZetaChain_Native {

	class TransactionData {
	public:
		TransactionData();
		~TransactionData();

		std::string getHash() {
			return hash;
		}

		void setHash(std::string hash) {
			this->hash = hash;
		}

		unsigned long getSize() {
			return size;
		}

		void setSize(unsigned long size) {
			this->size = size;
		}

		time_t getTimeLocked() {
			return this->timeLocked;
		}

		void setTimeLocked(time_t timeLocked) {
			this->timeLocked = timeLocked;
		}

		friend bool operator==(const TransactionData& lhs, const TransactionData& rhs) {
			return lhs.hash == rhs.hash
				&& lhs.size == rhs.size;
		}

		friend bool operator!=(const TransactionData& lhs, const TransactionData& rhs) {
			return !(lhs == rhs);
		}

		std::string computeHash();
		std::vector<unsigned char> toBytes();
		std::string toString();
		bool lock(unsigned long timeout);

	private:
		std::string hash;
		unsigned long size;
		time_t timeLocked;
	};
}