#ifndef HEUR_H
#define HEUR_H

#include "board.h"

class Heuristic
{
public:
    virtual ~Heuristic() { }
    virtual size_t compute(const Board& b) = 0;
};

class BFSHeuristic : public Heuristic
{
public:
    size_t compute(const Board& b);
};


class DirectHeuristic : public Heuristic
{
public:
    size_t compute(const Board& b);
private:
    int findDirectVehicles(const Board& b);
    
};

class IndirectHeuristic : public Heuristic
{
public:
    size_t compute(const Board& b);
private:
    int findDirectVehicles(const Board& b);
    int findIndirectVehicles(const Board& b);
    std::set<Vehicle::VID_T> directVehicles;
    std::set<Vehicle::VID_T> currAbove;//set of vehicles above me
    std::set<Vehicle::VID_T> currBelow;//set of vehicles below me
    std::set<Vehicle::VID_T> visited; //set of vehicles we have already visited
    bool oneEscape(const Board& b, const Vehicle v);
    void findAbove(const Board& b, const Vehicle v);
    void findBelow(const Board& b, const Vehicle v);
};


#endif
