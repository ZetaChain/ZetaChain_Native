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
#include <map> // std::map
#include <string> // std::string
#include <ctime> // time_t localtime() struct tm* asctime()
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <stdexcept> // throw throw std::runtime_error()
#include "thirdparty/json.hpp"
#include "operators.hpp"
#include "conversions.hpp" // toBytes()
#include "transactions/transaction.hpp"
#include "blockdata/unsignedlonglongblockdata.hpp"
#include "hashing.hpp"

namespace BlockchainCpp {
	UnsignedLongLongBlockData::UnsignedLongLongBlockData(unsigned long long data){
		this->rawData = data;
	}

	UnsignedLongLongBlockData::~UnsignedLongLongBlockData(){

	}

	std::string UnsignedLongLongBlockData::computeHash(){
		return Hashing::hashVector(this->toBytes());
	}

	std::vector<unsigned char> UnsignedLongLongBlockData::toBytes(){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(UnsignedLongLongBlockData));

 		std::vector<std::string> values = Conversions::mapToValuesString(this->transactions, this->transactions.size());
		
		for(int i = 0; i < values.size() - 1; i++)
			bytes += Conversions::toBytes(values[i]);

		bytes += Conversions::toBytes(&this->size);
		bytes += Conversions::toBytes(&this->transactionCount);
		bytes += Conversions::toBytes(&this->bits);
		bytes += Conversions::toBytes(&this->timeCreated);
		bytes += Conversions::toBytes(&this->timeRecieved);
		bytes += Conversions::toBytes(&this->timeLocked);
		bytes += Conversions::toBytes(&this->rawData);
		return bytes;
	}

	std::string UnsignedLongLongBlockData::toString(){
		nlohmann::json j;
		nlohmann::json transactions = nlohmann::json::array();
		std::vector<std::string> values = Conversions::mapToValuesString(this->transactions, this->transactions.size());

		for(int i = 0; i < this->transactions.size() - 1; i++) {
			if(values.size() == 0)
				break;
			nlohmann::json obj = values[i];
			j.push_back(obj);
		}
		
		j["type", "BlockData"];
		j["hash", this->hash];
		j["size", this->size];
		j["transactions"] = transactions.dump();
		j["transactionCount", this->transactionCount];
		j["bits", this->bits];
		j["timeCreated", this->timeCreated];
		j["timeRecieved", this->timeRecieved];
		j["timeLocked", this->timeLocked];
		j["rawData", this->rawData];
		return j;
	}

	bool UnsignedLongLongBlockData::verify(){
		return this->hash == computeHash();
	}

	bool UnsignedLongLongBlockData::lock() {
		//TODO
		return false;
	}


	std::string UnsignedLongLongBlockData::getHash(){
		return this->hash;
	}

	std::map<std::string, Transaction<TransactionData*>*> UnsignedLongLongBlockData::getTransactions(){
		return static_cast<std::map<std::string, Transaction<TransactionData*>*>>(this->transactions);
	}

	unsigned long UnsignedLongLongBlockData::getSize(){
		return this->size;
	}

	unsigned long UnsignedLongLongBlockData::getTransactionCount(){
		return this->transactionCount;
	}

	unsigned long UnsignedLongLongBlockData::getBits(){
		return this->bits;
	}

	time_t UnsignedLongLongBlockData::getTimeCreated(){
		return this->timeCreated;
	}

	time_t UnsignedLongLongBlockData::getTimeRecieved(){
		return this->timeRecieved;
	}

	time_t UnsignedLongLongBlockData::getTimeLocked(){
		return this->timeLocked;
	}

	unsigned long long UnsignedLongLongBlockData::getRawData(){
		return this->rawData;
	}

	void UnsignedLongLongBlockData::setHash() {
		if(this->hash != "")
			throw std::runtime_error("Hash has already been set");
		this->hash = computeHash();
	}

	void UnsignedLongLongBlockData::setHash(std::string hash) {
		if(this->hash != "")
			throw std::runtime_error("Hash has already been set");
		this->hash = hash;
	}

	void UnsignedLongLongBlockData::setTransactions(std::map<std::string, Transaction<TransactionData*>*> transactions){
		if(this->transactions.size() != 0)
			throw std::runtime_error("Transactions have already been set");
		this->transactions = transactions;
	}

	void UnsignedLongLongBlockData::setSize(unsigned long size){
		if(this->size != 0)
			throw std::runtime_error("Size has already been set");
		this->size = size;
	}

	void UnsignedLongLongBlockData::setTransactionCount(unsigned long count){
		if(this->transactionCount != 0)
			throw std::runtime_error("Transaction Count has already been set");
		this->transactionCount = count;
	}

	void UnsignedLongLongBlockData::setBits(unsigned long bits){
		if(this->bits != 0)
			throw std::runtime_error("Bits has already been set");
		this->bits = bits;
	}

	void UnsignedLongLongBlockData::setTimeCreated(time_t timeCreated){
		if(this->timeCreated != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeCreated);
			time_t _time = this->timeCreated;
			throw std::runtime_error("Block Data was already created at");
		}
		this->timeCreated = timeCreated;
	}

	void UnsignedLongLongBlockData::setTimeRecieved(time_t timeRecieved){
		if(this->timeRecieved != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeRecieved);
			time_t _time = this->timeRecieved;
			throw std::runtime_error("Block Data was already recieved");
		}
		this->timeRecieved = timeRecieved;
	}

	void UnsignedLongLongBlockData::setTimeLocked(time_t timeLocked){
		if(this->timeLocked != 0) {
			struct tm* timeinfo;
			timeinfo = localtime(&this->timeLocked);
			time_t _time = this->timeLocked;
			throw std::runtime_error("Block Data was already locked");
		}
		this->timeLocked = timeLocked;
	}

	void UnsignedLongLongBlockData::setRawData(unsigned long long rawData) {
		this->rawData = rawData;
	}

}
