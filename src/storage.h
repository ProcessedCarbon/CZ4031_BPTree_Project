#ifndef H_STORAGE
#define H_STORAGE

#include <iostream>
#include <vector>

#include "block.h"

using namespace std;
struct Storage {
    public:
        vector<Block> __blocks;  // Shared pointer used to automatically destroy block when it is empty
        void AddBlock(Block block);

        // Getters
        int getNumBlocks();
        int getNumRecords();
        int getStorageSize();

        ~Storage() = default;
};

#endif   
