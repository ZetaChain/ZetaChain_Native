/*
MIT License

Copyright (c) 2017 Blockchain-VCS

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
#include "conversions.hpp" // toBytes()
#include "thirdparty/json.hpp"
#include "operators.hpp"
#include "transactionoutput.hpp"
#include "hashing.hpp"

namespace BlockchainCpp {
	std::vector<unsigned char> TransactionOutput::toBytes() {
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(TransactionOutput));
		bytes += Conversions::toBytes(this->hash);
		bytes += Conversions::toBytes(this->address);
		bytes += Conversions::toBytes(&this->value);
		bytes += Conversions::toBytes(&this->timeCreated);
		bytes += Conversions::toBytes(&this->timeLocked);
		return bytes;
	}

	std::string TransactionOutput::toString() {
		nlohmann::json j;
		j["type", "TransactionOutput"];
		j["hash", this->hash];
		j["address", this->address];
		j["value", this-> value];
		j["timeCreated", this->timeCreated];
		j["timeLocked", this->timeLocked];
		return j;
	}

	bool TransactionOutput::verify() {
		return this->hash == computeHash();
	}

	std::string TransactionOutput::computeHash() {
		return Hashing::hashVector(this->toBytes());
	}
}
