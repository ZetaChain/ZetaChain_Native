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
#include <vector>
#include "constants.hpp"
#include "transactions/transactionoutput.hpp"
#include "io/serialisation.hpp"
#include "io/transactionoutputwriter.hpp"

namespace ZetaChain_Native::IO {
	TransactionOutputWriter::TransactionOutputWriter(std::string filePath, TransactionOutput* output, bool binary){
		this->filePath = filePath;
		this->output = output;
		this->binary = binary;
		if(this->binary)
			file.open(this->filePath, std::fstream::out | std::fstream::binary);
		else
			file.open(this->filePath, std::fstream::out);
	}

	TransactionOutputWriter::~TransactionOutputWriter() {
		this->file.close();
		delete this->output;
	}

	bool TransactionOutputWriter::write() {
		if(this->binary){
			file << TRANSACTION_OUTPUT_HEADER;
			Serialisation::writeTransactionOutput(&file, output);
			file << 0;
		}
		else {
			std::string str = this->output->toString();
			file << str;
			file << "\n";
		}
		return true;
	}

	void TransactionOutputWriter::close() {
		this->file.close();
	}
}