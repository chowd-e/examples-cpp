#pragma once
//--------------------------------------------------------------------
// SportsCard.h
// Header of SportsCard class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <iostream>

#include "Item.h"

using namespace std;

class SportsCard : public Item {
private:
    string grade;
    string player;      // specify detail of Comparable type ('type','player','title')
    string manufacturer;
public:

    //------------------------------------------------------------------------
    // Default constructor
    // Preconditions: None
    // Postconditions: New SportsCard object created as implemented by 
    //      derived classes
    SportsCard() : player(""), grade(""), manufacturer("") { Comparable::key = 'S'; };

    //------------------------------------------------------------------------
    // Calls constructor
    // Preconditions: None
    // Postconditions: New SportsCard object created as implemented by 
    //      derived classes
    Comparable* create();

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: SportsCard has been cleared along with any dynamic 
    //          memory associated
    ~SportsCard() {//cout << "Destroying Card" << endl; 
    };

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: SportsCard has been cleared along with any dynamic 
    //          memory associated
    void remove() { delete this; };

    //------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a SportsCard Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling SportsCard is less than RHS
    bool operator<(Comparable& rhs) const;

    //------------------------------------------------------------------------
    // operator overload for assignment of objects
    // Preconditions: Calling Comparable must be of the same class as the RHS
    // Postconditions: Comparable has been assingned deep copy of RHS. Returns
    //      true on successful assignment
    Comparable& operator=(Comparable& rhs);

    //------------------------------------------------------------------------
    // operator overload for assignment of objects
    // Preconditions: Calling Comparable must be of the same class as the RHS
    // Postconditions: Comparable has been assingned deep copy of RHS. Returns
    //      true on successful assignment
    bool operator==(Comparable& rhs) const;

    // Initialize members
    void setMembers();
};