#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
  /**
   * @brief Construct a new Heap object
   * 
   * @param m ary-ness of heap tree (default to 2)
   * @param c binary predicate function/functor that takes two items
   *          as an argument and returns a bool if the first argument has
   *          priority over the second.
   */
  Heap(int m=2, PComparator c = PComparator());

  /**
  * @brief Destroy the Heap object
  * 
  */
  ~Heap();

  /**
   * @brief Push an item to the heap
   * 
   * @param item item to heap
   */
  void push(const T& item);

  /**
   * @brief Returns the top (priority) item
   * 
   * @return T const& top priority item
   * @throw std::underflow_error if the heap is empty
   */
  T const & top() const;

  /**
   * @brief Remove the top priority item
   * 
   * @throw std::underflow_error if the heap is empty
   */
  void pop();

  /// returns true if the heap is empty

  /**
   * @brief Returns true if the heap is empty
   * 
   */
  bool empty() const;

private:
  /// Add whatever helper functions and data members you need below
  std::vector<T> myData;
  void heapify(int index);
  size_t size();
  int m_;
  PComparator c_;



};

// Add implementation of member functions here


template <typename T, typename PComparator>
Heap<T, PComparator>::Heap(int m, PComparator c):
m_(m), c_(c)
{
  
}

template <typename T, typename PComparator>
Heap<T, PComparator>::~Heap(){
  myData.clear();
}


// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
  // Here we use exceptions to handle the case of trying
  // to access the top element of an empty heap
  if(empty()){
    // ================================
    // throw the appropriate exception
    // ================================
    throw std::underflow_error("Heap is empty");

  }
  // If we get here we know the heap has at least 1 item
  // Add code to return the top element
  return myData[0];


}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
  if(empty()){
    // ================================
    // throw the appropriate exception
    // ================================
    
  throw std::underflow_error("Heap is empty");


  }
  std::swap(myData[0], myData[(int)size()-1]);
  myData.pop_back();
  int start = 0;
  heapify(start);

}
template <typename T, typename PComparator>
void Heap<T,PComparator>::push(const T& item){
  myData.push_back(item);
  int index = (int)size()-1;
  while(index != 0){
    int parent = (index-1)/m_;

    T& current = myData[index];
    T& parent_element = myData[parent];
    if(c_(parent_element, current)){//if parent is better than current
      break;
    }
    std::swap(current, parent_element);
    index = parent;

  }

}

template <typename T, typename PComparator>
bool Heap<T,PComparator>::empty() const{
  return myData.empty(); //if myData is empty, return true
}

template <typename T, typename PComparator>
void Heap<T,PComparator>::heapify(int index){
  if(m_*index+1 > (int)size()-1){ //if my index is greater than the size of the heap, return
    return;
  }
  int bestChild = m_*index+1; //get index of the left child(our best for now)
  int nextChild = bestChild+1;
  while(nextChild <= m_*index+m_ && nextChild <= (int)size()-1){
    if(c_(myData[nextChild], myData[bestChild])){ //if nextChild is better than bestChild
      bestChild = nextChild;
    }
    nextChild++;
  }//get the best child

  if(c_(myData[bestChild], myData[index])){
    std::swap(myData[index], myData[bestChild]);
  }
  heapify(bestChild); //repeat until we hit the end of the height of the list
}

template <typename T, typename PComparator>
size_t Heap<T,PComparator>::size(){
  return myData.size();
}




#endif

