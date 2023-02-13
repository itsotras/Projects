#include "cmdhandler.h"
#include "util.h"
using namespace std;

// Complete
QuitHandler::QuitHandler()
{

}

// Complete
QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

// Complete
bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

// Complete
Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

// Complete
PrintHandler::PrintHandler()
{

}

// Complete
PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

// Complete
bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

// Complete
Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
        
    return HANDLER_OK;
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
}

// Add code for other handler class implementations below
//Incoming Handler
IncomingHandler::IncomingHandler()
{

}

IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";

}


Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    WebPage* w;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        w = eng->retrieve_page(name); //get the webpage pointer
        if(w == nullptr){
            throw std::invalid_argument("filename doesn't exist");
        }
       
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    std::set<WebPage*> mySet = w->incoming_links(); //get the list of webpages
    display_hits(mySet, ostr);//display hits

    return HANDLER_OK;
    
}

//Outgoing Handler
OutgoingHandler::OutgoingHandler()
{

}

OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";

}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    WebPage* w;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        w = eng->retrieve_page(name); //get the webpage pointer   
        if(w == nullptr){
            return HANDLER_ERROR;
        }
        
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    std::set<WebPage*> mySet = w->outgoing_links(); //get the list of webpages
    display_hits(mySet, ostr);
    return HANDLER_OK;
}

//AndHandler
AndHandler::AndHandler()
{
    combiner_ = new AndWebPageSetCombiner();
}

AndHandler::AndHandler(Handler* next)
    : Handler(next)
{

}

AndHandler::~AndHandler(){
    delete combiner_;
}

bool AndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";

}

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //we get the words
    vector<std::string> myStrings;
    string myString;
    while(instr>>myString){ //insert all the words into the myStrings vector
        
        myStrings.push_back(conv_to_lower(myString));
        

    }
    
    WebPageSet searchSet = eng->search(myStrings, combiner_); //get the set of all the webpages we want to display
    display_hits(searchSet, ostr); //display all of them

    return HANDLER_OK;
}

//OrHandler
OrHandler::OrHandler()
{
    combiner_ = new OrWebPageSetCombiner();
}

OrHandler::OrHandler(Handler* next)
    : Handler(next)
{

}

OrHandler::~OrHandler(){
    delete combiner_;
}

bool OrHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OR";

}

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //we get the words
    vector<std::string> myStrings;
    string myString;
    while(instr>>myString){
        
        myStrings.push_back(conv_to_lower(myString));

    }
    WebPageSet searchSet = eng->search(myStrings, combiner_); //get the set of all the webpages we want to display
    display_hits(searchSet, ostr);
    return HANDLER_OK;
}

//DiffHandler
DiffHandler::DiffHandler()
{
    combiner_ = new DiffWebPageSetCombiner();
}

DiffHandler::DiffHandler(Handler* next)
    : Handler(next)
{

}

DiffHandler::~DiffHandler(){
    delete combiner_;
}

bool DiffHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";

}

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //we get the words
    vector<std::string> myStrings;
    string myString;
    while(instr>>myString){ //push them back into the vector
        
        myStrings.push_back(conv_to_lower(myString));

    }
    WebPageSet searchSet = eng->search(myStrings, combiner_); //get the set of all the webpages we want to display
    display_hits(searchSet, ostr);
    return HANDLER_OK;
}

