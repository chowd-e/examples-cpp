#pragma once
//--------------------------------------------------------------------
// SportsCard.cpp
// Implementation of SportsCard class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "SportsCard.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New SportsCard object created as implemented by 
//      derived classes
Comparable* SportsCard::create() {
    //cout << "creating SportsCard" << endl;
    Comparable* myObj = new SportsCard();
    return myObj;
}

//------------------------------------------------------------------------
// pure virtual function for inherited Object comparisons
// Preconditions: RHS must be a SportsCard Object
// Postconditions: two Coins have been compared and a boolean result returned
//      indicating if calling SportsCard is less than RHS
bool SportsCard::operator<(Comparable& rhs) const {
    SportsCard* castComp = static_cast<SportsCard*>(&rhs);
    if (castComp == nullptr) return false;

    if (this->player == castComp->player)
        if (this->year == castComp->year)
            if (this->manufacturer == castComp->manufacturer)
                return this->grade < castComp->grade;
            else return (this->manufacturer < castComp->manufacturer);
        else return (this->year < castComp->year);
    else return (this->player < castComp->player);
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
Comparable& SportsCard::operator=(Comparable& rhs) {
    SportsCard* castComp = static_cast<SportsCard*>(&rhs);
    if (castComp != nullptr) {
        this->storage = castComp->storage;
        setMembers();
    }
    return rhs;
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
bool SportsCard::operator==(Comparable& rhs) const {
    SportsCard* castComp = static_cast<SportsCard*>(&rhs);
    return player == castComp->player && grade == castComp->grade && \
        manufacturer == castComp->manufacturer && Comparable::key == castComp->key && \
        year == castComp->year;
}


// Initialize members
void SportsCard::setMembers() {
    Comparable::key = Comparable::storage[0][0];
    Comparable::count = stoi(Comparable::storage[1]);
    Item::year = Comparable::storage[2];
    grade = Comparable::storage[3];
    player = Comparable::storage[4];
    manufacturer = Comparable::storage[5];
}