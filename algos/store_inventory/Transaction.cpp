#pragma once
//--------------------------------------------------------------------
// Transaction.cpp
// Implementation of Transaction class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
//#include <iostream>
//#include "Transaction.h"

//-------------------------------------------------------------------------------------
//  Base Destructor
//  pre: none
//  post: Transaction type nullified
//Transaction::~Transaction() { cout << "destroy transaction" << endl; }

////-------------------------------------------------------------------------------------
////  get type
////  pre: none
////  post: return character indicating transaction type;
//char Transaction::getType() const {
//    return this->type;
//}
//
////-------------------------------------------------------------------------------------
////  assignmeent operator
////  pre: none
////  post: Transaction has been assigned value of rhs using create() with copying rhs.item;
//const Transaction* Transaction::operator=(const Transaction* rhs) {
//    this->create(rhs->getType());
//}
//
////-------------------------------------------------------------------------------------
////  Overload output operator
////  pre: none
////  post: contents of transaction have been output to specified ostream in indicated format;
//ostream& operator<<(ostream& os, const Transaction*& myTrans) {
//    os << myTrans->getType() << ", " << myTrans->print() << endl;
//    return os;
//}
