//--------------------------------------------------------------------
// Vertex.h
// Header of Vertex class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-11
//--------------------------------------------------------------------
// Vertex class
//	Description:
//      This class is used to hold a vertex name for formation of a 
//      Graph object. Class is capable of input and output from iostream 
//      as described in the implementation file.
//          
#pragma once
#include <iostream>

using namespace std;

class Vertex {
private:
    string name = ""; // string storing the name of the vertex

public:
    //------------------------------------------------------------------------
    // Overload output stream <<()
    // Preconditions: None
    // Postconditions: streams Vertex name to the indicated output stream
    friend ostream& operator<<(ostream& os, Vertex& v);

    //------------------------------------------------------------------------
    // Overload input stream >>()
    // Preconditions: None
    // Postconditions: streams single line from input stream into vertex name
    friend istream& operator>>(istream& is, Vertex& v);
};