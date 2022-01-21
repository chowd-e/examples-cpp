#pragma once
//--------------------------------------------------------------------
// Coin.cpp
// Implementation of Coin class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Coin.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New Coin object created as implemented by 
//      derived classes
Comparable* Coin::create() {
    //cout << "creating Coin" << endl;
    Comparable* myObj = new Coin();
    return myObj;
}

//------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a Coin Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling Coin is less than RHS
bool Coin::operator<(Comparable& rhs) const {
    Coin* castComp = static_cast<Coin*>(&rhs);
    if (castComp == nullptr) return false;

    if (this->type == castComp->type)
        if (this->year == castComp->year)
            return (this->grade < castComp->grade);
        else return (this->year < castComp->year);
    else return (this->type < castComp->type);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& Coin::operator=(Comparable& rhs) {
    Coin* castComparable = static_cast<Coin*>(&rhs);
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
bool Coin::operator==(Comparable& rhs) const {
    Coin* castComp = static_cast<Coin*>(&rhs);
    if (castComp == nullptr) return false;

    return type == castComp->type && grade == castComp->grade && \
        Comparable::key == castComp->key \
        && year == castComp->year;
}

// Initialize members
void Coin::setMembers() {
    Comparable::key = (Comparable::storage[0])[0];
    Comparable::count = (stoi(Comparable::storage[1]));
    Item::year = Comparable::storage[2];
    grade = stoi(Comparable::storage[3]);
    type = Comparable::storage[4];
};