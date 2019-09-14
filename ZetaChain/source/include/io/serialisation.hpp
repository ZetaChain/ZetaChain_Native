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
#include "conversions.hpp"

namespace ZetaChain_Native::IO::Serialisation {
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
	TransactionData* readTransactionData(std::ifstream* stream);

	template <class T>
	std::vector<unsigned char> serialise(std::ofstream* stream, T data) {
		T* ptr = &data;
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(data));
		for (int i = 0; i < sizeof(data) - 1; i++) {
			bytes.push_back(*(reinterpret_cast<unsigned char*>(ptr + i)));
		}
		return bytes;
	}

	template <class T>
	T* deserialise(std::ifstream* stream, T** data, size_t size) {
		for (int i = 0; i < size - 1; i++) {
			*(data + i) = readUnsignedChar(stream);
		}
		return reinterpret_cast<T*>(*data);
	}

	template <class T>
	bool writeTransaction(std::ofstream* stream, T data) {
		writeString(stream, data->getHash());
		writeInt(stream, data->getInputCount());
		writeInt(stream, data->getOutputCount());
		for (std::map<std::string, TransactionInput*>::iterator it = data->getInputs().begin(); it != data->getInputs().end(); it++) {
			writeTransactionInput(stream, it->second);
		}
		for (std::map<std::string, TransactionOutput*>::iterator it = data->getOutputs().begin(); it != data->getOutputs().end(); it++) {
			writeTransactionOutput(stream, it->second);
		}
		writeDouble(stream, data->getValue());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeLocked());
		writeLongLong(stream, data->getTimeConfirmed());
		writeTransactionData(stream, data->getData());
		return true;
	}

	template <class T>
	bool writeBlockData(std::ofstream* stream, T data) {
		writeString(stream, data->getHash());
		writeUnsignedLong(stream, data->getTransactionCount());
		if (data->getTransactions().size() > 0) {
			for (std::map<std::string, Transaction<TransactionData*>*>::iterator it = data->getTransactions().begin(); it != data->getTransactions().end(); it++) {
				writeTransaction(stream, it->second);
			}
		}
		writeUnsignedLong(stream, data->getSize());
		writeUnsignedLong(stream, data->getBits());
		writeLongLong(stream, data->getTimeCreated());
		writeLongLong(stream, data->getTimeRecieved());
		writeLongLong(stream, data->getTimeLocked());
		auto d = *data;
		std::vector<unsigned char> bytes = serialise(stream, d.getRawData());
		for (int i = 0; i < sizeof(d.getRawData()) - 1; i++) {
			writeUnsignedChar(stream, bytes[i]);
		}
		return true;
	}

	template <class T>
	bool writeBlock(std::ofstream* stream, T data) {
		writeBlockData(stream, data.getData());
		writeLong(stream, data.getHeight());
		writeLongLong(stream, data.getTimeCreated());
		writeLongLong(stream, data.getTimeLocked());
		writeUnsignedLong(stream, data.getSize());
		writeUnsignedLong(stream, data.getBits());
		writeChar(stream, data.isMainChain());
		writeLong(stream, data.getIndex());
		writeLong(stream, data.getValue());
		writeLong(stream, data.getNonce());
		writeString(stream, data.getPreviousHash());
		return true;
	}

	template <class B>
	bool writeOrphanedChain(std::ofstream* stream, Blockchain<Block<B>>* data) {
		Block<B>* blkPtr = data->getLastBlock();
		Block<B> blk = *blkPtr;
		writeBlock<Block<B>>(stream, blk);
		writeUnsignedLong(stream, data->getCount());
		std::vector<Block<B>> blocks = Conversions::mapToValues(data->getBlocks());
		for (int i = 0; i < blocks.size() - 1; i++) {
			writeBlock(stream, blocks[i]);
		}
		return true;
	}

	template <class B>
	bool writeBlockchain(std::ofstream* stream, Blockchain<Block<B>>* data) {
		Block<B>* blkPtr = data->getLastBlock();
		Block<B> blk = *blkPtr;
		writeBlock<Block<B>>(stream, blk);
		writeUnsignedLong(stream, data->getCount());
		writeUnsignedLong(stream, data->getOrphanCount());
		std::vector<Block<B>> blocks = Conversions::mapToValues(data->getBlocks());
		for (int i = 0; i < blocks.size() - 1; i++) {
			writeBlock(stream, blocks[i]);
		}
		if (data->getOrphanedChains().size() > 0)
			for (int i = 0; i < data->getOrphanedChains().size() - 1; i++)
				writeOrphanedChain(stream, data->getOrphanedChains()[i]);
		return true;
	}


	template <class D>
	Transaction<D> readTransaction(std::ifstream* stream) {
		Transaction<D> result = Transaction<D>();
		std::string hash = *readString(stream);
		int inputCount = *readInt(stream);
		int outputCount = *readInt(stream);
		std::map<std::string, TransactionInput*> inputs = std::map<std::string, TransactionInput*>();
		for (int i = 0; i < inputCount - 1; i++) {
			TransactionInput* input = readTransactionInput(stream);
			inputs.insert(std::make_pair(input->getHash(), input));
		}
		std::map<std::string, TransactionOutput*> outputs = std::map<std::string, TransactionOutput*>();
		for (int i = 0; i < outputCount - 1; i++) {
			TransactionOutput* output = readTransactionOutput(stream);
			outputs.insert(std::make_pair(output->getHash(), output));
		}
		double value = *readDouble(stream);
		time_t timeCreated = *readUnsignedLong(stream);
		time_t timeLocked = *readUnsignedLong(stream);
		time_t timeConfirmed = *readUnsignedLong(stream);
		D* out = malloc(sizeof(D));
		D data = *deserialise<D>(stream, &out, 4);
		result.setHash(hash);
		result.setInputs(inputs);
		result.setOutputs(outputs);
		result.setValue(value);
		result.setTimeCreated(timeCreated);
		result.setTimeLocked(timeLocked);
		result.setTimeConfirmed(timeConfirmed);
		result.setData(data);
		free(out);
		return result;
	}

	template <class T, class R>
	T readBlockData(std::ifstream* stream) {
		T result = { 0 };
		std::string hash = *readString(stream);
		unsigned long transactionCount = *readUnsignedLong(stream);
		std::map<std::string, Transaction<TransactionData>> transactions = std::map<std::string, Transaction<TransactionData>>();
		for (int i = 0; i < transactionCount - 1; i++) {
			Transaction<TransactionData> tx = readTransaction<TransactionData>(stream);
			if (!tx.verify())
				return result;
			transactions.insert(std::make_pair(tx.getHash(), tx));
		}
		unsigned long size = *readUnsignedLong(stream);
		unsigned long bits = *readUnsignedLong(stream);
		time_t timeCreated = *readUnsignedLong(stream);
		time_t timeRecieved = *readUnsignedLong(stream);
		time_t timeLocked = *readUnsignedLong(stream);
		R* out = reinterpret_cast<R*>(malloc(sizeof(R)));
		R data = *deserialise<R>(stream, &out, sizeof(R));
		result.setHash(hash);
		result.setTransactionCount(transactionCount);
		result.setTransactions(transactions);
		result.setSize(size);
		result.setBits(bits);
		result.setTimeCreated(timeCreated);
		result.setTimeRecieved(timeRecieved);
		result.setTimeLocked(timeLocked);
		result.setRawData(data);
		free(out);
		if (!result.verify())
			return result;
		return result;
	}

	template <class B>
	Block<B> readBlock(std::ifstream* stream) {
		Block<B> result = Block<B>(nullptr);
		B data = readBlockData<B, decltype(result.getData())>(stream);
		std::string hash = *readString(stream);
		long height = *readLong(stream);
		time_t timeCreated = *readUnsignedLong(stream);
		time_t timeLocked = *readUnsignedLong(stream);
		unsigned long size = *readUnsignedLong(stream);
		unsigned long bits = *readUnsignedLong(stream);
		char mainChain = *readChar(stream);
		long index = *readLong(stream);
		long value = *readLong(stream);
		long nonce = *readLong(stream);
		std::string previousHash = *readString(stream);
		result.setData(&data);
		result.setHash(hash);
		result.setHeight(height);
		result.setTimeCreated(timeCreated);
		result.setTimeLocked(timeLocked);
		result.setSize(size);
		result.setBits(bits);
		result.setIsMainChain(mainChain);
		result.setValue(value);
		result.setNonce(nonce);
		result.setPreviousHash(previousHash);
		if (!result.verify())
			return nullptr;
		return result;
	}

	template <class B>
	Blockchain<Block<B>> readOrphanedChain(std::ifstream* stream) {
		Blockchain<Block<B>> result = Blockchain<Block<B>>();

		Block<B> lastBlock = readBlock<B>(stream);
		if (!lastBlock.verify())
			throw std::runtime_error("Last Block could not be verified");
		unsigned long count = *readUnsignedLong(stream);
		unsigned long orphanCount = 0UL;
		std::map<std::string, Block<B>> blocks = std::map<std::string, Block<B>>();
		for (int i = 0; i < count - 1; i++) {
			Block<B> block = readBlock<B>(stream);
			if (!block.verify())
				throw std::runtime_error("Block could not be verified");
			blocks.insert(std::make_pair(block.getHash(), block));
		}
		result.setLastBlock(&lastBlock);
		result.setCount(count);
		result.setOrphanCount(orphanCount);
		result.setBlocks(blocks);
		return result;
	}

	template <class B>
	Blockchain<Block<B>> readBlockchain(std::ifstream* stream) {
		Blockchain<Block<B>> result = Blockchain<Block<B>>();
		Block<B>* lastBlock = &readBlock<B>(stream);
		if (!lastBlock->verify())
			return result;
		unsigned long count = *readUnsignedLong(stream);
		unsigned long orphanCount = *readUnsignedLong(stream);
		std::map<std::string, Block<B>> blocks = std::map<std::string, Block<B>>();
		for (int i = 0; i < count - 1; i++) {
			Block<B> block = readBlock<B>(stream);
			if (!block.verify())
				return result;
			blocks.insert(std::make_pair(block.getHash(), block));
		}
		std::vector<Blockchain<Block<B>>*> orphanedChains = std::vector<Blockchain<Block<B>>*>();
		for (int i = 0; i < orphanCount - 1; i++) {
			Blockchain<Block<B>>* orphan = &readOrphanedChain<B>(stream);
			orphanedChains.push_back(orphan);
		}
		result.setLastBlock(lastBlock);
		result.setCount(count);
		result.setOrphanCount(orphanCount);
		result.setBlocks(blocks);
		result.setOrphanedChains(orphanedChains);
		return result;
	}
}