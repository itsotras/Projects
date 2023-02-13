#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"
#include <iostream>

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

// To be completed
void MDParser::parse(std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // Complete your code below
    std::string myTerm = "";
    std::string link = "";

    char newChar = istr.get();

    PARSE_STATE_T myState = NORMALTEXT; //assume it is regular text first

    while(!istr.fail()){

        if(myState == NORMALTEXT){

            if(!isalnum(newChar)){//if this is a character that isn't a letter or number (a special character)
                if(newChar == '['){ // when we hit [ character
                    myState = LINKTEXT; // set myState to LINKTEXT
                }
                if(myTerm != ""){ // if myTerm is not empty
                    myTerm = conv_to_lower(myTerm); //change to standard
                    allSearchableTerms.insert(myTerm); //add it to allSearchableTerms set

                    myTerm = ""; //empty myTerm
                }
                
                
            }
            else{ //if character is not a special character
                myTerm += newChar;//add to myTerm
            }
        }
        else if(myState == LINKTEXT){
            if(!isalnum(newChar)){ //if this is a character that isn't a letter or number (a special character)

                if(newChar == ']'){ // when we hit ] character

                    myState = ISLINK; // set myState to ISLINK
                }
                if(myTerm != ""){ //if myTerm is not empty
                    myTerm = conv_to_lower(myTerm); //change to standard
                    allSearchableTerms.insert(myTerm); //add it to allSearchableTerms set

                    myTerm = ""; //empty myTerm
                }
            }
            else{ //if character is not a special character
                myTerm += newChar;//add to myTerm
            }
        }
        else if(myState == ISLINK){ //if this is a link(potentially)

            if(newChar != '('){ //set it to LINKURL
                if(newChar == '['){
                    myState = LINKTEXT;
                }
                else{   
                    myState = NORMALTEXT;
                }
            }
            else{ //if the character after is not (, continue reading as if we are not going to get a list
                myState = LINKURL;
            }
        }
        else if(myState == LINKURL){
            if(newChar == ')'){
                myState = NORMALTEXT; //change myState to NORMALTEXT
                if(link != ""){ //if link is not empty
                    link = conv_to_lower(link); //change to standard
                    allOutgoingLinks.insert(link); //add it to allOutgoingLinks set
                    link = ""; //empty link
                }
            }
            else{
                link += newChar; //add to link
            }
        }
        //myTerm += newChar;
        newChar = istr.get(); //get next character
    }

    if(myTerm != ""){ //if there is one last term we missed
        myTerm = conv_to_lower(myTerm); //change to standard
        allSearchableTerms.insert(myTerm); //add it to allSearchableTerms set
    }
    if(link != ""){ //if there is one last term we missed
        link = conv_to_lower(link); //change to standard
        allOutgoingLinks.insert(link); //add it to allSearchableTerms set
    }
}

// To be completed
std::string MDParser::display_text(std::istream& istr)
{
    std::string output;
    
    char c = istr.get();
    PARSE_STATE_T myState = NORMALTEXT;

    while(!istr.fail()){
        if(myState == NORMALTEXT){
            if(c == '['){ // when we hit [ character
                myState = LINKTEXT; // set myState to LINKTEXT
            }
            output+=c;
        }
        else if(myState == LINKTEXT){
            if(c == ']'){ // when we hit ] character

                myState = ISLINK; // set myState to ISLINK
            }
            
            output+=c;
        }
        else if(myState == ISLINK){
            if(c != '('){ //if the character after is not (, continue reading as if we are not going to get a list
                myState = NORMALTEXT;
                
            output+=c;
            }
            else{ //set myState to LINKURL
                myState = LINKURL;
            }
        }
        else if(myState == LINKURL){
            if(c == ')'){
                myState = NORMALTEXT; //change myState to NORMALTEXT
            }
        }
        c = istr.get();
    }
    return output;

}


