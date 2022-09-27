#include <iostream>

#include "block.h"

using namespace std;

void Block::AddRecord(Record record){
    __records.push_back(record);
}

void Block::DeleteRecord(int numVotes){
    if(__records.size() == 0){
        cerr << "[ERROR] Unable to delete record, no records avaliable" << endl;
        return;
    }

    cout << "Deleting......." << endl;
    for(int i = 0; i < __records.size(); i++){
        if(__records[i].getNumVotes() == numVotes){
            // cout << __records[i].getMovieId() + ": " + to_string(__records[i].getNumVotes()) + "," << endl;
            __records.erase(__records.begin() + i);
        }
    }
}

vector<Record> Block::FindRecords(int numVotes){
    vector<Record> foundRecords;
    for (Record r : __records) {
        if (r.getNumVotes() == numVotes) {
            foundRecords.push_back(r);
        }
    }
    // return empty record, but should not happen
    return foundRecords;
}

int Block::getNumRecord(){
    return __records.size();
}

int Block::getBlockSize(){
    return __blockSize;
}

void Block::ShowRecordsInBlock(){
    for (Record r : __records) {
        cout << r.getMovieId() << " ,";
    }
    cout << endl;
}

bool Block::checkSpace() {
    return __records.size() < __blockSize;
}