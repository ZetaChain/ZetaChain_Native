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
#include <ctime>
#include <map>
#include <vector>
#include "transactions/transaction.hpp"
#include "transactions/transactioninput.hpp"
#include "transactions/transactionoutput.hpp"
#include "blocks/block.hpp"
#include "blockdata/blockdata.hpp"
#include "blockchains/blockchain.hpp"
#include "io/serialisation.hpp"

namespace BlockchainCpp::IO::Serialisation {
	bool writeChar(std::ofstream* stream, char data) {
		*stream << data;
		return true;
	}
	bool writeShort(std::ofstream* stream, short data) {
		*stream << data;
		return true;
	}
	bool writeInt(std::ofstream* stream, int data) {
		*stream << data;
		return true;
	}
	bool writeLong(std::ofstream* stream, long data) {
		*stream << data;
		return true;
	}
	bool writeLongLong(std::ofstream* stream, long long data) {
		*stream << data;
		return true;
	}
	bool writeUnsignedChar(std::ofstream* stream, unsigned char data) {
		*stream << data;
		return true;
	}
	bool writeUnsignedShort(std::ofstream* stream, unsigned short data) {
		*stream << data;
		return true;
	}
	bool writeUnsignedInt(std::ofstream* stream, unsigned int data) {
		*stream << data;
		return true;
	}
	bool writeUnsignedLong(std::ofstream* stream, unsigned long data) {
		*stream << data;
		return true;
	}
	bool writeUnsignedLongLong(std::ofstream* stream, unsigned long long data) {
		*stream << data;
		return true;
	}
	bool writeFloat(std::ofstream* stream, float data) {
		*stream << data;
		return true;
	}
	bool writeDouble(std::ofstream* stream, double data) {
		*stream << data;
		return true;
	}
	bool writeString(std::ofstream* stream, std::string data) {
		*stream << data;
		*stream << "\0";
		return true;
	}

	bool writeTransactionInput(std::ofstream* stream, TransactionInput* data) {
		writeString(stream, data->getHash());
		writeString(stream, data->getAddress());
		writeDouble(stream, data->getValue());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeLocked());
		return true;
	}
	bool writeTransactionOutput(std::ofstream* stream, TransactionOutput* data) {
		writeString(stream, data->getHash());
		writeString(stream, data->getAddress());
		writeDouble(stream, data->getValue());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeLocked());
		return true;
	}

	bool writeTransactionData(std::ofstream* stream, TransactionData* data) {
		writeString(stream, data->getHash());
		writeUnsignedLong(stream, data->getSize());
		return true;
	}

	char* readChar(std::ifstream* stream) {
		char ch;
		*stream >> ch;
		return &ch;
	}
	short* readShort(std::ifstream* stream) {
		short s;
		*stream >> s;
		return &s;
	}
	int* readInt(std::ifstream* stream) {
		int i;
		*stream >> i;
		return &i;
	}
	long* readLong(std::ifstream* stream) {
		long l;
		*stream >> l;
		return &l;
	}
	long long* readLongLong(std::ifstream* stream) {
		long long ll;
		*stream >> ll;
		return &ll;
	}
	unsigned char* readUnsignedChar(std::ifstream* stream) {
		unsigned char uch;
		*stream >> uch;
		return &uch;
	}
	unsigned short* readUnsignedShort(std::ifstream* stream) {
		unsigned short us;
		*stream >> us;
		return &us;
	}
	unsigned int* readUnsignedInt(std::ifstream* stream) {
		unsigned int ui;
		*stream >> ui;
		return &ui;
	}
	unsigned long* readUnsignedLong(std::ifstream* stream) {
		unsigned long ul;
		*stream >> ul;
		return &ul;
	}
	unsigned long long* readUnsignedLongLong(std::ifstream* stream) {
		unsigned long long ull;
		*stream >> ull;
		return &ull;
	}
	float* readFloat(std::ifstream* stream) {
		float f;
		*stream >> f;
		return &f;
	}
	double* readDouble(std::ifstream* stream) {
		double d;
		*stream >> d;
		return &d;
	}
	std::string* readString(std::ifstream* stream) {
		std::string str;
		*stream >> str;
		return &str;
	}
	TransactionInput* readTransactionInput(std::ifstream* stream) {
		std::string hash = *readString(stream);
		std::string address = *readString(stream);
		double value = *readDouble(stream);
		time_t timeCreated = *readLongLong(stream);
		time_t timeLocked = *readLongLong(stream);
		TransactionInput* result = new TransactionInput();
		result->setHash(hash);
		result->setAddress(address);
		result->setValue(value);
		result->setTimeCreated(timeCreated);
		result->setTimeLocked(timeLocked);
		return result;
	}
	TransactionOutput* readTransactionOutput(std::ifstream* stream) {
		std::string hash = *readString(stream);
		std::string address = *readString(stream);
		double value = *readDouble(stream);
		time_t timeCreated = *readLongLong(stream);
		time_t timeLocked = *readLongLong(stream);
		TransactionOutput* result = new TransactionOutput();
		result->setHash(hash);
		result->setAddress(address);
		result->setValue(value);
		result->setTimeCreated(timeCreated);
		result->setTimeLocked(timeLocked);
		return result;
	}
	TransactionData* readTransactionData(std::ifstream* stream) {
		std::string hash = *readString(stream);
		unsigned long size = *readUnsignedLong(stream);
		// TransactionData* result = new TransactionData(); // TODO fix
		// result->setHash(hash);
		// result->setSize(size);
		return nullptr;
	}
}