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
            blockSize = __blockSize;
        }

        void AddRecord(Record record);

        void DeleteRecord(float numVotes);

        // Getters
        int getNumRecord();
        int getBlockSize();

        vector<Record> FindRecords(float numVotes);

        void ShowRecordsInBlock();

        bool checkSpace();

        ~Block() = default;
};

#endif