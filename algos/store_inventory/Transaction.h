#pragma once
//--------------------------------------------------------------------
// Transaction.h
// Header of Transaction class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <string>
//#include "Store.h"
#include "Comparable.h"

using namespace std;

// forward declaration
class Store;

class Transaction : public Comparable {
protected:
    string customerID = "";
    string transactionInfo = "";
public:
    ~Transaction() {};
    
    // Return key
    char getKey() { return key; };

    // return ID
    int getID() { return stoi(customerID); };
    
    // return transaction info
    string getInfo() { return transactionInfo; };

    // Execute derived command
    virtual void execute(Store* myStore) = 0;


};