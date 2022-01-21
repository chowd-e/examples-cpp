#pragma once
//--------------------------------------------------------------------
// HashTable.cpp
// Implementation of HashTable class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "HashTable.h"

//------------------------------------------------------------------------
// Default Constructor
// Preconditions: none
// Postconditions: Initialized HashTable to size 'b' filled with nullptrs
//      optional argument of keysPtr to load dummy objects of at initial key locations
HashTable::HashTable(char custKey, unsigned int b) : customerKey(custKey), buckets(b) {
    while (table.size() < b)
        table.push_back(nullptr);

    Comparable* dummyCoin = new Coin();
    Comparable* dummyCard = new SportsCard();
    Comparable* dummyBook = new ComicBook();
    Comparable* dummyCustomer = new Customer();
    //set transactions as lowercase

    // Factory methods for item inventory
    table[hash(dummyCoin->getKey())] = dummyCoin;
    table[hash(dummyCard->getKey())] = dummyCard;
    table[hash(dummyBook->getKey())] = dummyBook;
    table[hash(dummyCustomer->getKey())] = dummyCustomer;

    // Factory emthods for Transactions
    Comparable* dummyBuy = new Buy();
    Comparable* dummySell = new Sell();
    Comparable* dummyHist = new History();
    Comparable* dummyDisplay = new Display();
    Comparable* dummyCustDetails = new CustomerCMD();
       

    // Factory methods for actions
    table[hash(dummyBuy->getKey())] = dummyBuy;
    table[hash(dummySell->getKey())] = dummySell;
    table[hash(dummyHist->getKey())] = dummyHist;
    table[hash(dummyDisplay->getKey())] = dummyDisplay;
    table[hash(dummyCustDetails->getKey())] = dummyCustDetails;
}

//------------------------------------------------------------------------
// destructor
// Preconditions: none
// Postconditions: All dynamica memory has been deallocated of Table Contents
HashTable::~HashTable() {
    clear();
}

//------------------------------------------------------------------------
// Copy Constructor
// Preconditions: none
// Postconditions: Deep Copy of all objects within HashTable
HashTable::HashTable(const HashTable& rhs) {
    if (!(*this == rhs)) {
        copy(rhs);
    }
}

//------------------------------------------------------------------------
// assignment operator
// Preconditions: none
// Postconditions: copy rhs to calling object - no dynamic memory
const HashTable& HashTable::operator=(const HashTable& rhs) {
    copy(rhs);
    return rhs;
}

//------------------------------------------------------------------------
// equality overload
// Preconditions: none
// Postconditions: test of equality
bool HashTable::operator==(const HashTable& rhs) const {
    return table == rhs.table && buckets == rhs.buckets;
}

//------------------------------------------------------------------------
// destructor
// Preconditions: none
// Postconditions: All dynamica memory has been deallocated of Table Contents
void HashTable::clear() {
    for (auto& item : table)
        if(item != nullptr) item->remove(); // dynamic memory 

    table.clear();
    buckets = 0;
}

//------------------------------------------------------------------------
// copy constructor
// Preconditions: none
// Postconditions: shallow copy of items in rhs - no dynamic memory responsibility
void HashTable::copy(const HashTable& rhs) {
    buckets = rhs.buckets;
    customerKey = rhs.customerKey;
    clear();

    for (auto& item : table) 
        table.push_back(item);
}

//------------------------------------------------------------------------
// hash function pointing to dummy constructor
// Preconditions: only accept uppercase letters
// Postconditions: character has been hashed returning index in table for 
//      creation, returns negative 1 if invalid
int HashTable::hash(char ch) const {
    if ((ch == NULL || ch < 'A' || (ch > 'Z' && ch < 'a') || ch > 'z') && ch != customerKey) {
        return -1;
    }
    else return ch - 'A';
}

//------------------------------------------------------------------------
// Create Dummy Comparable
// Preconditions: none
// Postconditions: new comparable created with dynamically allocated
Comparable* HashTable::build(char ch) {
    int subscript = hash(ch);
    if (subscript != -1 && table[subscript] != nullptr)   return table[subscript]->create();
    else {
        cerr << "Error - Invalid Key: \"" << ch << "\"" << endl;
        return nullptr;
    }
}

//------------------------------------------------------------------------
    // insert Item into hash table
    // Preconditions: no passing nullptr
    // Postconditions: Item has been inserted into table based on key/hash
void HashTable::insert(Comparable* myComparable) {
    table[hash(myComparable->getKey())] = myComparable;
}

//------------------------------------------------------------------------
// remove Item from hash table
// Preconditions: not passing nullptr
// Postconditions: Item has been marked deleted but remains in table
//      returns true if value was marked deleted, false if not found
bool HashTable::remove(Comparable* myComparable) { 
    int index = hash(myComparable->getKey());
    if (index != -1 && table[index] != nullptr) {
        table[index] = nullptr;
        return true;
    }
    return false;
    
};