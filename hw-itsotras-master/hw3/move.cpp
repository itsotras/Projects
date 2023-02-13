#include "move.h"


// Complete
Move::Move(Board* board)
{
    m.first = Vehicle::INVALID_ID;
    m.second = 0;
    b = new Board(*board); //so we can delete it later
    g = 0;
    h = 0;
    prev = NULL;
}

// To be completed
Move::Move(const Board::MovePair& move, Board* board,  Move *parent):
m(move), g(parent->g+1), prev(parent)
{
    b = new Board(*board); //so we can delete it later

}

// To be completed
Move::~Move()
{
    delete b; //delete the board
}

// To be completed
bool Move::operator<(const Move& rhs) const
{
    // Replace this
    MoveBoardComp comp;
    
    
    if((this->g + this->h) < (rhs.g + rhs.h)){ //if m1 f value is less than m2 f value
        return true;
    }
    else if((this->g + this->h) == (rhs.g + rhs.h)){ //if f scores are equal
        if(this->h < rhs.h){
            return true;
        }
        else if(this->h == rhs.h){ //if hs are equal
            return comp(this, &rhs);
        }
        else{
            return false;
        }
    }

    return false;
}

// To be completed
void Move::score(Heuristic *heur) 
{
    h = heur->compute(*b);//compute the heuristic score
}
