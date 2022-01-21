#pragma once
//--------------------------------------------------------------------
// Customer.h
// Header of Customer class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "Comparable.h"

using namespace std;

class Customer : public Comparable {
private:
    string id;
    string name;
    vector<string> history;

public:

    //-------------------------------------------------------------------------------------
    //  Default Constructor . Convenience
    //  pre: none
    //  post: Customer object created,with optional paramters filling member variables, 
    //      empty transaction history;
    Customer() : id(""), name("") { Comparable::key = 'q'; };

    //-------------------------------------------------------------------------------------
    //  Destructor
    //  pre: none
    //  post: dynamic memory in Transaction history deallocated as needed;
    ~Customer() { 
        //cout << "Destroying Customer" << endl;
    };

    void remove() { delete this; };

    // Construct Customer object
    // pre none
    // post, dynamically create and return new object
    Comparable* create();

    //-------------------------------------------------------------------------------------
    //  Assignment operator 
    //  pre: none
    //  post: assign customer object to calling object, deep copy;
    Comparable& operator=(Comparable& rhs);

    //-------------------------------------------------------------------------------------
    //  Equality operator 
    //  pre: none
    //  post: assign customer object to calling object, deep copy;
    bool operator==(Comparable& rhs) const;

    //-------------------------------------------------------------------------------------
    //  Comparison overload
    //  pre: none
    //  post: used for ordering in ItemBST;
    bool operator<(Comparable& rhs) const;

    //-------------------------------------------------------------------------------------
    //  add transaction 
    //  pre: none
    //  post: add a transaction to the transaction history;
    bool add(string transaction);

    //------------------------------------------------------------------------
    // set members
    // Preconditions: storage has been filled 
    // Postconditions: member variables assigned accordingly    
    void setMembers();

    // return Customer ID
    int getID() const { return stoi(id); };

    //-------------------------------------------------------------------------------------
    //  Print contents of customer and return in string
    //  pre: none
    //  post: string returned with contents of customer history;
    ostream& print(ostream& os) const;
};