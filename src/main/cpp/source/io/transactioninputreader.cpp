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
#include <string>
#include <fstream>
#include "thirdparty/json.hpp"
#include "io/serialisation.hpp"
#include "io/transactioninputreader.hpp"
#include "transactions/transactioninput.hpp"
#include "constants.hpp"

namespace BlockchainCpp::IO {
	TransactionInputReader::TransactionInputReader(std::string filePath, bool binary) {
		this->filePath = filePath;
		this->binary = binary;
		if(this->binary)
			file.open(this->filePath, std::fstream::in | std::fstream::binary);
		else
			file.open(this->filePath, std::fstream::in);
	}

	TransactionInputReader::~TransactionInputReader() {
		file.close();
		free(header);
	}

	TransactionInput* TransactionInputReader::read() {
		if(this->binary) {
			header[0] = *Serialisation::readUnsignedChar(&file);
			header[1] = *Serialisation::readUnsignedChar(&file);
			header[2] = *Serialisation::readUnsignedChar(&file);
			header[3] = *Serialisation::readUnsignedChar(&file);
			if(!this->verify())
				return nullptr;
			input = Serialisation::readTransactionInput(&file);
			if(!input->verify())
				return nullptr;
			return input;
		}
		else {
			nlohmann::json j = nlohmann::json::parse(file);
			if(j["type"] != "TransactionInput")
				return nullptr;
			TransactionInput* result = new TransactionInput();
			result->setHash(j["hash"]);
			result->setAddress(j["address"]);
			result->setValue(j["value"]);
			result->setTimeCreated(j["timeCreated"]);
			result->setTimeLocked(j["timeLocked"]);
			if(!result->verify())
				return nullptr;
			return result;
		}
	}

	bool TransactionInputReader::verify() {
		if(header[0] != TRANSACTION_INPUT_HEADER[0] || header[1] != TRANSACTION_INPUT_HEADER[1] || 
			header[2] != TRANSACTION_INPUT_HEADER[2] || header[3] != TRANSACTION_INPUT_HEADER[3])
			return false;
		return true;
	}

	void TransactionInputReader::close() {
		file.close();
	}
}