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
#include <iostream> // std::cout
#include <string> // std::string
#include <algorithm> // std::find_if
#include <vector> // std::vector
#include <map> // std::map
#include <stdexcept> // throw std::runtime_error()
#include "constants.hpp" // MAX_BLOCK_SIZE

namespace BlockchainCpp {


	template <class BlockType>

	class Blockchain {

	public:
		Blockchain(){

		}

		Blockchain(std::map<std::string, BlockType*>, std::vector<Blockchain<BlockType>> orphanChains) : public Blockchain() {
			
			this->blocks = hashedBlocks;
			this->orphanChains = orphanChains;
		}

		template <class BlockType>

		bool add(BlockType* block) {
			if(block->getData() == nullptr)
				throw std::runtime_error("Can not add a null block to a blockchain");
			
			block->setHeight(this->count + 1);
			block->setIndex(this->count);

			if(lastBlock == nullptr)
				block->setPreviousHash("");
			else
				block->setPreviousHash(lastBlock->getHash());

			block->mine();
			block->lock();

			std::string hash = block->getHash();
			if(!containsBlockByHash(hash)) {
				this->blocks.insert( {hash, block} );
				this->count++;
				lastBlock = block;
				return true;
			}
			std::cout << "[WARNING] Block with Hash: " << hash << " was already present in the blockchain Skipping" << std::endl;
			return false;
		}


		bool containsBlockByHash(std::string hash){
			if(this->blocks.find(hash) == this->blocks.end())
				return false;
			
			return true;
		}

		BlockType* getBlockByHash(std::string hash){
			if(!this->containsBlockByHash(hash))
				return nullptr;

			return this->blocks[hash];
		}

		bool containsBlockByHeight(unsigned long height) {
			return height <= count + 1;
		}

		BlockType* getBlockByHeight(unsigned long height) {
			if(!this->containsBlockByHeight(height))
				return nullptr;
			
			std::string hash = this->lastBlock->getHash();
			BlockType* blk = this->lastBlock;

			while(hash != ""){
				blk = getBlockByHash(hash);
				if(blk == nullptr)
					break;
				if(blk->getHeight() == height)
					return blk;
				hash = blk->getPreviousHash();
			}

			return nullptr;
		}

		std::map<std::string, BlockType*> getBlocks() const {
			return this->blocks;
		}

		std::vector<Blockchain<BlockType>> getOrphanedChains() const {
			return this->orphanChains;
		}

		BlockType* getLastBlock() const {
			return this->lastBlock;
		}

		unsigned long getCount() const {
			return this->count;
		}

		virtual ~Blockchain() {
			// for(int i = 0; i < this->blocks->size() - 1; i++) {
			// 	delete this->blocks[i];
			// }
			// delete this->blocks;

			// for(int i = 0; i < this->orphanChains->size(); i++) [
			// 	delete this->orphanChains[i];
			// ]
			// delete this->orphanChains;
		}
	protected:

	private:
		BlockType* lastBlock = nullptr;
		std::map<std::string, BlockType*> blocks = std::map<std::string, BlockType*>();
		std::vector<Blockchain<BlockType>*> orphanChains = std::vector<Blockchain<BlockType>*>();
		unsigned long count = 0;
	};
}