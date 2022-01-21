#pragma once
/** @file SearchTree.cpp
 * UWB CSS 502 Data Structures and Algorithms
 * Program 1 - Improved Binary Search Tree
 * @author Chase Howard
 * @date 2021-01-10
 * Description: This contains the header file of the Comparable class. Designed
 *      to be used with SearchTree.h
 */

 /*
     Structure overview:
         This class stores a single character as a member variable and only contains methods to
         overload the comparison operators as well as input and output data form the class. All
         comparisons are equivalent to the ASCII comparisons

     Input:
         Characters can be sreamed into the Comparable object using an overloaded input stream
         operator. Whitespace is ignored from input and discarded from the istream.

     Output:
         Character stored in member variable "ch" can be output using an overloaded output stream
         operator.
  */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Comparable {
private:
    
    //   Parse string from input file to store into member storage
     //  pre none
     //  post Comparable now sotres comma delimted data from file input
    void parseString(string is);

protected: 
    char key;
    int count;
    vector<string> storage; 

    // return vector containing information relating to object
    vector<string> getStorage() { return storage; };

    //------------------------------------------------------------------------
    // set count
    // Preconditions: quantity is a non-negative number
    // Postconditions: Item has been dectemented by indicated quantity
    //      return -1 on unsuccessful decrement
    void setCount(int newCount) { count = newCount; };

    //------------------------------------------------------------------------
    // pure virtual function for decrementing derived objects by int
    // Preconditions: quantity is a non-negative number
    // Postconditions: Item has been dectemented by indicated quantity
    //      return -1 on unsuccessful decrement
    void updateCount() { Comparable::storage[1] = to_string(count); };

public:

    Comparable() : key('~'), count(0) { //cout << "Creating Comparable" << endl;
    };

    ~Comparable() { storage.clear();  };

    /**   Overload equality operator "==" to compare two Comparables contents (char)
      * @param Comparables& right hand side comparison of character
      * @pre none
      * @post Comparables have been compared and bool result returned indicating equality
      * @return bool indication of Comparables equality
      * Functions called:  none  */
    virtual bool operator==(Comparable& rhs) const = 0;

    /**   Overload equality operator "==" to compare two Comparables contents (char)
      * @param Comparables& right hand side comparison of character
      * @pre none
      * @post Comparables have been compared and bool result returned indicating equality
      * @return bool indication of Comparables equality
      * Functions called:  none  */
    virtual Comparable& operator=(Comparable& rhs) = 0;

    //   Overload comparison operator "<" to compare two Comparables chars
    //  param Comparables& right hand side comparison of character
    //  pre none
    //  post Comparables have been compared and bool result returned for indicating if lhs ASCII < rhs
    //  return bool indication of "character" comparison between two Comparables
    //  unctions called:  none*/
    virtual bool operator<(Comparable& rhs) const = 0;

    //------------------------------------------------------------------------
    // pure virtual function for inherited Constructors
    // Preconditions: None
    // Postconditions: New Item object created as implemented by 
    //      derived classes based on input string from read files, returns ptr 
    //      to new dynamically allocated Item
    virtual Comparable* create() = 0;

    //------------------------------------------------------------------------
    // pure virtual function for inherited destructors
    // Preconditions: None
    // Postconditions: Item has been cleared along with any dynamic 
    //          memory associated
    virtual void remove() = 0;

    //------------------------------------------------------------------------
    // Overload operator <<
    // Preconditions: None
    // Postconditions: Ouptut contents of comparable to ostream
    friend ostream& operator<<(ostream& os, const Comparable* myComp);

    //------------------------------------------------------------------------
    // Overload operator <<
    // Preconditions: None
    // Postconditions: Ouptut contents of comparable to ostream
    friend ostream& operator<<(ostream& os, const Comparable& myComp);

    //------------------------------------------------------------------------
    // function for printing contents
    // Preconditions: None
    // Postconditions: output contents of object to ostream
    virtual ostream& print(ostream& os) const = 0;

    //------------------------------------------------------------------------
    // return key character
    // Preconditions: quantity is a non-negative number
    // Postconditions: Item has been dectemented by indicated quantity
    //      return -1 on unsuccessful decrement
    int getKey() const { return key; };

    //------------------------------------------------------------------------
    // function returning count of Comparable
    // Preconditions: none
    // Postconditions: Item has been dectemented by indicated quantity
    //      return -1 on unsuccessful decrement
    int getCount() const { return count; };

    //------------------------------------------------------------------------
    // execution of actions
    // Preconditions: None
    // Postconditions: Comparable has executed function as needed
    void init(string is);

    //------------------------------------------------------------------------
    // set members
    // Preconditions: storage has been filled 
    // Postconditions: member variables assigned accordingly
    virtual void setMembers() = 0;

    //------------------------------------------------------------------------
    // pure virtual function for incrementing derived objects by int
    // Preconditions: quantity is a non-negative number
    // Postconditions: Item has been incremented by indicated quantity
    //      return -1 on unsuccessful increment
    void increment(int quantity = 1) { count += quantity; updateCount(); };

    //------------------------------------------------------------------------
    // pure virtual function for decrementing derived objects by int
    // Preconditions: quantity is a non-negative number
    // Postconditions: Item has been dectemented by indicated quantity
    //      return -1 on unsuccessful decrement
    bool decrement(int quantity = 1);

    
};