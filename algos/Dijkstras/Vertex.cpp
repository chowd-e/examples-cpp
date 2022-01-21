//--------------------------------------------------------------------
// Vertex.cpp
// Implementation of Vertex class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-11
//--------------------------------------------------------------------
// Vertex class
//	Description:
//      This class is used to hold a vertex name for formation of a 
//      Graph object. Class is capable of input and output from iostream 
//      as described in the implementation below.
//      

#pragma once
#include "Vertex.h"
#include <string>

using namespace std;

//------------------------------------------------------------------------
// Overload output stream <<()
// Preconditions: None
// Postconditions: streams Vertex name to the indicated output stream
ostream& operator<<(ostream& os, Vertex& v) {
    os << v.name;
    return os;
}

//------------------------------------------------------------------------
// Overload input stream >>()
// Preconditions: None
// Postconditions: streams single line from input stream into vertex name
istream& operator>>(istream& is, Vertex& v) {
    getline(is, v.name);
    return is;
}