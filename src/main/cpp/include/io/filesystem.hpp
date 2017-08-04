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


namespace BlockchainCpp::IO::Filesystem {
	#ifdef _WIN32
		HANDLE createFile(LPCSTR fileName, DWORD desiredAccess, DWORD shareMode, LPSECURITY_ATTRIBUTES attributes,
						DWORD creationDisposition, DWORD flags, HANDLE templateFile);
		BOOL createDirectory(LPCSTR directoryName, LPSECURITY_ATTRIBUTES attributes);
		BOOL closeFile(HANDLE file);
		BOOL openFile(LPCSTR fileName, LPOFSTRUCT reOpenBuf, UINT style);
		BOOL deleteFile(LPCSTR filePath);

	#elif __linux__ || __unix__ || __APPLE__
		int createDirectory(std::string path, mode_t mode);
		int openFile(std::string pathname, int flags);
		int openFile(std::string pathname, int flags, mode_t mode);
		int createFile(std::string pathname, mode_t mode);
		void deleteFile(std::string file);
	#endif
}