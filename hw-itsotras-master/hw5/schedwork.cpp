

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, int row, int col, const size_t d, const size_t m, DailySchedule& sched, int maxRow, int maxCol);
bool isValid(const AvailabilityMatrix& avail, int row, int col, int j,const size_t m, DailySchedule& sched);//tells us if this position is a valid person's ID to add to sched
// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    int maxR = avail.size()-1;
    int maxC = avail[0].size()-1;

    for(int i = 0; i<= (maxR); i++){//initialize vector with INVALID IDS
        std::vector<Worker_T> myVec;
        for(int j = 0; j< (int)(dailyNeed); j++){
            myVec.push_back(INVALID_ID);
        }
        sched.push_back(myVec);
    }
    return scheduleHelper(avail, 0, 0, dailyNeed, maxShifts, sched, maxR, maxC);//start at the first position in the availability matrix



}

bool scheduleHelper(const AvailabilityMatrix& avail, int row, int col, const size_t d, const size_t m, DailySchedule& sched, int maxRow, int maxCol){//recursive function
    
    if(row == maxRow+1){//if we reach the end of the rows
        return true;
    }
    else if(col == maxCol+1){//if we reach the end of the columns
        return scheduleHelper(avail, row+1, 0, d, m, sched, maxRow, maxCol);//get the bool of the next value we add
    }
    else{
        if(sched[row][col] == INVALID_ID){//we need to change it
            for(int i = 0; i <= maxCol; i++){//loop through all the possible values
                if(avail[row][i] == true){
                    sched[row][col] = i;//add this new ID
                    if(isValid(avail, row, col, i, m, sched)){//if it is valid for that ID to be added
                        bool status = scheduleHelper(avail, row, col+1, d, m, sched, maxRow, maxCol);//get the status of the next one we add
                        if(status){
                            return true;
                        }
                    }
                }else{
                    //do nothing
                }
            }
            sched[row][col] = INVALID_ID;//reset to INVALID
            return false;

        }
        else{
            return scheduleHelper(avail, row, col+1, d, m, sched, maxRow, maxCol);
        }
    }   
        

}


bool isValid(const AvailabilityMatrix& avail, int row, int col, int j,const size_t m, DailySchedule& sched){
    int counter = 0;
    for(int i = 0; i < (int)(sched.size()); i++){//go through all the rows and count how many times I appear
        counter += std::count(sched[i].begin(), sched[i].end(), j);
    }
    if(counter <= (int)(m)){//if my ID is in the valid # of shifts
        if(std::count(sched[row].begin(), sched[row].end(), j) == 1){//if this value is the only one in the row we're at
            return true;//this is valid
        }
        else{
            return false;//if there is more than one of us in the row, it is not valid
        }
    }
    else{
        return false;//return false
    }

}

