#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

// Use inheritance from std::vector (choose public/private) as appropriate
template <typename T>
class Stack : private std::vector<T>
{
public:
    Stack();
    ~Stack();
    bool empty() const;
    size_t size() const;
    void push(const T& item);
    void pop();  // throws std::underflow_error if empty
    const T& top() const; // throws std::underflow_error if empty
    // Add other members only if necessary
};

template <typename T>
Stack<T>::Stack(): 
std::vector<T>()
{

}

template <typename T>
Stack<T>::~Stack()
{
    std::vector<T>::clear();
}

template <typename T>
bool Stack<T>::empty() const
{
    return std::vector<T>::empty(); //gets if Stack is empty or not
}

template <typename T>
size_t Stack<T>::size() const{
    return std::vector<T>::size(); //gets size
}

template <typename T>
void Stack<T>::pop(){
    if(Stack<T>::empty()){
        throw std::underflow_error("The Stack is empty"); 
    }
    std::vector<T>::pop_back(); //pops the 'top' of stack
}

template <typename T>
void Stack<T>::push(const T& item){
    std::vector<T>::push_back(item); //pushes to 'top' of the stack
}
template <typename T>
const T& Stack<T>::top() const{ //get the top
    if(Stack<T>::empty()){
        throw std::underflow_error("The Stack is empty"); 
    }
    return std::vector<T>::back();
}


#endif