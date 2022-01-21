#pragma once
//--------------------------------------------------------------------
// CustomerCMD.cpp
// Implementation of CustomerCMD class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "CustomerCMD.h"
#include "Store.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New CustomerCMD object created as implemented by 
//      derived classes
Comparable* CustomerCMD::create() {
    //cout << "creating CustomerCMD" << endl;
    Comparable* myObj = new CustomerCMD();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a CustomerCMD Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling CustomerCMD is less than RHS
bool CustomerCMD::operator<(Comparable& rhs) const {
    CustomerCMD* castBuy = static_cast<CustomerCMD*>(&rhs);
    if (castBuy == nullptr) return false;

    return (this->key == castBuy->key);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& CustomerCMD::operator=(Comparable& rhs) {
    CustomerCMD* castComparable = static_cast<CustomerCMD*>(&rhs);
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
bool CustomerCMD::operator==(Comparable& rhs) const {
    CustomerCMD* castBuy = static_cast<CustomerCMD*>(&rhs);
    if (castBuy == nullptr) return false;

    return Transaction::customerID == castBuy->customerID && \
        Transaction::transactionInfo == castBuy->transactionInfo;
}

//------------------------------------------------------------------------
// set members
// Preconditions: storage has been filled 
// Postconditions: member variables assigned accordingly
void CustomerCMD::setMembers() {
    Comparable::key = tolower((Comparable::storage[0])[0]);
    Transaction::customerID = (Comparable::storage[1]);
}

//------------------------------------------------------------------------
 // execute
 // Preconditions: Comparable has been initialized
 // Postconditions: call to function in Store to perform
void CustomerCMD::execute(Store* myStore) {
    myStore->displayCustomer(getID());
}