////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Author:           Matthew Lingenfelter                           //
//   Creation Date:    November 1, 2020                               //
//   Purpose:                                                         //
//      This program uses an array template class to handle partially //
//      filled arrays.                                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>
#include <cassert>

using namespace std;

template <class dataType> 
class Array {

public:
  Array(int size= 3);                // default constructor
  Array(const Array&);               // copy constructor
  Array(const dataType[], int);      // construct from an dataType array
  ~Array();                          // destructor
  
  int getCapacity() const { return capacity; }
  int getNumUsed() const { return numUsed; }
  bool atCapacity() const { return numUsed == capacity; } // test if full

  const Array<dataType> &operator=(const Array &); // assignment

  Array<dataType> &operator+=(int);  // increase the capacity
  Array<dataType> &operator--();     // predecrement - remove first element
  Array<dataType> operator--(int);   // postdecrement - remove first element

  bool operator<=(const Array &) const;  // compare based on capacity 

  dataType &operator[](int);             // l-value subscript operator
  const dataType &operator[](int) const; // r-value subscript operator

  const dataType *operator&() const;     // address operator

private:
  int capacity;              // capacity of the array
  int numUsed;               // number of elements in use
  dataType *ptr;             // pointer to first element of array
};

// Default constructor for class Array 
template <class dataType> 
Array<dataType>::Array(int size) {
  capacity= size;
  numUsed= 0;
  
  ptr = new dataType[capacity]; // create space for array
  assert( ptr != 0 );    // terminate if memory not allocated

  // initialize to default value
  for (int i = 0; i < capacity; ++i) ptr[i]= dataType();
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Constructor from dataType array                   //
//   Description:   Constructs a dataType Array from a standard array //
//   Member Type:   Constructor                                       //
//   Parameters:    const dataType A[] - an array getting passed in   //
//                  int num2copy - number of elements to copy into    //
//                         the new array                              //
//   Return Value:  none                                              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// constructor initializes from a standard array
// designate the number of elements to copy 
template <class dataType>
Array<dataType>::Array(const dataType A[], int num2copy) {
  //sets the capacity of the array
  if(num2copy < 3) capacity= 3;
  else capacity= num2copy;

  //initializes the numbers used to 0
  numUsed= 0;

  ptr = new dataType[ capacity ]; // create space for array
  assert( ptr != 0 );           // terminate if memory not allocated
  for (int i = 0; i < capacity; i++) {
    if(i < num2copy) {
      ptr[ i ] = A[ i ];
      numUsed++; // counts the number of elements in use
    }
    // if there are "empty" spots in the array, they are set to the default value
    else ptr[ i ]= dataType();
  }
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Copy Constructor                                  //
//   Description:   Creates a copy of an excisting Array              //
//   Member Type:   Constructor                                       //
//   Parameters:    const Array &A - the Array to be copies           //
//   Return Value:  none                                              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Copy constructor for class Array
// must receive a reference to prevent infinite recursion
template <class dataType> 
Array<dataType>::Array(const Array &A) {  
  // sets the new capacity to the one being copied
  capacity= A.capacity;

  //initializes the numbers used to 0
  numUsed= 0;
  
  ptr = new dataType[capacity]; // create space for array
  assert( ptr != 0 );    // terminate if memory not allocated

  for ( int i = 0; i < capacity; i++ ) {
     ptr[ i ] = A.ptr[ i ];  // copies all elements into new object
     numUsed++; // counts the number of elements used
  }
}

// Destructor for class Array
template <class dataType> 
Array<dataType>::~Array() {
  delete [] ptr;            // reclaim space for array
}

// Overloaded assignment operator
template <class dataType> 
const Array<dataType> &Array<dataType>::operator=( const Array &right ) {

  if ( right.ptr != ptr ) {  // check for self-assignment
    // for arrays of different sizes, deallocate original
    // left side array, then allocate new left side array.
    if ( capacity != right.capacity ) {
       delete [] ptr;                // reclaim space
       capacity=right.capacity;
       numUsed = right.numUsed;// resize this object
       ptr = new dataType[capacity]; // create space for array copy
       assert( ptr != 0 );           // terminate if not allocated
    }

    for ( int i = 0; i < capacity; i++ )
      ptr[ i ] = right.ptr[ i ];  // copy array into object
  }
  return *this;   // enables x = y = z;
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   non-const Subscript Operator                      //
//   Description:   Overloaded subscript operator for non-const Arrays//
//   Member Type:   Inspector                                         //
//   Parameters:    int subscipt - the number location of the element //
//                        the user wanted                             //
//   Return Value:  ptr[subscript] - the element in the location given//
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Overloaded subscript operator for non-const Arrays
// reference return creates an lvalue
template <class dataType> 
dataType &Array<dataType>::operator[](int subscript) {
  if( subscript < 0 || subscript >= capacity ) {
    cout << "non-const subscript error..." << endl;
  }
  else if( subscript >= numUsed) {
    numUsed = subscript + 1;
  }
  return ptr[ subscript ]; // reference return
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   const subscript operator                          //
//   Description:   Overloaded subscript operator for const Arrays    //
//   Member Type:   Inspector                                         //
//   Parameters:    int subscript - number location of the element    //
//                         the user wants to access                   //
//   Return Value:  ptr[subcript] - the element in the location given //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Overloaded subscript operator for const Arrays
// const reference return creates an rvalue
// subscript must be in range of "used" elements
template <class dataType> 
const dataType &Array<dataType>::operator[](int subscript) const {
  if( subscript < 0 || subscript >= capacity ) {
     cout << "const subscript error..." << endl;
  }

  return ptr[ subscript ]; // const reference return
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Address(&) Operator                               //
//   Description:   Gets and returns the memory location for the      //
//                     pointer of the first element                   //
//   Member Type:   Inspector                                         //
//   Parameters:    None                                              //
//   Return Value:  pp - the location of the pointer                  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// dereference operator (return first array value)
template <class dataType> 
const dataType *Array<dataType>::operator&() const {
  //gets and returns the memory location of the pointer for the first element
  int *pp = &ptr[0];
  return (pp); 
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Plus Equals(+=) Operator                          //
//   Description:   Increased the capacity of the array               //
//   Member Type:   Modifier                                          //
//   Parameters:    ine incr - the amount the capacity will be        //
//                       increased                                    //
//   Return Value:  *this - returns itself to enable cascading        //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Increase the capacity of the array
template <class dataType> 
Array<dataType> &Array<dataType>::operator+=(int incr) {
  // copy everything to a temperary array with increased capacity
  int tempCapacity= capacity + incr;
  dataType *tempPtr;
  tempPtr = new dataType[tempCapacity]; // create space for array
  assert( tempPtr != 0 );    // terminate if memory not allocated

  for ( int i = 0; i < tempCapacity; i++ ) {
    tempPtr[ i ] = ptr[ i ];  // copy init into object
  }
  
  // change prt and capacity to the new temp
  capacity = tempCapacity;
  ptr = tempPtr;
  
  return(*this);
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Pre-decrement(--) operator                        //
//   Description:   Removes the first element of the array            //
//   Member Type:   Modifier                                          //
//   Parameters:    None                                              //
//   Return Value:  *this - returns itself to enable cascading        //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// predecrement - remove first element
template <class dataType> 
Array<dataType> &Array<dataType>::operator--() {
  dataType *tempPtr;
  tempPtr = new dataType[capacity]; // create space for array
  assert( tempPtr != 0 );    // terminate if memory not allocated

  for ( int i = 0; i < capacity; i++ ) {
    tempPtr[ i ] = ptr[ i + 1];  // copy init into object
  }
  ptr = tempPtr; // changes the ptr to the tempPtr
  numUsed--; // subtracts one from the numbers of elements used

  return(*this);
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Pre-decrement(--) operator                        //
//   Description:   Removes the first element of the array            //
//   Member Type:   Modifier                                          //
//   Parameters:    int                                               //
//   Return Value:  temp - a copy of the array passed in              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

//postincrement - add default element to the front of array
template <class dataType> 
Array<dataType> Array<dataType>::operator--(int) {
  Array<int> temp(ptr, numUsed);

  dataType *tempPtr;
  tempPtr = new dataType[ capacity ]; // create space for array
  assert( tempPtr != 0 );    // terminate if memory not allocated

  for ( int i = 0; i < capacity; i++ ) {
    tempPtr[ i ] = ptr[ i + 1 ];  // copy init into object
  }
  ptr = tempPtr; // changes the ptr to the tempPtr
  numUsed--; // subtracts one from the numbers of elements used
  
  return(temp);
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Comparison Operator                               //
//   Description:   Compares the capacities of two arrays             //
//   Member Type:   Inspector                                         //
//   Parameters:    const Array &right - the second array being       //
//                        compared                                    //
//   Return Value:  true - if the second array's capacity it greater  //
//                  false - if the first array's capacity is greater  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Determine which is greater (just compare the number used)
template <class dataType>
bool Array<dataType>::operator<=(const Array &right) const  {
  if(capacity <= right.getCapacity())
    return true;
  else
    return false;
}

// Overloaded output operator for class Array 
template <class dataType> 
ostream &operator<<(ostream &output, const Array<dataType> &A) {
  int i;
  output << "[ ";
  for ( i = 0; i < A.getNumUsed(); i++ ) {
    if (i != 0) { output << ", "; }
    output << A[ i ]; 
  }
  output << " ]" << endl;;

  return output;   // enables cout << x << y;
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//   Method Name:   Input Stream(>>) Operator                         //
//   Description:   Adds an element at the end of the array           //
//   Member Type:   Modifier                                          //
//   Parameters:    istream &input - the input stream                 //
//                  Array<dataType> &A - the array getting modified   //
//   Return Value:  input - to enable cascading                       //
//                                                                    //
////////////////////////////////////////////////////////////////////////

// Overloaded input operator for class Array 
template <class dataType> 
istream &operator>>(istream &input, Array<dataType> &A) {
  int intemp = 0;

  //gets the input value from user
  input >> intemp;
  
  //checks if the Array is at capacity
  if(A.atCapacity() && (A.getNumUsed() == 3)) {
    //if it is at capacity it increases the capacity by 1
    A += 1;
  }

  //loops through the array until the first empty spot and assigns the
  //user value to that spot
  for(int i = 0; i < A.getCapacity(); i++) {
    if(A[ i ] == 0 ) {
      A[ i ] = intemp;
      //once it assigns the value, breaks from the loop to not continuously
      //assign the remaining spots
      break;
    }
  }
  
  return input;   // enables cin >> x >> y;
}

#endif
