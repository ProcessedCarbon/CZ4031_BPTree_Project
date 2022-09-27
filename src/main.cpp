#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "record.h"
#include "storage.h"
#include "bpTree.h"

using namespace std;

typedef unsigned int uint;

//defining constants
//to change file path to actual data set eventually
//#define FILEPATH "../data/testdata.tsv"
#define FILEPATH "../data/testdata1.tsv"
//#define FILEPATH "../data/data.tsv"
#define NEWLINE '\n'
#define ROW_DELIMITER '\t'
#define DISK_CAPACITY 500000000 //500,000,000 = 500MB
#define COUT_LINE_DELIMITER "==================================================================="
#define MB 1000000

//function declarations
void printExperiment1Results(Storage *disk);
void printExperiment2Results(BPTree t);
void printExperiment3Results(BPTree t);

//main entry point
int main()
{
    
  uint BLOCK_SIZE;
  string userSelection=" ";
  do {
  cout << "Select Block Size (Enter 1 or 2): " << NEWLINE << "1. 200B" << NEWLINE << "2. 500B" << endl;
  cin >> userSelection;
  } while (userSelection.compare("2") != 0 && userSelection.compare("1") != 0);
  switch (stoi(userSelection)) {
  case 1:
    BLOCK_SIZE = 200;
    break;
  case 2:
    BLOCK_SIZE = 500;
    break;
  }
  cout << "Your selection is: " << BLOCK_SIZE << "B" << endl;

  // Compute N given blocksize
  int sizeOfOtherMembersInNode = (sizeof(int) * 4) + sizeof(bool) + sizeof(Node *); // other data less ptr and key

  // Need space for both ptr to child as well as the keys
  int n = (BLOCK_SIZE  -  sizeOfOtherMembersInNode) / (sizeof(int) + sizeof(void **)); 

  cout << "Max keys in each B+ Tree Node (n) = " << n << endl;

  BPTree bptree(n);

  //allocate a fraction of main memory for disk storage
  Storage disk{BLOCK_SIZE, DISK_CAPACITY};

  cout << COUT_LINE_DELIMITER << NEWLINE << "READING IN DATA FROM FILE: data.tsv" << NEWLINE << "Please wait..." << endl;
  ifstream tsvData(FILEPATH); //read data

  if (tsvData.is_open())
  {
    int recordCounter = 0;
    string row;
    
    // remove the row of column headers.
    getline(tsvData,row);

    while (getline(tsvData, row))
    {
      // cout << "The row is: " << row << endl;
      //Row is: tt0000001  5.6     1645

      Record record;
      stringstream linestream(row); //linestream is an object of stringstream that references the string of each row 
      string data; // column data of each record

      // store movie id in record struct
      getline(linestream, data, ROW_DELIMITER);
      strcpy(record.__movieId, data.c_str());
      // cout << record.__movieId << endl;

      // store average rating in record struct
      getline(linestream, data, ROW_DELIMITER);
      record.__avgRating = stof(data);
      // cout << record.__avgRating << endl;

      // store number of votes in record struct
      getline(linestream, data, ROW_DELIMITER);
      record.__numVotes = stoi(data);

      //Insert record into database
      tuple<uint, void *> recordAddressInfo = disk.writeRecordToDb(sizeof(record));

      // //logic to Insert record into B+ tree
      bptree.InsertWithAssignParents(record.__numVotes, get<1>(recordAddressInfo));

      ++recordCounter;
      //cout << "Record number " << recordCounter << " has been Inserted." << endl;
    }
    tsvData.close();
  }

  //Print results for Experiment 1 (currently this only includes the records, no index YET...)
  //Pass the Storage disk by address instead of value to avoid creating a new copy which could result in double free errors.
  printExperiment1Results(&disk);
  //cout << "[TREE]"<< endl;
  //bptree.DisplayTree(bptree.root);

  printExperiment2Results(bptree);
  //printExperiment3Results(bptree);
}

void printExperiment1Results(Storage *disk) {
  cout << COUT_LINE_DELIMITER << NEWLINE << "Experiment 1 Results" << NEWLINE << COUT_LINE_DELIMITER << endl;
  cout << "The number of blocks used is: " << disk->getNumberOfBlocks() << endl;
  cout << "The database size (in B) based on blocks is: " << disk->getDatabaseSizeBasedOnBlocks() << endl;
  cout << "The database size (in B) based on records is: " << disk->getDatabaseSizeBasedOnRecords() << endl;
  cout << "The database size (in MB) based on blocks is: " << double(disk->getDatabaseSizeBasedOnBlocks())/MB << endl;
  cout << "The database size (in MB) based on records is: " << double(disk->getDatabaseSizeBasedOnRecords())/MB << endl;
}
void printExperiment2Results(BPTree t){
  cout << COUT_LINE_DELIMITER << NEWLINE << "Experiment 2 Results" << NEWLINE << COUT_LINE_DELIMITER << endl;
  cout << "The max number of keys per node (n): " << t.MAX << endl;
  cout << "The height of the tree: " << t.getHeight() << endl;
  cout << "The number of nodes: " << t.getNumNodes() << endl;

  cout << "Root content is: ";
  t.root->printNodeContent(t.root);

  cout << "First child of root is: ";
  ((Node *)t.root->ptr[0])->printNodeContent(((Node *)t.root->ptr[0]));
}

void printExperiment3Results(BPTree t){
  cout << COUT_LINE_DELIMITER << NEWLINE << "Experiment 3 Results" << NEWLINE << COUT_LINE_DELIMITER << endl;
  cout << "[DELETING KEYS]" << endl;
  t.DeleteAndCheckNewRoot(90);
  cout << "NEW BP TREE" << NEWLINE << COUT_LINE_DELIMITER << endl;
  t.DisplayTree(t.root);
}

