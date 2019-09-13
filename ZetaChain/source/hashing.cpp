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
#include "hashing.hpp"
#include "sha256.hpp"

extern bool __noSHA256;

extern "C" unsigned char* HashDataASM(unsigned char* data, size_t size);

namespace ZetaChain_Native {
	namespace Hashing {
		std::string hashString(std::string str) {
			if (__noSHA256) {
				char* cstr = const_cast<char*>(str.c_str());
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(cstr), str.size());
				return std::string(ptr, ptr + 32);
			}
			return sha256(str);
		}
		std::string hashChar(char ch) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&ch), sizeof(char));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, ch));
		}
		std::string hashUnsignedChar(unsigned char uch) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(&uch, sizeof(unsigned char));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, uch));
		}
		std::string hashShort(short s) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&s), sizeof(short));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, s));
		}
		std::string hashUnsignedShort(unsigned short us) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&us), sizeof(unsigned short));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, us));
		}
		std::string hashInt(int i) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&i), sizeof(int));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, i));
		}
		std::string hashUnsignedInt(unsigned int ui) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&ui), sizeof(unsigned int));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, ui));
		}
		std::string hashLong(long l) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&l), sizeof(long));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, l));
		}
		std::string hashUnsignedLong(unsigned long ul) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&ul), sizeof(unsigned long));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, ul));
		}
		std::string hashLongLong(long long ll) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&ll), sizeof(long long));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, ll));
		}
		std::string hashUnsignedLongLong(unsigned long long ull) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&ull), sizeof(unsigned long long));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, ull));
		}
		std::string hashBoolean(bool b) {
			if (__noSHA256) {
				unsigned char* ptr = HashDataASM(reinterpret_cast<unsigned char*>(&b), sizeof(bool));
				return std::string(ptr, ptr + 32);
			}
			return sha256(std::string(1, b));
		}
	}
}