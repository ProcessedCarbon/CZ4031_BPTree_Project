#ifndef H_RECORD
#define H_RECORD

#include <string>
#include <cstring>

#define TCONSTSIZE 10 

using namespace std;
/* Representing a record in the data base */
struct Record{
    // private:
    //     char __movieId[10]; //explicitly declare the length for easier byte tracking
    //     // string __movieName;
    //     float __avgRating;
    //     int __numVotes;

    public:
        Record() = default; //explicitly declare default constructor if no arguments are passed.
        char __movieId[TCONSTSIZE]; //explicitly declare the length for easier byte tracking
        // string __movieName;
        float __avgRating;
        int __numVotes;
        Record(char *movieId, float averageRating, int numVotes){
            strcpy(movieId, __movieId);
            __avgRating = averageRating;
            __numVotes = numVotes;
        }
        // Getters
        int getRecordSize();
        char* getMovieId();
        float getAverageRating();
        int getNumVotes();

        // Setter
        void setRecord(char *movieId, float averageRating, int numVotes);

        // string RecordStringFormat();

        ~Record() = default;
};

#endif
