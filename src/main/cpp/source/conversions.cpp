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
#include <vector>
#include <map>
#include <memory>
#include "conversions.hpp"

namespace Conversions {
	std::vector<unsigned char> toBytes(double* d){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(double));
		for(int itr = 0; itr < sizeof(double); itr++){
			bytes.push_back(static_cast<unsigned char>(*(d + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(float* f){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(float));
		for(int itr = 0; itr < sizeof(float); itr++){
			bytes.push_back(static_cast<unsigned char>(*(f + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(long long* ll){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(long long));
		for(int itr = 0; itr < sizeof(long long); itr++){
			bytes.push_back(static_cast<unsigned char>(*(ll + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(long* l){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(long));
		for(int itr = 0; itr < sizeof(long); itr++){
			bytes.push_back(static_cast<unsigned char>(*(l + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(int* i){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(int));
		for(int itr = 0; itr < sizeof(int); itr++){
			bytes.push_back(static_cast<unsigned char>(*(i + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(short* s){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(short));
		for(int itr = 0; itr < sizeof(short); itr++){
			bytes.push_back(static_cast<unsigned char>(*(s + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(unsigned long long* ull){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(unsigned long long));
		for(int itr = 0; itr < sizeof(unsigned long long); itr++){
			bytes.push_back(static_cast<unsigned char>(*(ull + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(unsigned long* ul){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(unsigned long));
		for(int itr = 0 ;itr < sizeof(unsigned long); itr++){
			bytes.push_back(static_cast<unsigned char>(*(ul + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(unsigned int* ui){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(unsigned int));
		for(int itr = 0; itr < sizeof(unsigned int); itr++){
			bytes.push_back(static_cast<unsigned char>(*(ui + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(unsigned short* us){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(unsigned short));
		for(int itr = 0; itr < sizeof(unsigned short); itr++){
			bytes.push_back(static_cast<unsigned char>(*(us + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(bool* b){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(sizeof(bool));
		for(int itr = 0; itr < sizeof(bool); itr++){
			bytes.push_back(static_cast<unsigned char>(*(b + itr)));
		}
		return bytes;
	}

	std::vector<unsigned char> toBytes(std::string str){
		std::vector<unsigned char> bytes = std::vector<unsigned char>(str.size());
		for(int itr = 0; itr < str.size(); itr++){
			bytes.push_back(static_cast<unsigned char>(str[itr]));
		}
		return bytes;
	}
}