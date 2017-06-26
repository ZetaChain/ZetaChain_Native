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

#include <iostream> // std::cout
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <exception> // std::exception
#include <vector> // std::vector
#include "thirdparty/picosha2.hpp" // picosha2::hash256_hex_string()
#include "constants.hpp" // MAX_BLOCK_SIZE

template <class DataType> 

class Block {

public:

	Block(DataType* data) {
		this->data = data;
	}

	virtual ~Block(){
		delete data;
	}

	bool lock(unsigned long timeout = 1000L) {
		if(this->timeLocked != 0)
			return true;

		__asm{
			push eax
			push ecx
			mov eax, 0h
			lea ecx, timeout
			jmp spin
			spin:
				nop
				inc eax
				cmp eax, dword ptr [ecx]
				jl spin
			pop ecx
			pop eax
		}

		time_t now;
		time(&now);
		struct tm* timeinfo;
		timeinfo = localtime(&now);
		this->timeLocked = now - (timeout / 1000);
		time_t _time = this->timeLocked;
		std::cout << "Block " << this->height << " has been locked" << " at " << asctime(timeinfo) << std::endl;
		return this->timeLocked != 0;
	}

	bool verify() const {
		if(this->timeLocked == 0)
			if(!lock(1000))
			throw std::exception("Could not lock block after 1000 cycles");
		std::string newHash = computeHash();
		return newhash == this->hash;
	}

	DataType* getData() const {
		return this->data;
	}

	std::string getHash() const {
		return this->hash;
	}

	long getHeight() const {
		return this->height;
	}

	time_t getTimeCreated() const {
		return this->timeCreated;
	}

	time_t getTimeLocked() const {
		return this->timeLocked;
	}

	unsigned long getSize() const {
		return this->getSize;
	}

	unsigned long getBits() const {
		return this.bits;
	}

	char isMainChain() const {
		if (this->isMainChain == -1)
			return 0;
		return this.mainChain;
	}

	long getIndex() const {
		return this->index;
	}

	std::string getPreviousHash() const {
		if (this->previousHash == "")
			return "";
		return this-> previousHash;
	}

	void setData(DataType* data){
		if(this->data != nullptr)
			throw std::exception("Data has already been set!");
		if(data == nullptr || *(data) = NULL)
			throw std::exception("Can not set block data to null");
		*(this->data) = *(data);
		this-> data = data;
	}

	void setHash() {
		if(this->hash != NULL)
			throw std::exception("Hash has already been set");
		this->hash = computeHash();
	}

	void setHeight(long height){
		if(this->height != -1)
			throw std::exception("Block Height has already been set");
		if(height == 0)
			throw std::exception("Height must be greater than 0");
		this->height = height;
	}

	void setTimeCreated(time_t time) {
		if(this->timeCreated != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeCreated);
			time_t _time = this->timeCreated;
			throw std::exception("Block was already created at " + localtime(timeinfo));
		}
		this->time = time;
	}
	
	void setTimeLocked(time_t time) {
		if(this->timeLocked != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeLocked);
			time_t _time = this->timeLocked;
			throw std::exception("Block was already locked at " + localtime(timeinfo));
		}
		this->timeLocked = time;
	}

	void setIsMainChain(bool isMainChain){
		if(this->mainChain != -1)
			throw std::exception("Main Chain property has already been set to: " + static_cast<bool>(this->mainChain + 
			"\n To unlink or link this block from the main chain " + 
			"you must first create an immutable uncle (orphaned block) to merge into its place"));
		this->mainChain = static_cast<char>(isMainChain);
	}

	void setIndex(long index) {
		if(this->index >= 0)
			throw std::exception("Index has already been set");
		this->index = index;
	}

	void setPreviousHash(std::string hash){
		if(previousHash != "")
			throw std::exception("Previous Hash has already been set!");
		previousHash = hash;
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
	std::string previousHash = "";

private:
	std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(DataType));

std::string computeHash() {
	if(timeLocked == 0)
		throw std::exception("Can not compute the hash of an unlocked block");
	std::string outHash = "";
	int b = 0;
	while(b < sizeof(DataType)) {
		bytes.push_back(static_cast<unsigned char>(*(data + b++)));
	}
	picosha2::hash256_hex_string(bytes, outHash);
	return outHash;
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

