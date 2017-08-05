#pragma once
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

namespace BlockchainCpp::IO::Serialisation {
	bool writeChar(std::ofstream* stream, char data);
	bool writeShort(std::ofstream* stream, short data);
	bool writeInt(std::ofstream* stream, int data);
	bool writeLong(std::ofstream* stream, long data);
	bool writeLongLong(std::ofstream* stream, long long data);
	bool writeUnsignedChar(std::ofstream* stream, unsigned char data);
	bool writeUnsignedShort(std::ofstream* stream, unsigned short data);
	bool writeUnsignedInt(std::ofstream* stream, unsigned int data);
	bool writeUnsignedLong(std::ofstream* stream, unsigned long data);
	bool writeUnsignedLongLong(std::ofstream* stream, unsigned long long data);
	bool writeFloat(std::ofstream* stream, float data);
	bool writeDouble(std::ofstream* stream, double data);
	bool writeString(std::ofstream* stream, std::string data);

	bool writeTransactionInput(std::ofstream* stream, TransactionInput* data);
	bool writeTransactionOutput(std::ofstream* stream, TransactionOutput* data);
	bool writeTransactionData(std::ofstream* stream, TransactionData* data);

	template <class T>
	bool writeTransaction(std::ofstream* stream, T* data) {
		writeString(stream, data->getHash());
		writeInt(stream, data->getInputCount());
		writeInt(stream, data->getOuputCount());
		for(std::map<std::string, TransactionInput*>::iterator it = data->getInputs().begin(); it != data->getInputs().end(); i++) {
			writeTransactionInput(stream, it->second);
		}
		for(std::map<std::map, TransactionOutput*>::iterator it = data->getOutputs().begin(); it != data->getOutputs().end(); it++) {
			writeTransactionOutput(stream, it->second);
		}
		writeDouble(stream, data->getValue());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->TimeLocked());
		writeLongLong(stream, data->timeConfirmed());
		writeTransactionData(stream, data->getData());
		return true;
	}

	template <class T>
	bool writeBlock(std::ofstream* stream, T* data) {
		writeBlockData(stream, data->getData());
		writeLong(stream, data->getHeight());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeLocked());
		writeUnsignedLong(stream, data->getSize());
		writeUnsignedLong(stream, data->getBits());
		writeChar(stream, data->getIsMainChain());
		writeLong(stream, data->getIndex());
		writeLong(stream, data->getValue());
		writeLong(stream, data->getNonce());
		writeString(stream, data->getPreviousHash());
		return true;
	}

	template <class T>
	bool writeBlockData(std::ofstream* stream, T* data) {
		writeString(stream, data->getHash());
		for(std::map<std::string, Transaction<TransactionData*>*>::iterator it = data->getTransactions().begin(); it != data->getTransactions().end(); i++){
			writeTransaction(stream, it->second);
		}
		writeUnsignedLong(stream, data->getSize());
		writeUnsignedLong(stream, data->getTransactionCount());
		writeUnsignedLong(stream, data->getBits());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeRecieved());
		writeLongLong(stream, data->getTimeLocked());
		std::vector<unsigned char> bytes = data->getRawData()->toBytes();
		for(int i = 0; i < bytes.size(); i++){
			writeUnsignedChar(stream, bytes[i]);
		}
		return true;
	}

	template <class T>
	bool writeBlockchain(std::ofstream* stream, T* data) {
		if(data->getLastBlock() != nullptr)
			writeBlock(stream, data->getLastBlock());
		for(int i = 0; i < data->getBlocks().size(); i++)
			writeBlock(stream, data->getBlocks()[i]);
		if(data->orphanedChains.size() > 0)
			writeBlockchain(stream, data->getOrphanedChains()[i]);
		writeUnsignedLong(stream, data->getCount())
		return true;
	}

	char* readChar(std::ifstream* stream);
	short* readShort(std::ifstream* stream);
	int* readInt(std::ifstream* stream);
	long* readLong(std::ifstream* stream);
	long long* readLongLong(std::ifstream* stream);
	unsigned char* readUnsignedChar(std::ifstream* stream);
	unsigned short* readUnsignedShort(std::ifstream* stream);
	unsigned int* readUnsignedInt(std::ifstream* stream);
	unsigned long* readUnsignedLong(std::ifstream* stream);
	unsigned long long* readUnsignedLongLong(std::ifstream* stream);
	float* readFloat(std::ifstream* stream);
	double* readDouble(std::ifstream* stream);
	std::string* readString(std::ifstream* stream);

	TransactionInput* readTransactionInput(std::ifstream* stream);
	TransactionOutput* readTransactionOutput(std::ifstream* stream);

	template <class T>
	T* readTransaction(std::ifstream* stream) {
		return nullptr;
	}

	template <class T>
	T* readBlock(std::ifstream* stream) {
		return nullptr;
	}

	template <class T>
	T* readBlockData(std::ifstream* stream) {
		return nullptr;
	}

	template <class T>
	T* readBlockchain(std::ifstream* stream) {
		return nullptr;
	}

}