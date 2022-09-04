#ifndef H_RECORD
#define H_RECORD

#include <string>
using namespace std;
/* Representing a record in the data base */
struct Record{
    private:
        string __movieName;
        float __avgRating;
        int __numVotes;

    public:
        Record(string movieName, float averageRating, int numVotes){
            __movieName = movieName;
            __avgRating = averageRating;
            __numVotes = numVotes;
        }
        
        // Getters
        int getRecordSize();
        string getMovieName();
        float getAverageRating();
        int getNumVotes();

        // Setter
        void setRecord(string movieName, float averageRating, int numVotes);

        string RecordStringFormat();

        ~Record() = default;
};

#endif