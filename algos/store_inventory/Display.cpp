#pragma once
//--------------------------------------------------------------------
// Display.cpp
// Implementation of Display class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Display.h"
#include "Store.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New Display object created as implemented by 
//      derived classes
Comparable* Display::create() {
    //cout << "creating Display" << endl;
    Comparable* myObj = new Display();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a Display Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling Display is less than RHS
bool Display::operator<(Comparable& rhs) const {
    Display* castBuy = static_cast<Display*>(&rhs);
    if (castBuy == nullptr) return false;

    return (this->key == castBuy->key);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& Display::operator=(Comparable& rhs) {
    Display* castComparable = static_cast<Display*>(&rhs);
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
bool Display::operator==(Comparable& rhs) const {
    Display* castBuy = static_cast<Display*>(&rhs);
    if (castBuy == nullptr) return false;

    return Transaction::customerID == castBuy->customerID && \
        Transaction::transactionInfo == castBuy->transactionInfo;
}

// Initialize members
void Display::setMembers() {
    Comparable::key = tolower((Comparable::storage[0])[0]);
}

void Display::execute(Store* myStore) {
    myStore->display();
}