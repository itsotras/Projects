#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "solver.h"

using namespace std;

// To be completed
Solver::Solver(const Board& b, Heuristic *heur):
b_(b), heur_(heur)
{

}

// To be completed
Solver::~Solver()
{
    for(MoveSet::iterator it = closedList.begin(); it != closedList.end(); ++it){ //delete all the moves
        delete (*it);

    }
}

// To be completed
bool Solver::run()
{
    Board newB = Board(b_); //make a new board
    Move *start = new Move(&newB);
    openList.push(start);
    closedList.insert(start);
    expansions_ = 1;

    while(! openList.empty() ){//while open list is not empty
        
        Move* s = openList.top(); //get the top and pop it out
        openList.pop();
        

        if(s->b->solved()){//if we make it
            backtrace_ = s;
            break;
        }
        else{//if open list is still full
            Board::MovePairList newMoves = s->b->potentialMoves();//make the list of newMoves
            expansions_+=newMoves.size();//make expansions the number of these possible moves
            for(Board::MovePairList::iterator it = newMoves.begin(); it != newMoves.end(); ++it){//iterate through newMoves

                Board newBoard = Board(*(s->b));//get the board
                newBoard.move(it->first, it->second);//update it
                Move* newMove = new Move(*it, &newBoard, s); //create the new move
                newMove->score(heur_);//score h

                if(closedList.find(newMove) == closedList.end()){//if not in the set
                    closedList.insert(newMove);//insert it

                    openList.push(newMove); //for deallocating

                }
                else{ //if in the closed Set already
                    expansions_--; //delete if already in the set

                        delete newMove;

                }

            }
            

        }

    }
    return backtrace_ != nullptr; //if backtrace_ is nullptr, there is no solution
}

// To be completed
Board::MovePairList Solver::solution() const
{

    Board::MovePairList moveOrder;
    process(backtrace_, moveOrder);
    return moveOrder;
}

void Solver::process(Move* move, Board::MovePairList &moveOrder) const{
    if(move->prev == nullptr){
        return;
    }
    else{
        process(move->prev, moveOrder); //get to the end
        Board::MovePair copy = move->m;
        moveOrder.push_back(copy); //push back my m
    }
}

// Complete
size_t Solver::numExpansions() const
{
    return expansions_;
}
