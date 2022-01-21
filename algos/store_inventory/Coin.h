#pragma once
//--------------------------------------------------------------------
// Coin.h
// Header of Coin class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <string>

#include "Item.h"

using namespace std;

class Coin : public Item {
private:
    int grade;       // rating given to Comparable
    string type;      // specify detail of Comparable type ('type','player','title')

public:
    //------------------------------------------------------------------------
    // Default constructor
    // Preconditions: None
    // Postconditions: New Coin object created as implemented by 
    //      derived classes
    Coin() : grade(0), type("") { Comparable::key = 'M'; };
    
    //------------------------------------------------------------------------
    // Calls constructor
    // Preconditions: None
    // Postconditions: New Coin object created as implemented by 
    //      derived classes
    Comparable* create();

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: Coin has been cleared along with any dynamic 
    //          memory associated
    ~Coin() {//cout << "Destroying Coin" << endl; 
    };

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: Coin has been cleared along with any dynamic 
    //          memory associated
    void remove() { delete this; };

    //------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a Coin Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling Coin is less than RHS
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
     
    //------------------------------------------------------------------------
     // set members
     // Preconditions: storage has been filled 
     // Postconditions: member variables assigned accordingly
    void setMembers();
 };