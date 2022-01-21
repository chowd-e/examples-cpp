#pragma once
//--------------------------------------------------------------------
// Sell.cpp
// Implementation of Sell class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Sell.h"
#include "Store.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New Sell object created as implemented by 
//      derived classes
Comparable* Sell::create() {
    //cout << "creating Sell" << endl;
    Comparable* myObj = new Sell();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a Sell Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling Sell is less than RHS
bool Sell::operator<(Comparable& rhs) const {
    Sell* castBuy = static_cast<Sell*>(&rhs);
    if (castBuy == nullptr) return false;

    return (this->transactionInfo == castBuy->transactionInfo);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& Sell::operator=(Comparable& rhs) {
    Sell* castComparable = static_cast<Sell*>(&rhs);
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
bool Sell::operator==(Comparable& rhs) const {
    Sell* castBuy = static_cast<Sell*>(&rhs);
    if (castBuy == nullptr) return false;

    return Transaction::customerID == castBuy->customerID && \
        Transaction::transactionInfo == castBuy->transactionInfo;
}

// Initialize members
void Sell::setMembers() {
    Comparable::key = tolower((Comparable::storage[0])[0]);
    Transaction::customerID = (Comparable::storage[1]);
    for (unsigned int i = 2; i < storage.size() - 1; i++)
        Transaction::transactionInfo += Comparable::storage[i] + ", ";

    Transaction::transactionInfo += storage[storage.size() - 1]; // final w/out delim
}

void Sell::execute(Store* myStore) {
    myStore->sell(this);
}