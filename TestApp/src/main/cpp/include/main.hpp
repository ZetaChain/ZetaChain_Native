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
#include <iostream> // std::cout, std::cin
#include <thread> // std::this_thread
#include <random> // rand(), srand()
#include <chrono> // high_resolution_clock, std::chrono::time_point_cast, std::chrono::nanoseconds
#include <vector> // std::vector
#include <map> // std::map
#include <stdexcept> // std::runtime_error
#include "blocks/block.hpp" // Block Stuff
#include "blockchains/blockchain.hpp" // Blockchain Stuff
#include "blockdata/intblockdata.hpp" // IntBlockData
#include "blockdata/stringblockdata.hpp" // StringBlockData
#include "blockdata/floatblockdata.hpp" // FloatBlockData
#include "blockdata/doubleblockdata.hpp" // DoubleBlockData
#include "blockdata/charblockdata.hpp" // CharBlockData
#include "blockdata/shortblockdata.hpp" // ShortBlockData
#include "blockdata/longblockdata.hpp" // LongBlockData
#include "blockdata/longlongblockdata.hpp" // LongLongBlockData
#include "blockdata/boolblockdata.hpp" // BoolBlockData
#include "blockdata/unsignedcharblockdata.hpp" // UnsignedCharBlockData
#include "blockdata/unsignedshortblockdata.hpp" // UnsignedShortBlockData
#include "blockdata/unsignedintblockdata.hpp" // UnsignedIntBlockData
#include "blockdata/unsignedlongblockdata.hpp" // UnsignedLongBlockData
#include "blockdata/unsignedlonglongblockdata.hpp" // UnsignedLongLongBlockData
#include "customblockdata.hpp" // CustomBlockData
#include "customdata.hpp" // CustomData
#include "io/blockchainwriter.hpp" // BlockchainWriter
#include "io/blockchainreader.hpp"
#include "io/filesystem.hpp" // createDirectory, createFile
#include "io/serialisation.hpp"
#include "constants.hpp" // chars, BLOCK_HEADER

using namespace BlockchainCpp;

extern bool __nosha256;
extern bool __useJSONFormat;

void createIntBlockchain();
void createStringBlockchain();
void createFloatBlockchain();
void createDoubleBlockchain();
void createCharBlockchain();
void createShortBlockchain();
void createLongBlockchain();
void createLongLongBlockchain();
void createBoolBlockchain();
void createUnsignedCharBlockchain();
void createUnsignedShortBlockchain();
void createUnsignedIntBlockchain();
void createUnsignedLongBlockchain();
void createUnsignedLongLongBlockchain();
void createCustomDataBlockchain();

template<class T>
T readBlockchain(std::string filePath, bool binary) {
	T chain = nullptr;
	IO::BlockchainReader<decltype(chain)>* reader = new IO::BlockchainReader<decltype(chain)>(filePath, binary);
	chain = reader->read();
	reader->close();
	return chain;
}

template<class T>
void loadBlockchain(std::string filePath, bool binary) {
	T* blockchain = nullptr;
	blockchain = readBlockchain<decltype(blockchain)>(filePath, binary);

	// std::cout << "Verifying Loaded Blockchain" << std::endl;

	// if(!blockchain->verify())
	// 	throw std::runtime_error("Error Verifying Blockchain");

	// std::cout << "Blockchain Sucessfully Verified" << std::endl;

	std::cout << "Printing Loaded Blockchain" << std::endl;

	for(int i = 0; i < blockchain->getBlocks().size() - 1; i++) {
		std::cout << "Hash: " << blockchain->getBlockByHeight(i).getHash() << " Height: " << blockchain->getBlockByHeight(i).getHeight() << " Data: " << blockchain->getBlockByHeight(i).getData()->getRawData() << std::endl;
	}
}