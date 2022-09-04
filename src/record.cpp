#include "record.h"

using namespace std;

int Record::getRecordSize(){
        return sizeof(__movieId) + sizeof(__avgRating) + sizeof(__numVotes);
}

// string Record::RecordStringFormat(){
//     return __movieId + ":" + to_string(__avgRating) + " | " + to_string(__numVotes) + "\n";
// }

char* Record::getMovieId(){
    return __movieId;
}

float Record::getAverageRating(){
    return __avgRating;
}

int Record::getNumVotes(){
    return __numVotes;
}

void Record::setRecord(char *movieId, float averageRating, int numVotes){
    strcpy(movieId, __movieId);
    __avgRating = averageRating;
    __numVotes = numVotes;
}
