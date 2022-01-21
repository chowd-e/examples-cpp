#pragma once
//--------------------------------------------------------------------
// Store.h
// Header of Store class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Store.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------------------
//  Create Comparable
//  pre: File has been read and returned properly formatted string as arg
//  post: Comparable has been created and allocated to the heap - nullptr returned otherwise
Comparable* Store::createComparable(string is)
{
    if(is == "") return nullptr;
    Comparable* newComp;
    if (isdigit(is[0]))
        newComp = factory.build(CUSTOMER_KEY);
    else if (isalpha(is[0]))
        newComp = factory.build(is[0]);
    else return nullptr;

    if (newComp == nullptr) return nullptr; // incorrect build

    newComp->init(is); // populate member variables
    return newComp;
}

//-------------------------------------------------------------------------------------
//  create transaction
//  pre: File has been read and returned properly formatted string as arg
//  post: dynamically allocated transaction absed on factory method returned
Transaction* Store::createTransaction(string is)
{
    if (is == "") return nullptr;

    if (isalpha(is[0])) {
        char ch = tolower(is[0]);
        Comparable* newComp = factory.build(ch);
        if (newComp == nullptr) return nullptr; // incorrect build

        Transaction* newTrans = static_cast<Transaction*>(newComp);
        if (newTrans == nullptr) return nullptr; // incorrect conversion

        newTrans->init(is);
        return newTrans;
    }
    return nullptr;
}

//-------------------------------------------------------------------------------------
//  Insert Customer
//  pre: none
//  post: post: customer inserted into appropriate storage location, returns false on fail - memory
//      handed to BST and handled outside this function on false insertion
bool Store::insertCustomer(Comparable* myComp)
{
    //insert into customer array based on ID
    if (myComp->getKey() != CUSTOMER_KEY) return false; // don't insert item

    Customer* newCust = static_cast<Customer*>(myComp);
    if (newCust == nullptr) return false; // failed cast

    int index = newCust->getID(); // index of customer id

    if (index >= 0 && index < MAX_CUSTOMERS) { //validate input
        if (customerIDs[index] != nullptr) {  // customer ID exists already, failure
            cerr << "Error - Duplicate Customer ID: " << newCust->getID() << endl;
            return false;
        }
        else if (!customerTree.insert(myComp)) // insert new customer to Tree
            return false; // found duplicate in tree 
    }
    else { // invalid customer ID
        cerr << "Error - Invalid Customer ID: \"" << index << "\"" << endl;
        //myComp->remove();
        return false;
    }

    customerIDs[index] = myComp; // write new customer to array
    myComp = nullptr; // memory handed to tree
    return true;
}
//-------------------------------------------------------------------------------------
//  Insert Item
//  pre: none
//  post: item inserted into appropriate storage location, returns false on fail - memory
//      handed to BST and handled within this function on false insertion
bool Store::insertItem(Comparable* myComp)
{
    // insert item into tree collection
    if (myComp->getKey() == CUSTOMER_KEY) return false; // don't insert customer

    int index = factory.hash(myComp->getKey());
    if (index == -1) return false;// index wasn't found

    if (inventory[index] == nullptr) //tree hasn't been created yet
        inventory[index] = new ComparableBST(myComp); // dynamially allocate new tree
    else if (!(*inventory[index]).insert(myComp)) // insert in existing tree
        return false; // newComp->remove(); // failed insertion

    return true;
}

//-------------------------------------------------------------------------------------
//  Default Constructor 
//  pre: none
//  post: Store object created, nullptrs fill customerIDs and inventory. orderedInventory
//      empty and CustomerTree emoty tree;
Store::Store() {
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
        inventory[i] = nullptr;

    for (int i = 0; i < MAX_CUSTOMERS; i++)
        customerIDs[i] = nullptr;
}

//-------------------------------------------------------------------------------------
//  Destructor
//  pre: none
//  post: Store object deleted with all dynamically allocated memory handled as needed;
Store::~Store() {
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
        if (inventory[i] != nullptr) delete inventory[i];
}

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
void Store::inputInventory(ifstream& inFile) {
    string str;
    if (inFile.bad()) return;
    while (!inFile.eof()) {
        getline(inFile, str);
        Comparable* newComp = createComparable(str);
        if (newComp == nullptr) continue; // improper build

        if (!insertItem(newComp))
            newComp->remove(); // insertion failed       
    }
}

//------------------------------------------------------------------------
    // Input Customers
    // Preconditions: Input is formatted as specified 
    // Postconditions: Switch between three cases (Inventory, Customer, Command)
    //      2. Inserts customer into ItemBST and places a pointer to node in 
    //      member variable 'customerIDs' at index relating to Customer ID
void Store::inputCustomers(ifstream& inFile) {
    string str;
    if (inFile.bad()) return;
    while (!inFile.eof()) {
        getline(inFile, str);
        Comparable* newComp = createComparable(str);
        if (newComp == nullptr) continue; // improper build

        if(!insertCustomer(newComp))
            newComp->remove();     // handle memory here if insertion fails   
    }
}

