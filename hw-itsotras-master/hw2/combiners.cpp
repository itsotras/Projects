#include "searcheng.h"
#include "combiners.h"

// Complete the necessary code
WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
    WebPageSet newSet;
    for(WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it){ //loop through setA
        if(setB.find(*it) != setB.end()){ //if this is also in setB 
            newSet.insert(*it);
        }
    }
    return newSet;
}

WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){ //union of the sets
    WebPageSet newSet = setA; //set it as the first set, cause we are doing a union

    for(WebPageSet::iterator it = setB.begin(); it != setB.end(); ++it){ //loop through setB runtime is theta(mlogm)
        if(newSet.find(*it) == newSet.end()){ //if this not in the newSet already
            newSet.insert(*it);
        }
    }


    return newSet; //full runtime istheta(mlogm)
    
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
    WebPageSet newSet;
    for(WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it){ //loop through setA
        if(setB.find(*it) == setB.end()){ //if this not in setB 
            newSet.insert(*it);
        }
    }
    return newSet;
}