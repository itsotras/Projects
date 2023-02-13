#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        int len = k.length();
        int remainder = len % 6;//see if there are any strings without length 6
        int times = len/6;
        int j = 0;
        if(remainder == 0){
            j = 0;

        }
        else{
            j = 1;
        }
        std::string myStrings[5];
        for(int i = 0; i< 5; i++){
            myStrings[i] = "a";//initialize to have a value of 0 in ascii/our version
        }
        if(j == 1){//we have a remainder
            for(int i = 4; i >= 4-times+1; i--){
                myStrings[i] = k.substr(len-6-6*(4-i), 6);
            }
            //insert the strings in reverse order
            myStrings[4-times] = k.substr(0, remainder);//set the next open one to the remainder string

        }
        else{//we don't have a remainder

            for(int i = 4; i >= 4-times+1; i--){
                myStrings[i] = k.substr(len-6-6*(4-i), 6);
            }//insert the strings in reverse order
        }
        HASH_INDEX_T total = 0;
        unsigned long long array[5];
        for(int i = 0; i<5; i++){
            
            array[i] = getTotal(myStrings[i]);
            std::cout<<array[i]<<std::endl;
        }
        for(int i = 0; i<5; i++){
           total += array[i] * rValues[i];//get the corresponding values
        }



        return total;


    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if((int)letter >= 65 && (int)letter <= 90){
            letter = letter+32;
        }
        if((int)letter >= 97){
            return (HASH_INDEX_T)(letter-97);
        }
        else{
            return (HASH_INDEX_T)(letter-22);
        }

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }

    HASH_INDEX_T getTotal(std::string myString)const{
        int len = myString.length()-1;
         
        HASH_INDEX_T total = 0;
        HASH_INDEX_T prev_total = 0;
         
        for(int i = 0; i <=len; i++){
            prev_total = total;
            char newChar = myString[i];
            HASH_INDEX_T newValue = letterDigitToNumber(newChar);
            total = prev_total * 36 + newValue;

        }

        return total;
    }

};

#endif
