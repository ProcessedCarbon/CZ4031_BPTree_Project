#ifndef H_STORAGE
#define H_STORAGE

#include <iostream>
#include <vector>

#include "block.h"

typedef unsigned int uint;

using namespace std;
struct Storage {
    private:

        int __blocksAccessedCounter; //counter for number of blocks accessed
        int __blocksAllocatedCounter; //counter for number of blocks that have been allocated

        
        uint __currentBlockSizeUsed; //size in B used up in the current block being pointed to
        uint __totalBlockSizeUsed; //size in B of utilised storage based on blocks
        uint __totalRecordSizeUsed; //size in B of utilised storage based on records
        uint __memorySize; //size in B of the allocated memory
        uint __blockSize; //size in B of a block in memory

        void *__memoryPtr; //pointer to the starting address of allocated memory
        void *__blockPtr; //pointer to the starting address of the current block inside the allocated memory
    
    public:
        /**
         * @brief Construct a new Storage object using the default constructor.
         * 
         */
        Storage() = default;
        Storage(uint blockSize, uint memorySize);

        bool createNewBlock();
        tuple<uint, void *> writeRecordToDb(uint recordSize);
        uint getNumberOfBlocks();
        uint getDatabaseSizeBasedOnBlocks();
        uint getDatabaseSizeBasedOnRecords();
        
        vector<Block> __blocks;  // Shared pointer used to automatically destroy block when it is empty
        void AddBlock(Block block);
        // Getters
        int getNumBlocks();
        int getNumRecords();
        int getStorageSize();

        ~Storage();
};

#endif   
