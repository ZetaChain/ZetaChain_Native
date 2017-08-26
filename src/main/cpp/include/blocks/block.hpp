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
#include <iostream> // std::cout
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <stdexcept> // throw throw std::runtime_error()
#include <vector> // std::vector
#include "operators.hpp"
#include "conversions.hpp" // toBytes()
#include "constants.hpp" // MAX_BLOCK_SIZE
#include "hashing.hpp"

extern "C" void lockBlockASM(unsigned long timeout);
extern "C" long mineASM(void* dataAddr, long dataSize);

namespace BlockchainCpp {

	template <class DataType> 

	class Block {

	public:

		Block(DataType* data) {
			this->data = data;
		}

		virtual ~Block(){
			// delete data;
		}

		long mine() {
			std::cout << "Mining Block " << this->height << std::endl;
			auto start = std::chrono::high_resolution_clock::now();
			this->setValue(mineASM(reinterpret_cast<void*>(this->data), sizeof(this->data)));
			auto end = std::chrono::high_resolution_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			std::cout << "Block " << this->height << " Was Sucessfully Mined in: " << diff << " ns" << std::endl;
			return this->value;
		}

		bool lock(unsigned long timeout = 1000L) {
			if(this->timeLocked != 0)
				return true;
			
			lockBlockASM(timeout);

			time_t now;
			time(&now);
			struct tm* timeinfo;
			timeinfo = localtime(&now);
			this->timeLocked = now - (timeout / 1000);
			time_t _time = this->timeLocked;
			this->hash = computeHash();
			std::cout << "Block " << this->height << " has sucessfully been locked" << std::endl;
			return this->timeLocked != 0;
		}

		bool verify() {
			if(this->timeLocked == 0)
				if(!lock(1000))
					throw std::runtime_error("Could not lock block after 1000 cycles");
			std::string newHash = computeHash();
			return newHash == this->hash;
		}

		std::vector<unsigned char> toBytes() {
			std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(Block<DataType>));
			// bytes += data->toBytes();
			bytes += Conversions::toBytes(this->hash);
			bytes += Conversions::toBytes(&this->height);
			bytes += Conversions::toBytes(&this->timeCreated);
			bytes += Conversions::toBytes(&this->timeLocked);
			bytes += Conversions::toBytes(const_cast<unsigned long*>(&this->size));
			bytes += Conversions::toBytes(const_cast<unsigned long*>(&this->bits));
			bytes += Conversions::toBytes(&this->mainChain);
			bytes += Conversions::toBytes(&this->index);
			bytes += Conversions::toBytes(&this->value);
			bytes += Conversions::toBytes(&this->nonce);
			bytes += Conversions::toBytes(this->previousHash);
			return bytes;
		}

		DataType* getData() {
			return this->data;
		}

		std::string getHash() {
			return this->hash;
		}

		long getHeight() {
			return this->height;
		}

		time_t getTimeCreated() {
			return this->timeCreated;
		}

		time_t getTimeLocked() {
			return this->timeLocked;
		}

		unsigned long getSize() {
			return this->size;
		}

		unsigned long getBits() {
			return this->bits;
		}

		char isMainChain() {
			if (this->mainChain == -1)
				return 0;
			return this->mainChain;
		}

		long getIndex() {
			return this->index;
		}

		long getValue() {
			return this->value;
		}

		long getNonce() {
			return this->nonce;
		}

		std::string getPreviousHash() {
			if (this->previousHash == "")
				return "";
			return this-> previousHash;
		}

		void setData(DataType* data){
			if(this->data != nullptr)
				throw std::runtime_error("Data has already been set!");
			if(data == nullptr || *(data) = NULL)
				throw std::runtime_error("Can not set block data to null");
			*(this->data) = *(data);
			this-> data = data;
		}

		void setHash() {
			if(this->hash != "")
				throw std::runtime_error("Hash has already been set");
			this->hash = computeHash();
		}

		void setHeight(long height){
			if(this->height != -1)
				throw std::runtime_error("Block Height has already been set");
			if(height == 0)
				throw std::runtime_error("Height must be greater than 0");
			this->height = height;
		}

		void setTimeCreated(time_t time) {
			if(this->timeCreated != 0) {
				struct tm* timeinfo;
				timeinfo = localtime(&this->timeCreated);
				time_t _time = this->timeCreated;
				throw std::runtime_error("Block was already created");
			}
			this->time = time;
		}
		
		void setTimeLocked(time_t time) {
			if(this->timeLocked != 0) {
				struct tm* timeinfo;
				timeinfo = localtime(&this->timeLocked);
				time_t _time = this->timeLocked;
				throw std::runtime_error("Block was already locked");
			}
			this->timeLocked = time;
		}

		void setIsMainChain(bool isMainChain){
			if(this->mainChain != -1)
				throw std::runtime_error("Main Chain property has already been set to: " + static_cast<bool>(this->mainChain) + 
				"\n To unlink or link this block from the main chain " + 
				"you must first create an immutable uncle (orphaned block) to merge into its place");
			this->mainChain = static_cast<char>(isMainChain);
		}

		void setIndex(long index) {
			if(this->index >= 0)
				throw std::runtime_error("Index has already been set");
			this->index = index;
		}

		void setValue(long value) {
			if(this->value != -1)
				throw std::runtime_error("Value has already been set");
			this->value = value;
		}

		void setNonce(long nonce) {
			if(this->nonce != -1)
				throw std::runtime_error("Nonce has already been set");
			this->nonce = nonce;
		}

		void setPreviousHash(std::string _hash){
			if(previousHash != "")
				throw std::runtime_error("Previous Hash has already been set!");
			previousHash = _hash;
		}

	protected:
		DataType* data = nullptr;
		std::string hash = "";
		long height = -1;
		time_t timeCreated = 0;
		time_t timeLocked = 0;
		const unsigned long size = sizeof(DataType);
		const unsigned long bits = size * 8;
		char mainChain = -1;
		long index = -1;
		long value = -1;
		long nonce = -1;
		std::string previousHash = "";

	private:

	std::string computeHash() {
		return Hashing::hashVector(this->toBytes());
	}
	};

	template <class DataType> 
	bool operator==(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height == rhs.height;
	}

	template <class DataType> 
	bool operator!=(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height != rhs.height;
	}

	template <class DataType> 
	bool operator<(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height < rhs.height;
	}

	template <class DataType> 
	bool operator<=(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height <= rhs.height;
	}

	template <class DataType> 
	bool operator>(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height > rhs.height;
	}

	template <class DataType> 
	bool operator>=(const Block<DataType>& lhs, const Block<DataType>& rhs) {
		return lhs.height >= rhs.height;
	} 
}

