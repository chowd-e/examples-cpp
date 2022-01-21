#pragma once
//--------------------------------------------------------------------
// HashTable.h
// Header of HashTable class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <string>
#include <vector>

//#include "Comparable.h"
//#include "Item.h"
//#include "Transaction.h"
//
#include "Coin.h"
#include "SportsCard.h"
#include "ComicBook.h"
#include "Customer.h"

#include "Buy.h"
#include "Sell.h"
#include "Display.h"
#include "History.h"
#include "CustomerCMD.h"

using namespace std;

class HashTable {
private:
    int buckets;                    // number of buckets
    char customerKey;
    vector<Comparable*> table;     //storage for hash table Items

public:
    //------------------------------------------------------------------------
    // Default Constructor
    // Preconditions: none
    // Postconditions: Initialized HashTable to size 'b' filled with nullptrs
    //      optional argument of keysPtr to load dummy objects of at initial key locations
    HashTable(char customerKey = 'q', unsigned int b = 1000);

    //------------------------------------------------------------------------
    // destructor
    // Preconditions: none
    // Postconditions: All dynamica memory has been deallocated of Table Contents
    ~HashTable();

    // --------------- MUTATORS ----------------------------------------------

    // remove all elements from table
    void clear();

    // copy contents to another hash table
    void copy(const HashTable& rhs);

    //------------------------------------------------------------------------
   // Copy Constructor
   // Preconditions: none
   // Postconditions: Deep Copy of all objects within HashTable
    HashTable(const HashTable& rhs);

    //------------------------------------------------------------------------
    // Create Dummy Comparable
    // Preconditions: none
    // Postconditions: new comparable created with dynamically allocated
    Comparable* build(char ch);

    //------------------------------------------------------------------------
    // insert Item into hash table
    // Preconditions: no passing nullptr
    // Postconditions: Item has been inserted into table based on key/hash
    void insert(Comparable* myComparable);

    //------------------------------------------------------------------------
    // remove Item from hash table
    // Preconditions: not passing nullptr
    // Postconditions: Item has been marked deleted but remains in table
    //      returns true if value was marked deleted, false if not found
    bool remove(Comparable* myComparable);

    const HashTable& operator=(const HashTable& rhs);

    //------------------------ ACCESSORS ----------------------------------------

    bool operator==(const HashTable& rhs) const;

    //------------------------------------------------------------------------
    // hash function pointing to dummy constructor
    // Preconditions: only accept uppercase letters
    // Postconditions: character has been hashed returning index in table for 
    //      creation, returns negative 1 if invalid
    int hash(char ch) const;
};