#pragma once
//--------------------------------------------------------------------
// ComicBook.cpp
// Implementation of ComicBook class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include "ComicBook.h"

//------------------------------------------------------------------------
// Calls constructor
// Preconditions: None
// Postconditions: New ComicBook object created as implemented by 
//      derived classes
Comparable* ComicBook::create() {
    //cout << "creating ComicBook" << endl;
    Comparable* myObj = new ComicBook();
    return myObj;
}

//------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a ComicBook Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling ComicBook is less than RHS
bool ComicBook::operator<(Comparable& rhs) const {
    ComicBook* castComp = static_cast<ComicBook*>(&rhs);
    if (castComp == nullptr) return false;

    if (this->publisher == castComp->publisher)
        if (this->title == castComp->title)
            if (this->year == castComp->year)
                return (this->grade < castComp->grade);
            else return (this->year < castComp->year);
        else return (this->title < castComp->title);
    else return (this->publisher < castComp->publisher);
}

//------------------------------------------------------------------------
    // operator overload for assignment of objects
    // Preconditions: Calling Comparable must be of the same class as the RHS
    // Postconditions: Comparable has been assingned deep copy of RHS. Returns
    //      true on successful assignment
Comparable& ComicBook::operator=(Comparable& rhs) {
    ComicBook* castComparable = static_cast<ComicBook*>(&rhs);
    if (castComparable != nullptr) {
        this->storage = castComparable->storage;
        setMembers();
    }
    return rhs;
}

//------------------------------------------------------------------------
// operator overload for assignment of objects
// Preconditions: Calling Comparable must be of the same class as the RHS
// Postconditions: Comparable has been assingned deep copy of RHS. Returns
//      true on successful assignment
bool ComicBook::operator==(Comparable& rhs) const {
    ComicBook* castComp = static_cast<ComicBook*>(&rhs);
    if (castComp == nullptr) return false;

    return publisher == castComp->publisher && grade == castComp->grade && \
        publisher == castComp->publisher && Comparable::key == castComp->key && \
        year == castComp->year;
}

// initialize member variables accordingly
void ComicBook::setMembers() {
    Comparable::key = Comparable::storage[0][0];
    Comparable::count = stoi(Comparable::storage[1]);
    Item::year = Comparable::storage[2];
    grade = Comparable::storage[3];
    title = Comparable::storage[4];
    publisher = Comparable::storage[5];
}

ostream& operator<<(ostream& os, const ComicBook& myBook) {
    if (&myBook != nullptr)
        myBook.print(os);
    return os;
};