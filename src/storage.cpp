
#include <iostream>
#include <vector>

#include "storage.h"

using namespace std;
void Storage::AddBlock(Block block) {
    __blocks.push_back(block);
}

int Storage::getNumBlocks() {
    return __blocks.size();
}

int Storage::getNumRecords() {
    int numRecords = 0;
    for(Block b : __blocks){
        numRecords += b.getNumRecord();
    }
    return numRecords;
}

int Storage::getStorageSize() {
    int storageSize = 0;
    for(Block b : __blocks){
        storageSize += b.getBlockSize();
    }
    return storageSize;
}






