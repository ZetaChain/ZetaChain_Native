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
#include <vector>
#include <map>
#include <stdexcept>
#include "conversions.hpp"
#include "operators.hpp"
#include "hashing.hpp"
#include "customdata.hpp"

using namespace ZetaChain_Native;

CustomData::CustomData(int anInt, std::string aString, std::vector<unsigned char> aVector, std::map<std::string, int> aMap){
	this->anInt = anInt;
	this->aString = aString;
	this->aVector = aVector;
	this->aMap = aMap;
	this->hash = this->computeHash();
}

CustomData::~CustomData() {

}

bool CustomData::verify() {
	return this->hash == computeHash();
}

int CustomData::getAnInt() {
	return this->anInt;
}

std::string CustomData::getAString() {
	return this->aString;
}

std::vector<unsigned char> CustomData::getAVector() {
	return this->aVector;
}

std::map<std::string, int> CustomData::getAMap() {
	return this->aMap;
}

std::string CustomData::getHash() {
	return this->hash;
}

std::string CustomData::computeHash() {
	if(this->hash != "")
		throw std::runtime_error("Hash has already been set");
	std::vector<unsigned char> bytes = this->toBytes();
	return Hashing::hashVector(bytes);
}

std::vector<unsigned char> CustomData::toBytes() {
	std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(CustomData));
	bytes += Conversions::toBytes(&this->anInt);
	bytes += Conversions::toBytes(this->aString);
	bytes += this->aVector;
	for(std::map<std::string, int>::iterator it = this->aMap.begin(); it != this->aMap.end(); it++){
		bytes += Conversions::toBytes(&it->second);
	}
	return bytes;
}
