#pragma once
//--------------------------------------------------------------------
// ComicBook.h
// Header of ComicBook class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <string>
#include <iostream>

#include "Item.h"

using namespace std;

class ComicBook : public Item {
private:
    string grade;
    string title;      // specify detail of Comparable type ('type','player','title')
    string publisher;
public:
        
     //------------------------------------------------------------------------
    // Default constructor
    // Preconditions: None
    // Postconditions: New ComicBook object created as implemented by 
    //      derived classes 
    ComicBook() : title(""), grade(""), publisher("") { Comparable::key = 'C'; };

    //------------------------------------------------------------------------
    // Calls constructor
    // Preconditions: None
    // Postconditions: New ComicBook object created as implemented by 
    //      derived classes
    Comparable* create();

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: ComicBook has been cleared along with any dynamic 
    //          memory associated
    ~ComicBook() { //cout << "Destroying Book" << endl; 
    };

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: ComicBook has been cleared along with any dynamic 
    //          memory associated
    void remove() { delete this; };

    //------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a ComicBook Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling ComicBook is less than RHS
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

    // output contents of Comic Book
    friend ostream& operator<<(ostream& os, const ComicBook& myBook);
};