//------------------------------------------------------------------------
// Input Commands
// Preconditions: Input is formatted as specified 
// Postconditions: 
//      3. creates transactions as specified by command input linked
//      to customer accessed through 'customerIDs' to insert Transaction to
//      Customer->history vector
void Store::inputCommands(ifstream& inFile)
{
    string str;
    if (inFile.bad()) return;
    while (!inFile.eof()) {
        getline(inFile, str);
        Transaction* newCmd = createTransaction(str);
        if (newCmd == nullptr) continue; // improper build
        
        newCmd->execute(this); 
        newCmd->remove();
    }
}

// ------------------------------------------------------------------------------------
//  Validate Transaction
//  pre: none
//  post: Bool indicating if transaction is valid
bool Store::validTransaction(Transaction* myTrans) {
    ////decrement count for sold item
    int index = myTrans->getID();
    if (index < 0 || index > MAX_CUSTOMERS) {
        cerr << "Error - Invalid Customer ID: " << index << endl;
        return false;
    }

    // test validity of request to sell
    Comparable* myComp = customerIDs[myTrans->getID()];
    if (myComp == nullptr) {
        cerr << "Error - Invalid Input, Customer DNE: " << myTrans->getID() << endl;
        return false;
    }

    Customer* myCust = static_cast<Customer*>(myComp);
    if (myCust == nullptr) {
        cerr << "Error - Invalid Conversion" << endl;
        return false;
    }

    return true;
}


//------------------------------------------------------------------------
// sell
// Preconditions: none
// Postconditions: sell an item located in inventory, returns true on sucess
//      false if < 1 item remaining or item doesn't exist in invetory, decrements count
bool Store::sell(Transaction* myTrans)
{
    ////decrement count for sold item
    int index = myTrans->getID();
    if (index < 0 || index > MAX_CUSTOMERS) {
        cerr << "Error - Invalid Customer ID: " << index << endl;
        return false;
    }

  /*  if (!validTransaction(myTrans))
        return false;*/

    string itemInfo = myTrans->getInfo();
    itemInfo.insert(2, " 1,"); // insert dummy value for quantity to construct properly
    Comparable* temp = createComparable(itemInfo);
    if (temp == nullptr) {
        cerr << "Error - Invalid Input, Failed Construction: " << myTrans->getInfo() << endl;
        return false;
    }

    // test validity of request to sell
    Comparable* myComp = customerIDs[myTrans->getID()];
    if (myComp == nullptr) {
        cerr << "Error - Invalid Input, Customer DNE: " << myTrans->getID() << endl;
        temp->remove();
        return false;
    }

    Customer* myCust = static_cast<Customer*>(myComp);
    if (myCust == nullptr) {
        temp->remove();
        return false;
    }

    // find comparable in searchtree
    Comparable* modified = inventory[factory.hash(temp->getKey())]->retrieve(temp);
    if (modified == nullptr)
        cerr << "Error - Item not found: " << temp << endl;
    else if (!modified->decrement()) {
        cerr << "Error - Sell, No inventory" << endl;
        temp->remove();
        return false;
    }

    // add transaction to customer if sucessful above
    myCust->add("S, " + myTrans->getInfo());
    temp->remove();
    return true;
}

//------------------------------------------------------------------------
// buy
// Preconditions: input is valid per specification
// Postconditions: Adds item to inventory, increments count, true on sucess, false on invalid input
bool Store::buy(Transaction* myTrans)
{
    // add transaction to customer and decrement
    int index = myTrans->getID();
    if (index < 0 || index > MAX_CUSTOMERS) {
        cerr << "Error - Invalid Customer ID: " << index << endl;
        return false;
    }
    Comparable* myComp = customerIDs[index];
    if (myComp == nullptr) {
        cerr << "Error - Invalid Input, Customer DNE: " << myTrans->getID() << endl;
        return false;
    }
    Customer* myCust = static_cast<Customer*>(myComp);
    if (myCust == nullptr) {
        cerr << "Error - Invalid Input, Failed Construction: " << myTrans->getInfo() << endl;
        return false;
    }

    ////increment count for bought item
    string itemInfo = myTrans->getInfo();
    itemInfo.insert(2, " 1,"); // insert dummy value for quantity to construct properly
    Comparable* temp = createComparable(itemInfo);
    // find comparable in searchtree
    Comparable* modified = inventory[factory.hash(temp->getKey())]->retrieve(temp);
    if (modified == nullptr)
        cerr << "Error - Item not found: " << temp << endl;
    else {
        modified->increment();
        myCust->add("B, " + myTrans->getInfo());
    }
    temp->remove();
    return true;
}

//bool Store::addTransaction(Customer* myCust, string transaction)
//{
//    Comparable* myComp = customerIDs[customerIndex];
//    Customer* myCust = static_cast<Customer*>(myComp);
//    myCust->add(transaction);
//}

void Store::displayCustomer(int ID)
{
    cout << customerIDs[ID] << endl;
}

//------------------------------------------------------------------------
// display()
// Preconditions: none
// Postconditions: outputs the entire inventory of the store, including the number
//      of each item in inventory 
void Store::display() const{
    for (int i = 0; i < distance(begin(acceptedItems), end(acceptedItems)); i++) {
        int index = factory.hash(acceptedItems[i]);
        if (inventory[index] != nullptr)
            inventory[index]->print(cout);
    }
}

//------------------------------------------------------------------------
// display()
// Preconditions: none
// Postconditions: outputs the entire inventory of the store, including the number
//      of each item in inventory 
void Store::history() const { 
    //cout << &customerTree;
    customerTree.print(cout);
}


