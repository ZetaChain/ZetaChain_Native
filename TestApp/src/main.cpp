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
#include <iostream> // std::cout, std::cin
#include <thread> // std::this_thread
#include <random> // rand(), srand()
#include <chrono> // high_resolution_clock, std::chrono::time_point_cast, std::chrono::nanoseconds
#include <vector> // std::vector
#include <map> // std::map
#include <stdexcept> // std::runtime_error
#include "main.hpp"
#include "opencl/init.hpp"
#include "opencl/openclhandle.hpp"
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
#include "io/filesystem.hpp" // createDirectory, createFile
#include "constants.hpp" // chars, BLOCK_HEADER

int main(int argc, char** argv) {

	//Uncomment when debugging in Visual Studio
	// char ch;
	// std::cin >> ch;

	for(int i = 0; i < argc; i++){
		std::cout << argv[i] << std::endl;
		if(std::string(argv[i]) == "--noSHA256")
			__noSHA256 = true;
		else if(std::string(argv[i]) == "--useJSONFormat")
			__useJSONFormat = true;
		else if(std::string(argv[i]) == "--noOpenCL")
			__noOpenCL = true;
	}

	std::cout << "Using SHA256: " << static_cast<int>(!__noSHA256) << std::endl;
	std::cout << "Using JSON File Format: " << static_cast<int>(__useJSONFormat) << std::endl;
	std::cout << "Using OpenCL " << static_cast<int>(!__noOpenCL) << std::endl;

	if(!__noOpenCL) {
		OpenCL::OpenCLHandle* handle = OpenCL::init();
		std::cout << std::endl;
		delete handle;
	}

	createIntBlockchain();
	createStringBlockchain();
	createFloatBlockchain();
	createDoubleBlockchain();
	createCharBlockchain();
	createShortBlockchain();
	createLongBlockchain();
	createLongLongBlockchain();
	createBoolBlockchain();
	createUnsignedCharBlockchain();
	createUnsignedShortBlockchain();
	createUnsignedIntBlockchain();
	createUnsignedLongBlockchain();
	createUnsignedLongLongBlockchain();
	createCustomDataBlockchain();
	
	// loadBlockchain<Blockchain<Block<IntBlockData>>>("data/intblockchain", !__useJSONFormat);

	return 0;
}

void createIntBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating int Blockchain" << std::endl;

	Blockchain<Block<IntBlockData>> blockchain = Blockchain<Block<IntBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		int num = 1 + rand() % 2147483646; // int.max - 1
		Block<IntBlockData>* b = new Block<IntBlockData>(new IntBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<IntBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::string path;
	if(__useJSONFormat)
		path = "data/intblockchain.json";
	else
		path = "data/intblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<IntBlockData>>* writer = new IO::BlockchainWriter<Block<IntBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
	
}

void createStringBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	
	std::cout << "Creating string Blockchain" << std::endl;

	Blockchain<Block<StringBlockData>> blockchain_s = Blockchain<Block<StringBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		std::string str = "";
		for(int count = 0; count < 50; count++){
			t = std::chrono::high_resolution_clock::now();
			generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
			srand(generator);
			char c = chars[rand() % (sizeof(chars) - 1)];
			str += c;
		}
		
		Block<StringBlockData>* b = new Block<StringBlockData>(new StringBlockData(str));
		if(blockchain_s.add(b)) {
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain_s.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height_s = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height_s << "..." << std::endl;
	Block<StringBlockData>* blk_s = &blockchain_s.getBlockByHeight(height_s);
	if(blk_s != nullptr) {
		std::cout << "Found Block " << height_s << std::endl;
		std::cout << "Hash: " << blk_s->getHash() << " Height: " << blk_s->getHeight() << " Data: " << blk_s->getData()->getRawData() << std::endl;
	}

	std::cout << std::endl;

	std::string path;
	if(__useJSONFormat)
		path = "data/stringblockchain.json";
	else
		path = "data/stringblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<StringBlockData>>* writer = new IO::BlockchainWriter<Block<StringBlockData>>(path, &blockchain_s, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;

}

void createFloatBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	int i = 0;

	std::cout << "Creating float Blockchain" << std::endl;
	Blockchain<Block<FloatBlockData>> blockchain_f = Blockchain<Block<FloatBlockData>>();

	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		float num = (static_cast<float>(rand())) / static_cast<float>(RAND_MAX - 1); // float.max - 1
		Block<FloatBlockData>* b = new Block<FloatBlockData>(new FloatBlockData(num));
		
		if(blockchain_f.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain_f.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}
	unsigned long height_f = 532UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height_f << "..." << std::endl;
	Block<FloatBlockData>* blk_f = &blockchain_f.getBlockByHeight(height_f);
	if(blk_f != nullptr) {
		std::cout << "Found Block " << height_f << std::endl;
		std::cout << "Hash: " << blk_f->getHash() << " Height: " << blk_f->getHeight() << " Data: " << blk_f->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height_f << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/floatblockchain.json";
	else
		path = "data/floatblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<FloatBlockData>>* writer = new IO::BlockchainWriter<Block<FloatBlockData>>(path, &blockchain_f, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createDoubleBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	int i = 0;

	std::cout << "Creating double Blockchain" << std::endl;
	Blockchain<Block<DoubleBlockData>> blockchain_d = Blockchain<Block<DoubleBlockData>>();

	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		float num = (static_cast<double>(rand())) / static_cast<double>(RAND_MAX - 1); // double.max - 1
		Block<DoubleBlockData>* b = new Block<DoubleBlockData>(new DoubleBlockData(num));
		
		if(blockchain_d.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain_d.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}
	unsigned long height_d = 532UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height_d << "..." << std::endl;
	Block<DoubleBlockData>* blk_d = &blockchain_d.getBlockByHeight(height_d);
	if(blk_d != nullptr) {
		std::cout << "Found Block " << height_d << std::endl;
		std::cout << "Hash: " << blk_d->getHash() << " Height: " << blk_d->getHeight() << " Data: " << blk_d->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height_d << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::string path;
	if(__useJSONFormat)
		path = "data/doubleblockchain.json";
	else
		path = "data/doubleblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<DoubleBlockData>>* writer = new IO::BlockchainWriter<Block<DoubleBlockData>>(path, &blockchain_d, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createCharBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	
	std::cout << "Creating char Blockchain" << std::endl;

	Blockchain<Block<CharBlockData>> blockchain_c = Blockchain<Block<CharBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		char c = chars[rand() % (sizeof(chars) - 1)];
			
		Block<CharBlockData>* b = new Block<CharBlockData>(new CharBlockData(c));
		if(blockchain_c.add(b)) {
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain_c.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height_c = 357UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height_c << "..." << std::endl;
	Block<CharBlockData>* blk_c = &blockchain_c.getBlockByHeight(height_c);
	if(blk_c != nullptr) {
		std::cout << "Found Block " << height_c << std::endl;
		std::cout << "Hash: " << blk_c->getHash() << " Height: " << blk_c->getHeight() << " Data: " << blk_c->getData()->getRawData() << std::endl;
	}

	std::cout << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/charblockchain.json";
	else
		path = "data/charblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<CharBlockData>>* writer = new IO::BlockchainWriter<Block<CharBlockData>>(path, &blockchain_c, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createShortBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating short Blockchain" << std::endl;

	Blockchain<Block<ShortBlockData>> blockchain = Blockchain<Block<ShortBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		short num = static_cast<short>(1 + rand() % 32766); // short.max - 1
		Block<ShortBlockData>* b = new Block<ShortBlockData>(new ShortBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<ShortBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/shortblockchain.json";
	else
		path = "data/shortblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<ShortBlockData>>* writer = new IO::BlockchainWriter<Block<ShortBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createLongBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating long Blockchain" << std::endl;

	Blockchain<Block<LongBlockData>> blockchain = Blockchain<Block<LongBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		long num = static_cast<long>(1 + rand() % 2147483646); // long.max - 1
		Block<LongBlockData>* b = new Block<LongBlockData>(new LongBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<LongBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/longblockchain.json";
	else
		path = "data/longblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<LongBlockData>>* writer = new IO::BlockchainWriter<Block<LongBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}


void createLongLongBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating long long Blockchain" << std::endl;

	Blockchain<Block<LongLongBlockData>> blockchain = Blockchain<Block<LongLongBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		long long num = static_cast<long long>(1 + rand() % 9223372036854775806); // long long.max - 1
		Block<LongLongBlockData>* b = new Block<LongLongBlockData>(new LongLongBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<LongLongBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/longlongblockchain.json";
	else
		path = "data/longlongblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<LongLongBlockData>>* writer = new IO::BlockchainWriter<Block<LongLongBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createBoolBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating bool Blockchain" << std::endl;

	Blockchain<Block<BoolBlockData>> blockchain = Blockchain<Block<BoolBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		bool num = static_cast<bool>(rand() % 1); // long long.max - 1
		Block<BoolBlockData>* b = new Block<BoolBlockData>(new BoolBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<BoolBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/boolblockchain.json";
	else
		path = "data/boolblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<BoolBlockData>>* writer = new IO::BlockchainWriter<Block<BoolBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createUnsignedCharBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating unsigned char Blockchain" << std::endl;

	Blockchain<Block<UnsignedCharBlockData>> blockchain = Blockchain<Block<UnsignedCharBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		unsigned char num = static_cast<unsigned char>(rand() % 255); // unsigned char.max - 1
		Block<UnsignedCharBlockData>* b = new Block<UnsignedCharBlockData>(new UnsignedCharBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << static_cast<int>(b->getData()->getRawData()) << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<UnsignedCharBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << static_cast<int>(blk->getData()->getRawData()) << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;
		std::string path;
	if(__useJSONFormat)
		path = "data/unsignedcharblockchain.json";
	else
		path = "data/unsignedcharblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<UnsignedCharBlockData>>* writer = new IO::BlockchainWriter<Block<UnsignedCharBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}
void createUnsignedShortBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating unsigned short Blockchain" << std::endl;

	Blockchain<Block<UnsignedShortBlockData>> blockchain = Blockchain<Block<UnsignedShortBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		unsigned short num = static_cast<unsigned short>(1 + rand() % 65534); // unsigned short.max - 1
		Block<UnsignedShortBlockData>* b = new Block<UnsignedShortBlockData>(new UnsignedShortBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<UnsignedShortBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/unsignedshortblockchain.json";
	else
		path = "data/unsignedshortblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<UnsignedShortBlockData>>* writer = new IO::BlockchainWriter<Block<UnsignedShortBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}
void createUnsignedIntBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating unsigned int Blockchain" << std::endl;

	Blockchain<Block<UnsignedIntBlockData>> blockchain = Blockchain<Block<UnsignedIntBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		unsigned int num = static_cast<unsigned int>(1 + rand() % 4294967294); // unsigned int.max - 1
		Block<UnsignedIntBlockData>* b = new Block<UnsignedIntBlockData>(new UnsignedIntBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<UnsignedIntBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/unsignedintblockchain.json";
	else
		path = "data/unsignedintblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<UnsignedIntBlockData>>* writer = new IO::BlockchainWriter<Block<UnsignedIntBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createUnsignedLongBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating unsigned long Blockchain" << std::endl;

	Blockchain<Block<UnsignedLongBlockData>> blockchain = Blockchain<Block<UnsignedLongBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		unsigned long num = static_cast<unsigned long>(1 + rand() % 4294967294); // unsigned long.max - 1
		Block<UnsignedLongBlockData>* b = new Block<UnsignedLongBlockData>(new UnsignedLongBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<UnsignedLongBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

	std::string path;
	if(__useJSONFormat)
		path = "data/unsignedlongblockchain.json";
	else
		path = "data/unsignedlongblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<UnsignedLongBlockData>>* writer = new IO::BlockchainWriter<Block<UnsignedLongBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createUnsignedLongLongBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating unsigned long long Blockchain" << std::endl;

	Blockchain<Block<UnsignedLongLongBlockData>> blockchain = Blockchain<Block<UnsignedLongLongBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		unsigned long long num = static_cast<long long>(1 + rand() % 18446744073709551614); // unsigned long long.max - 1
		Block<UnsignedLongLongBlockData>* b = new Block<UnsignedLongLongBlockData>(new UnsignedLongLongBlockData(num));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << b->getData()->getRawData() << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<UnsignedLongLongBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/unsignedlonglongblockchain.json";
	else
		path = "data/unsignedlonglongblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<UnsignedLongLongBlockData>>* writer = new IO::BlockchainWriter<Block<UnsignedLongLongBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}

void createCustomDataBlockchain() {
	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();

	std::cout << "Creating Custom Data Blockchain" << std::endl;

	Blockchain<Block<CustomBlockData>> blockchain = Blockchain<Block<CustomBlockData>>();
	int i = 0;
	while( i < 1000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		int num = 1 + rand() % 2147483646; // int.max - 1
		std::string str = "Test String!";
		std::vector<unsigned char> vec = std::vector<unsigned char>(sizeof(BLOCK_HEADER));
		for(int k = 0; k < sizeof(BLOCK_HEADER) - 1; k++){
			vec.push_back(BLOCK_HEADER[k]);
		}
		std::map<std::string, int> map = std::map<std::string, int>();
		map.insert(std::make_pair(Hashing::hashInt(i), i));
		map.insert(std::make_pair(Hashing::hashInt(i + 1), i + 1));
		map.insert(std::make_pair(Hashing::hashInt(i + 2), i + 2));
		map.insert(std::make_pair(Hashing::hashInt(i + 3), i + 3));

		Block<CustomBlockData>* b = new Block<CustomBlockData>(new CustomBlockData(new CustomData(num, str, vec, map)));
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << "0x" << reinterpret_cast<void*>(b->getData()->getRawData()) << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<CustomBlockData>* blk = &blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << blk->getData()->getRawData() << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

		std::string path;
	if(__useJSONFormat)
		path = "data/customblockchain.json";
	else
		path = "data/customblockchain.dat";

	std::cout << "Writing Blockchain to: " << path << std::endl;
	if(!IO::Filesystem::directoryExists("data")) {
		if(!IO::Filesystem::createDirectory("data", NULL))
			throw std::runtime_error("Could Not Create data directory");
	}
	IO::BlockchainWriter<Block<CustomBlockData>>* writer = new IO::BlockchainWriter<Block<CustomBlockData>>(path, &blockchain, !__useJSONFormat);
	if(writer->write())
		std::cout << "Blockchain was successfully written to: " << path << std::endl;
	else
		std::cout << "ERROR! Could not write blockchain to: " << path << std::endl;
	writer->close();
	std::cout << std::endl;
}