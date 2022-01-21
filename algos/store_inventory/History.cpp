#pragma once
//--------------------------------------------------------------------
// History.cpp
// Implementation of History class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "History.h"
#include "Store.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New History object created as implemented by 
//      derived classes
Comparable* History::create() {
    //cout << "creating History" << endl;
    Comparable* myObj = new History();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a History Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling History is less than RHS
bool History::operator<(Comparable& rhs) const {
    History* castBuy = static_cast<History*>(&rhs);
    if (castBuy == nullptr) return false;

    return (this->key == castBuy->key);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& History::operator=(Comparable& rhs) {
    History* castComparable = static_cast<History*>(&rhs);
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
bool History::operator==(Comparable& rhs) const {
    History* castBuy = static_cast<History*>(&rhs);
    if (castBuy == nullptr) return false;

    return Transaction::customerID == castBuy->customerID && \
        Transaction::transactionInfo == castBuy->transactionInfo;
}

// Initialize members
void History::setMembers() {
    Comparable::key = tolower((Comparable::storage[0])[0]);
}

void History::execute(Store* myStore) {
    myStore->history();
}