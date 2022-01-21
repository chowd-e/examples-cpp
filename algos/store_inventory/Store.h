#pragma once
//--------------------------------------------------------------------
// Store.h
// Header of Store class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include <string>
#include <iostream>
#include <fstream>

//#include "ItemBST.h"
#include "ComparableBST.h"
#include "HashTable.h"

#include "Comparable.h"

//#include "Item.h"
//#include "Customer.h"
//#include "Transaction.h"
//
//#include "Coin.h"
//#include "ComicBook.h"
//#include "SportsCard.h"

#include "Buy.h"
//#include "Sell.h"
//#include "Display.h"
//#include "History.h"
//#include "Customer.h"

using namespace std;

class Transaction;

class Store {
private:
    static const char CUSTOMER_KEY = 'q';
    const char acceptedItems[3] = { 'M','C','S' }; // list of acceptable Items - prints in order
    //const char acceptedActions[5] = {'B','S','C','D','H'}; // insert accepted items to hashtables
    static const int MAX_CUSTOMERS = 1000; 
    static const int MAX_TABLE_SIZE = 'z' - 'A';

    
    Comparable* customerIDs[MAX_CUSTOMERS];  // array containing pointers to customers
    HashTable factory = HashTable(CUSTOMER_KEY, 1000); // hash table storint item constructors
    ComparableBST* inventory[MAX_TABLE_SIZE]; // array containing trees with coins / comics etc.
    ComparableBST customerTree;

    //-------------------------------------------------------------------------------------
    //  Create Comparable
    //  pre: File has been read and returned properly formatted string as arg
    //  post: Comparable has been created and allocated to the heap - nullptr returned otherwise
    Comparable* createComparable(string is);

    //-------------------------------------------------------------------------------------
    //  Create Transaction
    //  pre: File has been read and returned properly formatted string as arg
    //  post: Transaction has been created and allocated to the heap - nullptr returned otherwise
    Transaction* createTransaction(string is);

    //-------------------------------------------------------------------------------------
    //  Insert Customer
    //  pre: none
    //  post: post: customer inserted into appropriate storage location, returns false on fail - memory
    //      handed to BST and handled within this function on false insertion
    bool insertCustomer(Comparable* myComp);

    // ------------------------------------------------------------------------------------
    //  Validate Transaction
    //  pre: none
    //  post: Bool indicating if transaction is valid
    bool validTransaction(Transaction* myTrans);

    //-------------------------------------------------------------------------------------
    //  Insert Item
    //  pre: none
    //  post: item inserted into appropriate storage location, returns false on fail - memory
    //      handed to BST and handled within this function on false insertion
    bool insertItem(Comparable* myComp);

public:

    //-------------------------------------------------------------------------------------
    //  Default Constructor 
    //  pre: none
    //  post: Store object created, nullptrs fill customerIDs and inventory. orderedInventory
    //      empty and CustomerTree emoty tree;
    Store();

    //-------------------------------------------------------------------------------------
    //  Destructor
    //  pre: none
    //  post: Store object deleted with all dynamically allocated memory handled as needed;
    ~Store();

    // nope Store(Store& rhs);

    // nope Store& operator=(Store& rhs);
    
    //------------------------------------------------------------------------
    // Input Inventory
    // Preconditions: Input is formatted as specified 
    // Postconditions: Switch between three cases (Inventory, Customer, Command)
    //      1. Appends to existing HashTable 'inventory', creates if doesn't exist
    //      2. Inserts customer into ItemBST and places a pointer to node in 
    //      member variable 'customerIDs' at index relating to Customer ID
    //      3. creates transactions as specified by command input linked
    //      to customer accessed through 'customerIDs' to insert Transaction to
    //      Customer->history vector
    void inputInventory(ifstream& inFile);

    //------------------------------------------------------------------------
    // Input Customers
    // Preconditions: Input is formatted as specified 
    // Postconditions: Switch between three cases (Inventory, Customer, Command)
    //      2. Inserts customer into ItemBST and places a pointer to node in 
    //      member variable 'customerIDs' at index relating to Customer ID
    void inputCustomers(ifstream& inFile);

    //------------------------------------------------------------------------
    // Input Commands
    // Preconditions: Input is formatted as specified 
    // Postconditions: 
    //      3. creates transactions as specified by command input linked
    //      to customer accessed through 'customerIDs' to insert Transaction to
    //      Customer->history vector
    void inputCommands(ifstream& inFile);

    //------------------------------------------------------------------------
    // sell
    // Preconditions: none
    // Postconditions: sell an item located in inventory, returns true on sucess
    //      false if < 1 item remaining or item doesn't exist in invetory, decrements count
    bool sell(Transaction* myTrans);

    //------------------------------------------------------------------------
    // buy
    // Preconditions: input is valid per specification
    // Postconditions: Adds item to inventory, increments count, true on sucess, false on invalid input 
    bool buy(Transaction* myTrans);

    ////-------------------------------------------------------------------------------------
    ////  Add transaction
    ////  pre: Customer exists already
    ////  post: transaction has been added to customer of interested
    //bool addTransaction(Customer* myCust, string transaction);

    //------------------------------------------------------------------------
    // displayCustomer()
    // Preconditions: none
    // Postconditions: outputs the entire history of a single Customer, including 
    //      item transacted
    void displayCustomer(int ID);

    //------------------------------------------------------------------------
    // display()
    // Preconditions: none
    // Postconditions: outputs the entire inventory of the store, including the number
    //      of each item in inventory 
    void display() const;

    //------------------------------------------------------------------------
    // display()
    // Preconditions: none
    // Postconditions: outputs the entire inventory of the store, including the number
    //      of each item in inventory 
    void history() const ;
};