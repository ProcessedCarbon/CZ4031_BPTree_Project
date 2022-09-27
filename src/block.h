#ifndef H_BLOCK
#define H_BLOCK

#include <vector>
#include <memory>

#include "record.h"

using namespace std;

struct Block {
    private:
        int __blockSize;

    public:
        vector<Record> __records;

        Block(int blockSize){
            __blockSize = blockSize;
        }

        void AddRecord(Record record);

        void DeleteRecord(int numVotes);

        // Getters
        int getNumRecord();
        int getBlockSize();

        vector<Record> FindRecords(int numVotes);

        void ShowRecordsInBlock();

        bool checkSpace();

        ~Block() = default;
};

#endif