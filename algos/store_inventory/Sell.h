#pragma once
#include "Transaction.h"


class Sell : public Transaction {
public:
    //------------------------------------------------------------------------
    // Default constructor
    // Preconditions: None
    // Postconditions: New Sell object created as implemented by 
    //      derived classes
    Sell() { Comparable::key = 's'; };

    //------------------------------------------------------------------------
    // Calls constructor
    // Preconditions: None
    // Postconditions: New Sell object created as implemented by 
    //      derived classes
    Comparable* create();

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: Sell has been cleared along with any dynamic 
    //          memory associated
    ~Sell() {//cout << "Destroying Sell" << endl; 
    };

    //------------------------------------------------------------------------
    // calls destructor
    // Preconditions: None
    // Postconditions: Sell has been cleared along with any dynamic 
    //          memory associated
    void remove() { delete this; };

    //------------------------------------------------------------------------
    // pure virtual function for inherited Object comparisons
    // Preconditions: RHS must be a Sell Object
    // Postconditions: two Coins have been compared and a boolean result returned
    //      indicating if calling Sell is less than RHS
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
    // Preconditions: storage has been filled with approriate input
    // Postconditions: member variables assigned according to input
    void setMembers();

    //------------------------------------------------------------------------
    // execite
    // Preconditions: Comparable has been initialized
    // Postconditions: call to function in Store to perform
    void execute(Store* myStore);

    // print contents of transaction
    ostream& print(ostream& os) const { return os; };
};