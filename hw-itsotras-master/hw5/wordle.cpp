// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generateStrings(const std::string& input, const std::string& floating, const std::set<std::string>& dict, int open_spaces, int n, int letters_remain, 
std::string curr, std::set<std::string>& mySet, std::map<char, int> myMap);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    int open_spaces = 0;
    int n = in.size(); //get the number in our input string
    std::map<char,int> myMap;//map to store how many total characters of each we need
    int letters_remain = (int)(floating.size());//get the size of floating letters, how many letters we have left to use
    std::set<std::string> mySet;
    if(in == "" ){
        return mySet;
    }

    for(int i = 0; i<letters_remain; i++){
        if(myMap.find(floating[i]) == myMap.end()){//if not in my map of letters i need to add
            myMap.insert(make_pair(floating[i], 1));
        }
        else{//if it is in my map of letters
            std::map<char, int>::iterator it = myMap.find(floating[i]);
            it->second = it->second+1;//increment by one
        }
    }
    for(int i = 0; i<n; i++){
        if(in[i] == '-'){
            open_spaces++;//count the number of open spaces in the string
        }
    }
    
    
    
    if(open_spaces == 0){
        mySet.insert(in);
        return mySet;
    }
    generateStrings(in, floating, dict, open_spaces, n, letters_remain, "", mySet, myMap);
    return mySet;

}

// Define any helper functions here
void generateStrings(const std::string& input, const std::string &floating, const std::set<std::string>& dict, int open_spaces, int n, int letters_remain, 
std::string curr, std::set<std::string>& mySet, std::map<char, int> myMap){
    if(open_spaces < letters_remain){
        return;
    }
    if((int)curr.size() == n){//if we have made a string of size n
    
        if(dict.find(curr) != dict.end()){//if it is in the dictionary
            mySet.insert(curr);//insert it into my set
        }
        
    }
    else if(input[curr.size()] != '-'){//if the space we are about to add to is taken by a certain letter that is already in the input index

        generateStrings(input, floating, dict, open_spaces, n, letters_remain, curr+input[curr.size()], mySet, myMap);//put the same letter in the same place

    }
    else if(open_spaces == letters_remain && letters_remain != 0){//we should only use the letters in our floating string, yes?, if the space we are adding to isn't taken
        for(int i = 0; i < (int)(floating.size()); i++){

            char newChar = floating.at(i);//the character we are adding
            std::map<char, int> myCopyMap = myMap;
            std::map<char, int>::iterator it = myCopyMap.find(newChar); //get the iterator

            if(curr.find(newChar) == std::string::npos && it->second > 0 ){//if this character is not already in the string we're building
                    it->second = it->second-1;//decrement the amount of times it appears by one
                    generateStrings(input, floating, dict, open_spaces-1, n, letters_remain-1, curr+newChar, mySet, myCopyMap);//add the new character
            }
            else if(curr.find(newChar) != std::string::npos){//if this character is already in the current string we're building
                if(letters_remain > 0){//if we have some more letters to add and we have enough space
                    if(it->second > 0){ //and if we have to add this specific character once more
                        it->second = it->second-1;//decrement the amount of times it appears by one
                        generateStrings(input, floating, dict, open_spaces-1, n, letters_remain-1, curr+newChar, mySet, myCopyMap);//add the new character
                    }
                    else{//if we don't have to add it any more

                    }
                }
            else{//we don't have any more 'required' letters from floating, so we can add it again
                generateStrings(input, floating, dict, open_spaces-1, n, letters_remain, curr+newChar, mySet, myCopyMap);
                }
            }
        }
    }
    else{
        for(int i = 0; i<26; i++){//loop through all the letters
            char newChar = (char)('a'+i);//the character we are adding
            std::map<char, int> myCopyMap = myMap;
            std::map<char, int>::iterator it = myCopyMap.find(newChar);//find this newChar in my map

            if(floating.find(newChar) != std::string::npos){//if this character is in the floating string

                if(curr.find(newChar) == std::string::npos && it->second > 0 ){//if this character is not already in the string we're building
                    it->second = it->second-1;//decrement the amount of times it appears by one
                    generateStrings(input, floating, dict, open_spaces-1, n, letters_remain-1, curr+newChar, mySet, myCopyMap);//add the new character
                }

                else if(curr.find(newChar) != std::string::npos){//if this character is already in the current string we're building
                    if(letters_remain > 0){//if we have some more letters to add and we have enough space
                        if(it->second > 0){ //and if we have to add this specific character once more
                            it->second = it->second-1;//decrement the amount of times it appears by one
                            generateStrings(input, floating, dict, open_spaces-1, n, letters_remain-1, curr+newChar, mySet, myCopyMap);//add the new character
                        }
                        else{//if we don't have to add it any more

                        }
                    }
                    else{//we don't have any more 'required' letters from floating, so we can add it again
                        generateStrings(input, floating, dict, open_spaces-1, n, letters_remain, curr+newChar, mySet, myCopyMap);
                    }
                }

            }
            else{//if it is not in the floating string ie some random letter
                if(open_spaces > letters_remain || letters_remain == 0){//if we have enough space
                    generateStrings(input, floating, dict, open_spaces-1, n, letters_remain, curr+newChar, mySet, myCopyMap);
                }
                else{

                }
            }
            
        }
        
        
    }
}

