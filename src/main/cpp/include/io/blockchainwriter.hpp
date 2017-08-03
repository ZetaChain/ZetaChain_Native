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
#include <fstream>
#include <vector>
#include "constants.hpp"
#include "blockchains/blockchain.hpp"
#include "blocks/block.hpp"

namespace BlockchainCpp::IO {
	
	template <class BlockType>
	
	class BlockchainWriter{
		public:
			BlockchainWriter(std::string filePath, Blockchain<BlockType>* blockchain, bool binary = true) {
				this->filePath = filePath;
				this->blockchain = blockchain;
				this->binary = binary;
				file.open(this->filePath);
			}
			
			~BlockchainWriter() {
				file.close();
				delete this->blockchain;
			}

			bool write() {
				if(this->binary) {
					file << BLOCKCHAIN_HEADER;
					std::vector<unsigned char> bytes = this->blockchain->toBytes();
					for(int i = 0; i < bytes.size() - 1; i++) {
						file << bytes[i];
					}
					file << 0;
				}
				else {
					file << this->blockchain->toString();
					file << "\n";
				}
				return true;
			}

			void close() {
				this->file.close();
			}
		protected:

		private:
			std::string filePath;
			bool binary;
			Blockchain<BlockType>* blockchain;
			std::ofstream file;
	};
}