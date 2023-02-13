#include "stack.h"
#include <iostream>

using namespace std;

int main(){

    Stack<char> s;
    for(int i = 0; i < 10; i++){
        s.push((char) i+65 );
    }
    //cout<<s.size()<<endl;
    while(!s.empty()){
        cout<<s.top()<<endl;
        s.pop();
    }

    return 0;
}