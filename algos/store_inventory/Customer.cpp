#pragma once
//--------------------------------------------------------------------
// Customer.cpp
// Implemetnation of Customer class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Customer.h"

// Construct Customer object
Comparable* Customer::create() {
    //cout << "Creating Customer" << endl;
    Comparable* myObj = new Customer();
    return myObj;
}

//-------------------------------------------------------------------------------------
//  Assignment operator 
//  pre: none
//  post: assign customer object to calling object, deep copy;
Comparable& Customer::operator=(Comparable& rhs) {
    Customer* castComp = static_cast<Customer*>(&rhs);
    vector<string> rhStorage = castComp->getStorage();

    if (this != nullptr && castComp != nullptr) {
        if (!(*this == *castComp)) {
            this->remove();
            Comparable::storage = rhStorage;
            setMembers();
        }
    }
    return rhs;
}

//-------------------------------------------------------------------------------------
//  Equality operator 
//  pre: none
//  post: assign customer object to calling object, deep copy;
bool Customer::operator==(Comparable& rhs) const {
    Customer* castComp = static_cast<Customer*>(&rhs);
    if (castComp == nullptr) return false;

    return id == castComp->id;
}

//-------------------------------------------------------------------------------------
//  Comparison overload
//  pre: none
//  post: used for ordering in ItemBST;
bool Customer::operator<(Comparable& rhs) const {
    Customer* castComp = static_cast<Customer*>(&rhs);
    if (castComp == nullptr) return false;

    return this->name < castComp->name;
}

//-------------------------------------------------------------------------------------
//  add transaction 
//  pre: none
//  post: add a transaction to the transaction history;
bool Customer::add(string action) {
    history.push_back(action);
    return true;
}

void Customer::setMembers() {
    id = Comparable::storage[0];
    name = Comparable::storage[1];
}

// Disable this function for customer class
void updateCount() {};

//-------------------------------------------------------------------------------------
//  Print contents of customer and return in string
//  pre: none
//  post: string returned with contents of customer history;
ostream& Customer::print(ostream& os) const {
    cout << setfill('0') << setw(3) << getID();
    cout << ", " << name << endl;
    for (auto i = history.begin(); i != history.end(); ++i)
        os << "\t" << *i << endl;
    return os;
}