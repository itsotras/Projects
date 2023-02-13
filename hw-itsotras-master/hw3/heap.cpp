#include "heap.h"
#include <iostream>

using namespace std;


int main(){
    Heap<int> h;

    h.push(0);
    h.push(23);
    h.push(56);
    h.push(100);
    h.push(-123);
    h.push(5);
    h.push(8);
    h.push(10);
    h.push(2);
    h.push(110);

    for(int i = 0; i<10; i++){
        cout<<h.top()<<endl;
        h.pop();
    }

    return 0;
    
}