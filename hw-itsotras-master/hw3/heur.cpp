#include <cmath>
#include "heur.h"
#include <vector>
#include <set>
using namespace std;

// Complete
size_t BFSHeuristic::compute(const Board& b)
{
    return 0U;
}

int DirectHeuristic::findDirectVehicles(const Board& b){//for the directHeuristic
    Vehicle a = b.escapeVehicle(); //get the escape vehicle
    int startRow = a.startr;
    int startCol = a.startc;
    int endCol = startCol+a.length-1;
    int i = 1;
    int directV = 0;
    while(endCol+i < b.size()){
        if(b.at(startRow, endCol+i) != '.'){ //if there is a vehicle there
            directV++;
        }
        i++;
    }
    return directV;
}

// To be completed
size_t DirectHeuristic::compute(const Board& b)
{
    // Avoid compiler warnings -- replace this
    return (size_t) findDirectVehicles(b);
}

// To be completed
int IndirectHeuristic::findDirectVehicles(const Board& b){//for the IndirectHeuristic
    Vehicle a = b.escapeVehicle(); //get the escape vehicle
    int startRow = a.startr;
    int startCol = a.startc;
    int endCol = startCol+a.length;
    int i = 0;

    while(endCol+i < b.size()){
        if(b.at(startRow, endCol+i) != Vehicle::INVALID_ID){ //if there is a vehicle there
            directVehicles.insert(b.at(startRow, endCol+i));//push back to vehicle
        }
        i++;
    }
    return directVehicles.size();
}

int IndirectHeuristic::findIndirectVehicles(const Board& b){
    int indirectVehicles = 0;
    for(std::set<Vehicle::VID_T>::iterator it = directVehicles.begin(); it!= directVehicles.end(); ++it){//loop through vector
        Vehicle currentV = b.vehicle(*it);//get the current vehicle
        currAbove.clear();//clear the set
        currBelow.clear();//clear those below this vehicle
        if(oneEscape(b, currentV)){//if we only have one escape
            indirectVehicles += (currAbove.size()+currBelow.size()); //update Indirect Vehicles, one will be 0 if we get here
        }
        
        
    }

    return indirectVehicles;
}

void IndirectHeuristic::findAbove(const Board& b, const Vehicle v){
    int c = v.startc;
    for(int r = v.startr-1; r >=0; r--){
        if(b.at(r, c) != '.' && (visited.find(b.at(r, c)) == visited.end())){
            currAbove.insert(b.at(r, c));//insert this into my current Indirect set
            visited.insert(b.at(r,c));
        }

    }

}

void IndirectHeuristic::findBelow(const Board& b, const Vehicle v){
    
    int c = v.startc;
    for(int r = v.startr+v.length; r <b.size(); r++){
        if(b.at(r, c) != '.' && (visited.find(b.at(r, c)) == visited.end())){
            currBelow.insert(b.at(r, c));//insert this into my current Indirect set
            visited.insert(b.at(r,c));
        }

    }
}

bool IndirectHeuristic::oneEscape(const Board& b, const Vehicle v){
    Vehicle a = b.escapeVehicle();
    int numEscapes = 0;
    int botRow = v.startr+v.length-1;//bottomRow of this vehicle
    int topRow = v.startr; //top row of this vehicle
    if(botRow - (topRow-0) < a.startr){//if the possible # of moves up is less than the where a is in the row, if there is an escape up wards
        findAbove(b, v);//get those above me
        numEscapes++; //increase # of escapes
    }
    if(topRow + (b.size()-1 - botRow) > a.startr){//if the possible # of spaces up is more than the where a is in the row, if there is an escape below me
        findBelow(b, v);//get those below me
        numEscapes++; //increase # of escapes
    }

    return (numEscapes == 1); //returns true if there is only one escape



}

size_t IndirectHeuristic::compute(const Board& b)
{
    // Avoid compiler warnings -- replace this
    directVehicles.clear();
    currAbove.clear();
    currBelow.clear();//clear all the sets of these for the next one to use
    visited.clear();
    int directVs = findDirectVehicles(b);
    int indirectVs = findIndirectVehicles(b);
    return (size_t) directVs+indirectVs;
}

