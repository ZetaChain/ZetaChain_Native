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
#include <vector>

namespace ZetaChain_Native {
	namespace Hashing {
		std::string hashString(std::string str);
		std::string hashChar(char ch);
		std::string hashUnsignedChar(unsigned char uch);
		std::string hashShort(short s);
		std::string hashUnsignedShort(unsigned short us);
		std::string hashInt(int i);
		std::string hashUnsignedInt(unsigned int ui);
		std::string hashLong(long l);
		std::string hashUnsignedLong(unsigned long ul);
		std::string hashLongLong(long long ll);
		std::string hashUnsignedLongLong(unsigned long long ull);
		std::string hashBoolean(bool b);

		template <class T>

		std::string hashVector(std::vector<T> vec) {
			std::string str = "";
			if (vec.size() == 0)
				return "";
			for (int i = 0; i < vec.size() - 1; i++) {
				str += vec[i];
			}
			return hashString(str);
		}
	}
}