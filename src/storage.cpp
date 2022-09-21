#include <iostream>
#include <vector>

#include "storage.h"

using namespace std;

/**
 * @brief Construct a new Storage:: Storage object.
 * 
 * @param blockSize Size of the user inputted block in B.
 * @param memorySize Size of the memory allocated in B.
 */
Storage::Storage(uint blockSize, uint memorySize) {
    __blockSize = blockSize;
    __memorySize = memorySize;

    __totalBlockSizeUsed = 0; 
    __totalRecordSizeUsed = 0;
    __currentBlockSizeUsed = 0;

    __memoryPtr = operator new(memorySize);
    memset(__memoryPtr, '\0', memorySize); // copy null character to the memory allocated
    __blockPtr = nullptr; // initialise the block pointer to NULL

    __blocksAllocatedCounter = 0;
    __blocksAccessedCounter = 0;
    
}

/**
 * @brief Checks if there is sufficient space in the allocated memory to create a new block.
 * 
 * @param memorySize Size of the memory allocated.
 * @param totalBlockSizeUsed Total Size of the Blocks created so far.
 * @param blockSize Size of block.
 * @return true If there is enough space in memory to create a new block.
 * @return false If there is insufficient space in memory to create a new block.
 */
bool isSpaceInMemory(uint memorySize, uint totalBlockSizeUsed, uint blockSize) {
    if (memorySize < totalBlockSizeUsed + blockSize) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief Increment respecitve counters and update pointers if a new block is created.
 * 
 * @return true If a new block is created in the allocated memory.
 * @return false If a new block cannot be created in the allocated memory.
 */
bool Storage::createNewBlock() {
    if (isSpaceInMemory(__memorySize, __totalBlockSizeUsed, __blockSize)) {
        __totalBlockSizeUsed += __blockSize;
        __blockPtr = (char *)__memoryPtr + (__blockSize * __blocksAllocatedCounter);
        ++__blocksAllocatedCounter;
        __currentBlockSizeUsed = 0; //reset to 0 since a new block is created

        // cy
        //AddBlock(Block(__blockSize));

        return true;
    } else {
        return false; //not enough memory space to create a new block
    }
}

/**
 * @brief Checks if there is enough space in the block to add a new record.
 * 
 * @param blockSize Size of the Block specified.
 * @param currentBlockSizeUsed Size of the block pointed to that has been occupied.
 * @param recordSize Size of each record.
 * @return true If there is enough space in the block to add a new record.
 * @return false If there is insufficient space in the block to add a new record.
 */
bool isSpaceInBlock(uint blockSize, uint currentBlockSizeUsed, uint recordSize) {
    if (blockSize >= (recordSize + currentBlockSizeUsed)) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Writes the record to the database if there is enough space in the block to 
 * accomodate the record.
 * 
 * @param recordSize Size of the record to be written into the database.
 * @return tuple<uint, void *> A tuple pair of the size of block that has been used
 * and the start address of the block.
 */
tuple<uint, void *> Storage::writeRecordToDb(uint recordSize) {

    if (!isSpaceInBlock(__blockSize, __currentBlockSizeUsed, recordSize) || __blocksAllocatedCounter == 0) {
        bool isBlockCreated = createNewBlock();
        if (!isBlockCreated) {
            cout << "There is no more space in the memory allocated to create a new block" << endl;
            throw "There is no more space in the memory allocated to create a new block";
        }
    } else if (!(recordSize <= __blockSize)) {
        cout << "The record size is larger than block size and cannot be written" << endl;
        throw "The record size is larger than block size and cannot be written";
    }
    __totalRecordSizeUsed += recordSize;

    /*Here, the address of the record can be retrieved using the following concept:
     Start address of current block(__blockPtr) + address to be offset(__currentBlockSizeUsed) */
    tuple<uint, void*> writtenRecordAddressInfo(__currentBlockSizeUsed, __blockPtr);
    
    __currentBlockSizeUsed += recordSize;
    return writtenRecordAddressInfo;

}

uint Storage::getNumberOfBlocks() {
    return __blocksAllocatedCounter;
}

uint Storage::getDatabaseSizeBasedOnBlocks() {
    return __totalBlockSizeUsed;
}

uint Storage::getDatabaseSizeBasedOnRecords() {
    return __totalRecordSizeUsed;
}

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

/**
 * @brief Destroy the Storage:: Storage object. Reset the memory pointer to null.
 * 
 */
Storage::~Storage() {
    delete (char *)__memoryPtr;
    __memoryPtr = nullptr;
}






