#pragma once
//--------------------------------------------------------------------
// Item.h
// Header of Item class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <string>
#include <iostream>
#include <sstream>

#include "Comparable.h"

using namespace std;

class Item : public Comparable {
protected:
    string year = 0;

public:

    Item() : year("") { //cout << "Creating Item" << endl; 
    };

    ~Item() { //cout << "Destroying Item" << endl;
    };

    // overload function to print contents to ostream
    ostream& print(ostream& os) const;

    // overload operator to print contents to ostream
    friend ostream& operator<<(ostream& os, const Item& myItem);
    
    // overload operator to print contents to ostream
    friend ostream& operator<<(ostream& os, const Item* myItem);

    // assignment operator overload - cast comparable to Item
    Comparable& operator=(Comparable& rhs);

    //// assignment opertaor to Item
    //Item& operator=(Item& rhs);

    // Initialize members
    void setMembers();

    Comparable* create() { return nullptr; };

    Comparable& operator=(const Comparable& rhs);

    bool operator==(Comparable& rhs) const;

    bool operator<(Comparable& rhs) const;

    //bool operator<(Item& rhs) const;

    void remove() { delete this; };
};