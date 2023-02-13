#include <iostream>
#include "labellist.h"
using namespace std;

int main()
{
    LabelList l1;
    LabelList::MsgToken one = l1.add("Hello");
    LabelList::MsgToken two = l1.add("Goodbye");
    LabelList::MsgToken three = l1.add("Sheesh");
    LabelList::MsgToken four = l1.add("Pee");
    LabelList::MsgToken five = l1.add("Poo");
    l1.print(std::cout, "all", ' ');
    
    //l1.remove(one);
    //l1.remove(two);
    //l1.remove(three);
    //l1.remove(four);
    //l1.remove(five);
    //l1.clear();

    l1.print(std::cout, "all", ' ');
    //l1.print(std::cout, "1", ' ');
    //l1.print(std::cout, "2", ' ');
    //l1.print(std::cout, "3", ' '); 
    //l1.unlabel(one, "1");
    
    //l1.print(std::cout, "1", ' ');
    //l1.remove(one);
    //l1.remove(two);
    //l1.remove(three);
    //l1.print(std::cout, "all", ' ');
    //LabelList::MsgToken r = l1.add("Goodbye");
    //l1.remove(r);
    //LabelList::MsgToken s =  l1.add("Goodbye");;
    //l1.print(std::cout, "all", ' ');
    //l1.label(s, "Bye");
    
    return 0;
}