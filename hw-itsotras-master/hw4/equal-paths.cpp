#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here


int giveMaxPath(Node* curr,  bool& equalPath);

bool equalPaths(Node * root)
{
    // Add your code below
    bool equal = true;
    giveMaxPath(root, equal);
    return equal;
    
}

int giveMaxPath(Node* curr, bool& equalPath){
    if(curr == nullptr){
        return 0;
    }
    else{
        int tempLeft = giveMaxPath(curr->left, equalPath); //get the max path from the left node
        int tempRight = giveMaxPath(curr->right, equalPath); //get the max path from the right node
        
        if(tempLeft == 0){ //if left has depth of 0
            return tempRight+1;
        }
        else if(tempRight == 0){//if right path has depth of 0
            return tempLeft+1;
        }

        if(tempLeft != tempRight){//if they are not equal
            equalPath = false; //they don't have equal paths, so false
            if(tempLeft > tempRight){ //return whichever one is larger +1
                return tempLeft+1;
            }
            else{
                return tempRight+1;
            }
        }
        else {//if they are the same add the same number +1
            return tempLeft+1;
        }



    }
}

