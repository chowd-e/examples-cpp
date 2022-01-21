#pragma once
//--------------------------------------------------------------------
// Buy.cpp
// Implementation of Buy class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Buy.h"
#include "Store.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New Buy object created as implemented by 
//      derived classes
Comparable* Buy::create() {
    //cout << "creating Buy" << endl;
    Comparable* myObj = new Buy();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a Buy Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling Buy is less than RHS
bool Buy::operator<(Comparable& rhs) const {
    Buy* castBuy = static_cast<Buy*>(&rhs);
    if (castBuy == nullptr) return false;

    return (this->transactionInfo == castBuy->transactionInfo);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& Buy::operator=(Comparable& rhs) {
    Buy* castComparable = static_cast<Buy*>(&rhs);
    if (castComparable != nullptr) {
        this->storage = castComparable->storage;
        setMembers();
        return rhs;
    }
    return rhs;
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
bool Buy::operator==(Comparable& rhs) const {
    Buy* castBuy = static_cast<Buy*>(&rhs);
    if (castBuy == nullptr) return false;

    return Transaction::customerID == castBuy->customerID && \
        Transaction::transactionInfo == castBuy->transactionInfo;
}

//------------------------------------------------------------------------
// set members
// Preconditions: storage has been filled 
// Postconditions: member variables assigned accordingly
void Buy::setMembers() {
    Comparable::key = tolower((Comparable::storage[0])[0]);
    Transaction::customerID = (Comparable::storage[1]);
    for (unsigned int i = 2; i < storage.size() - 1; i++)
        Transaction::transactionInfo += Comparable::storage[i] + ", ";

    Transaction::transactionInfo += storage[storage.size() - 1]; // final w/out delim
}

//------------------------------------------------------------------------
// execute
// Preconditions: storage has been filled and members set
// Postconditions: executes desired function in Store
void Buy::execute(Store* myStore) {
    myStore->buy(this);
}