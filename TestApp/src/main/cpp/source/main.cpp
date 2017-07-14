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
#include "block.hpp" // Block Stuff
#include "blockchain.hpp" // Blockchain Stuff

using namespace BlockchainCpp;

int main(int argc, char** argv) {

	//Uncomment when debugging in Visual Studio
	// char ch;
	// std::cin >> ch;

	Blockchain<Block<int>> blockchain = Blockchain<Block<int>>();
	Blockchain<Block<std::string>> blockchain_s = Blockchain<Block<std::string>>();

	auto t = std::chrono::high_resolution_clock::now();
	auto generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	int i = 0;
	int j = 0;

	std::cout << "Creating int " << "Blockchain" << std::endl;

	while( i < 5000 ) {
		t = std::chrono::high_resolution_clock::now();
		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
		srand(generator);
		int data = 1 + rand() % 2147483646; // int.max - 1
		Block<int>* b = new Block<int>(&data);
		
		if(blockchain.add(b)){
			std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << *(b->getData()) << std::endl;
			std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
			std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
		}
		i++;
	}

	unsigned long height = 999UL;
	std::cout << std::endl << std::endl;
	std::cout << "Finding Block " << height << "..." << std::endl;
	Block<int>* blk = blockchain.getBlockByHeight(height);
	if(blk != nullptr) {
		std::cout << "Found Block " << height << std::endl;
		std::cout << "Hash: " << blk->getHash() << " Height: " << blk->getHeight() << " Data: " << *(blk->getData()) << std::endl;
	}
	else {
		std::cout << "Block " << height << " Not Found" << std::endl;
	}

	std::cout << std::endl << std::endl;

	// std::cout << "Creating string" << "Blockchain" << std::endl;

	// while( j < 5000 ) {
	// 	std::string str = "";
	// 	for(int count = 0; count < 50; i++){
	// 		t = std::chrono::high_resolution_clock::now();
	// 		generator = std::chrono::time_point_cast<std::chrono::nanoseconds>(t).time_since_epoch().count();
	// 		srand(generator);
	// 		char c = static_cast<char>(65 + rand() % 122); // A - z
	// 		str += c;
	// 	}
		
	// 	Block<std::string>* b = new Block<std::string>(&str);
	// 	if(blockchain_s.add(b)) {
	// 		std::cout << "Hash: " << b->getHash() << " Height: " << b->getHeight() << " Data: " << *(b->getData()) << std::endl;
	// 		std::cout << "Block " << b->getHeight() << " Was Successfully Added to the Blockchain" << std::endl;
	// 		std::cout << "Blockchain Contains " << blockchain.getBlocks().size() << " Blocks" << std::endl;
	// 	}
	// }

	// unsigned long height_s = 1234UL;
	// std::cout << std::endl << std::endl;
	// std::cout << "Finding Block " << height << "..." << std::endl;
	// Block<std::string>* blk_s = blockchain_s.getBlockByHeight(height);
	// if(blk != nullptr) {
	// 	std::cout << "Found Block " << height << std::endl;
	// 	std::cout << "Hash: " << blk_s->getHash() << " Height: " << blk_s->getHeight() << " Data: " << *(blk_s->getData()) << std::endl;
	// }

	return 0;
}