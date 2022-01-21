#pragma once
//--------------------------------------------------------------------
// Item.cpp
// Implementation of Item class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------

#include "Item.h"

//-----------------------   ACCESSORS    ------------------

// overload function to print contents to ostream
ostream& Item::print(ostream& os) const {
    if (this == nullptr) return os;
    for (auto i = storage.begin(); i != storage.end(); ++i) {
        (i == storage.begin() + 1 ? os << count : os << *i);
        if (i != storage.end() - 1)
            os << ", ";
        else os << endl;
    }
    return os;
}

//bool Item::operator==(Item& rhs) const {
//    if (this == nullptr && &rhs == nullptr) return true;
//    if (this == nullptr || &rhs == nullptr) return false;
//
//    vector<string> rhStorage = rhs.getStorage();
//    bool size = Comparable::storage.size() == rhStorage.size();
//    int minSize = min(Comparable::storage.size(), rhStorage.size());
//
//    for (int i = 0; i < minSize; i++)
//        if (Comparable::storage[i] != rhStorage[i])
//            return false;
//    return size;
//}

// assignment operator overload - cast comparable to Item
Comparable& Item::operator=(Comparable& rhs) {
    Item* castComp = static_cast<Item*>(&rhs);
    vector<string> rhStorage = castComp->getStorage();

    if (this != nullptr) {
        if (!(*this == *castComp)) {
            this->remove();

            for (auto& i : rhStorage)
                Comparable::storage.push_back(i);

            setMembers();
        }
    }
    return rhs;
}

bool Item::operator==(Comparable& rhs) const
{
    Item* castItem = static_cast<Item*>(&rhs);
    return *this == *castItem;
}

bool Item::operator<(Comparable& rhs) const
{
    Item* castItem = static_cast<Item*>(&rhs);
    return castItem == nullptr ? false : this->count < rhs.getCount();
}

// overload operator to print contents to ostream
ostream& operator<<(ostream& os, const Item& myItem) {
    if (&myItem != nullptr)
        myItem.print(os);
    return os;
}

ostream& operator<<(ostream& os, const Item* myItem) {
    if (myItem != nullptr)
        myItem->print(os);
    return os;
}

//-----------------------   MUTATORS    ------------------

//// assignment opertaor to Item
//Item& Item::operator=(Item& rhs) {
//    if (this != nullptr) {
//        if (!(*this == rhs)) {
//            this->remove();
//
//            for (auto& i : rhs.getStorage())
//                Comparable::storage.push_back(i);
//
//            setMembers();
//        }
//    }
//    return rhs;
//}

// Initialize members
void Item::setMembers() {
    Comparable::key = (Comparable::storage[0])[0];
    Item::count = (stoi(Comparable::storage[1]));
};
