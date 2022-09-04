#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


#include "record.h"

using namespace std;

//to change file path to actual data set eventually
#define FILEPATH "../data/testdata.tsv"
#define NEWLINE '\n'
#define ROW_DELIMITER '\t'

int main()
{

    int BLOCK_SIZE;
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

  cout <<" ==================================== " << NEWLINE << " READING IN DATA FROM FILE data.tsv" <<endl;
  ifstream datafile(FILEPATH); // actual data
  // std::ifstream file("../data/testdata.tsv"); // testing data
  

  // Insert data into database and populate list of addresses
  if (datafile.is_open())
  {
    int recordCounter = 0;
    string row;

    while (getline(datafile, row))
    {
      cout << "The row is: " << row << endl;
      //Row is: tt0000001  5.6     1645

      //0 means is column header
      if (recordCounter != 0) {

      Record record;
      stringstream linestream(row); //linestream is an object of stringstream that references the string of each row 
      string data; // column data of each record

      // store movie id in record struct
      getline(linestream, data, ROW_DELIMITER);
      strcpy(record.__movieId, data.c_str());
      cout << record.__movieId << endl;

      // store average rating in record struct
      getline(linestream, data, ROW_DELIMITER);
      record.__avgRating = stof(data);
      cout << record.__avgRating << endl;

      // store number of votes in record struct
      getline(linestream, data, ROW_DELIMITER);
      record.__numVotes = stoi(data);
      cout << record.__numVotes<< endl;
      }

      // //logic to insert record in to database 
      // //logic to insert record into B+ tree

      ++recordCounter;
      cout << "Record number " << recordCounter << " has been inserted." << endl;
    }
    datafile.close();
  }

}