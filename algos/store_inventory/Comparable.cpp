#pragma once

/** @file SearchTree.cpp
 * UWB CSS 502 Data Structures and Algorithms
 * Program 1 - Improved Binary Search Tree
 * @author Chase Howard
 * @date 2021-01-10
 * Description: This contains the implemetnation of the Comparable class. Designed
 *      to be used with SearchTree.h
 */

#include "Comparable.h"

// -----------------------  MUTATORS    ---------------------------

 //   Parse string from input file to store into member storage
  //  pre none
  //  post Comparable now sotres comma delimted data from file input
void Comparable::parseString(string is) {
    istringstream ss(is);

    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        if (substr[0] == ' ')
            substr.erase(0, 1); // remove whitespace
        this->storage.push_back(substr);
    }
}

bool Comparable::decrement(int quantity) {
    if (quantity > count){
        this->count = 0;
        return false;
    }
    else 
        count -= quantity;
    return true;
}
// -----------------------  ACCESSORS    ---------------------------

//------------------------------------------------------------------------
// Overload operator <<
// Preconditions: None
// Postconditions: Ouptut contents of comparable to ostream
ostream& operator<<(ostream& os, const Comparable* myComp) {
    if (myComp != nullptr)
        myComp->print(os);
    return os;
}

//------------------------------------------------------------------------
// Overload operator <<
// Preconditions: None
// Postconditions: Ouptut contents of comparable to ostream
ostream& operator<<(ostream& os, const Comparable& myComp) {
    if (&myComp != nullptr)
        myComp.print(os);
    return os;
}

//------------------------------------------------------------------------
// execution of actions
// Preconditions: None
// Postconditions: Comparable has executed function as needed
void Comparable::init(string is) {
    parseString(is);
    setMembers();
}

////------------------------------------------------------------------------
//// pure virtual function for inherited classes to print contents to cout
//// Preconditions: None
//// Postconditions: Item has been 
//ostream& Comparable::print(ostream& os) const {
//    for (auto i = storage.begin(); i != storage.end(); ++i) {
//        if (i != storage.end() - 1)
//            os << *i << ", ";
//        else os << *i << endl;
//    }
//    return os;
//}
