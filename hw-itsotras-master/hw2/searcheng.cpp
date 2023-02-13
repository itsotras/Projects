#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"

using namespace std;

// Helper function that will extract the extension of a filename
std::string extract_extension(const std::string& filename);

std::string extract_extension(const std::string& filename)
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}


// To be updated as needed 
SearchEng::SearchEng()
{

}

// To be completed
SearchEng::~SearchEng()
{
    for(std::map<std::string, PageParser*>::iterator it = parsers_.begin(); it!= parsers_.end(); ++it){//deallocate parsers
        delete it->second;
    }
    for(std::map<std::string, WebPage*>::iterator it = file_WebPage.begin(); it!= file_WebPage.end(); ++it){//deallocate the webpages
        delete it->second;
    }
}

// Complete
void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    if (parsers_.find(extension) != parsers_.end())
    {
        throw std::invalid_argument("parser for provided extension already exists");
    }
    parsers_.insert(make_pair(extension, parser));
}

// Complete
void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

// To be completed
void SearchEng::read_page(const string& filename)
{

    string ext = extract_extension(filename);
    PageParser* myParser; //get a parser for this extension

    if(parsers_.find(ext) == parsers_.end()){//if there is no extension for this file
        throw std::logic_error("parser for provided extension doesn't exist");
    }
    myParser = parsers_[ext];//get a parser
    ifstream reader (filename);
    if(reader.fail()){ //if invalid file
        throw std::invalid_argument("filename doesn't exist");
    }
    
    myParser->parse(reader, allMyTerms, allOutgoingLinks);
    reader.close();
    //now we make the webpage item
    WebPage* w = nullptr;
    if(file_WebPage.find(filename) == file_WebPage.end()){ //if webpage doesn't exist
        w = new WebPage(filename); //make a new Webpage
        file_WebPage.insert(make_pair(filename, w)); //adds it to file_WebPage
    }
    else{
        w = file_WebPage[filename];
        //w = (file_WebPage.find(filename)->second); //sets it to the webpage in certain places
    }

    StringSet newTerms = allMyTerms;
    w->all_terms(newTerms); //sets all the terms to this object's all_terms
    std::map<std::string, WebPageSet> bruv = myWebPages_;

    for(StringSet::iterator it = allMyTerms.begin(); it != allMyTerms.end(); ++it){ //go through all the terms in allMyTerms
        if(bruv.find(*it) == bruv.end()){//if not in the map keys
            WebPageSet newSet;
            newSet.insert(w);
            bruv.insert(make_pair(*it, newSet));
        }
        else{ //in map keys
            bruv.find(*it)->second.insert(w); //insert it in
        }
    }
    myWebPages_ = bruv;
    //now we need to set the webpages outgoinglinks
    for(StringSet::iterator it = allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){
        if(file_WebPage.find(*it) != file_WebPage.end()){//check if this webpage already exists
            w->add_outgoing_link(file_WebPage[*it]); //add it to my list of outgoing
            file_WebPage[*it]->add_incoming_link(w); //ad me to its list of incoming
        }
        else{ //if it doesn't
            WebPage* newWebPage = new WebPage(*it);
            file_WebPage.insert(make_pair(*it, newWebPage));//add it to file_WebPage
            w->add_outgoing_link(file_WebPage[*it]);
            file_WebPage[*it]->add_incoming_link(w);

        }
    } 
    


}

// To be completed
WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    /*std::map<std::string, WebPageSet> bruv = myWebPages_;

    for(std::map<std::string, WebPageSet>::iterator it = bruv.begin(); it != bruv.end(); ++it){ //loop through the map
        WebPageSet mySet = it->second;
        for(WebPageSet::iterator jt  = mySet.begin(); jt != mySet.end(); jt++){ //loop through the set
            if((*jt)->filename() == page_name){ //if there is a webPage that has this specific page_name, return it
                return *jt;
            }

        }
        
    }*/
    if(file_WebPage.find(page_name) != file_WebPage.end()){ //if in file_Webpage
        return file_WebPage.find(page_name)->second; //return it's pointer
    }
    return NULL; //return null if it doesn't exist
}

// To be completed
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    string ext = extract_extension(page_name);

    if(parsers_.find(ext) == parsers_.end()){//if there is no extension for this file
        throw std::logic_error("parser for provided extension doesn't exist");
    }
    ifstream ifile(page_name);
    PageParser* myParser;
    myParser = parsers_.find(ext)->second;
    if(ifile.fail()){
        throw std::invalid_argument("filename doesn't exist");
    }
    string myString = myParser->display_text(ifile);
    ostr<<myString;


}

// To be completed
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    
    //get the set from my map of <strings, set of Webpages>
    //use the combiner to make the new set between a set
    std::vector<std::string> myTerms = terms;
    std::map<std::string, WebPageSet> daPages = myWebPages_;
    std::map<std::string, WebPageSet>::iterator pageIterator;
    WebPageSet newSet;

    for(std::vector<std::string>::iterator finder = myTerms.begin(); finder!=myTerms.end(); ++finder){ //go through terms vector
        pageIterator = daPages.find(*finder);

        if(pageIterator != daPages.end()){ //if currently inside the map as a key
            if((bool)newSet.empty()){ //if my newSet is empty, set the first set that exists as newSet
                newSet = pageIterator->second;
            }
            else{
                newSet = combiner->combine(newSet, pageIterator->second); //otherwise, do the specific combine algorithm to the set
            }

        }



    }
    return newSet;
}

// Add private helper function implementations here

