#include "record.h"

using namespace std;

int Record::getRecordSize(){
        return sizeof(__movieName) + sizeof(__avgRating) + sizeof(__numVotes);
}

string Record::RecordStringFormat(){
    return __movieName + ":" + to_string(__avgRating) + " | " + to_string(__numVotes) + "\n";
}

string Record::getMovieName(){
    return __movieName;
}

float Record::getAverageRating(){
    return __avgRating;
}

int Record::getNumVotes(){
    return __numVotes;
}

void Record::setRecord(string movieName, float averageRating, int numVotes){
    __movieName = movieName;
    __avgRating = averageRating;
    __numVotes = numVotes;
}