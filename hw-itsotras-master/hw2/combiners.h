#ifndef COMBINERS_H
#define COMBINERS_H

#include "searcheng.h"

/*** Define Derived WebPageSetCombiners (for AND, OR, DIFF) classes below ***/
class AndWebPageSetCombiner : public WebPageSetCombiner // the and combiner
{
    public:

    virtual WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);


};

class OrWebPageSetCombiner : public WebPageSetCombiner // the or combiner
{
    public:
    virtual WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);

    
};
class DiffWebPageSetCombiner : public WebPageSetCombiner
{
    public:
     virtual WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);

    
};

#endif