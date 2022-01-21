#pragma once
//--------------------------------------------------------------------
// HuffmanAlgorithm.h
// Declaration of HuffmanAlgorithm class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-01-25
//--------------------------------------------------------------------
// HuffmanAlgorithm class: 
//	Description:
//  Accepts input in the form of an array indicating the number of counts
//  for each instance of a character a - z, where the integer at index 0
//  represents the counts of 'a' and the integer at index 25 represents the
//  counts of 'z'. Uses HuffmanTree to store information in Nodes and construct
//  an encoded tree for determining Huffman Codes. Constructs the tree using
//  a template HEAP class. Codes are stored in member variable string array 
//  codeBook where each characters code is stored at the corespoing index 

//  HuffmanAlgorithm can subsequently take a string using 'getWords()' and
//  output the correspoing huffman code using the codeBook lookup.

// Assumptions:
//	 Can only convert numeric characters
//  input must be an array of length NUM_LETTERS or 26
//--------------------------------------------------------------------

#include <iostream>
#include "Heap.h"
#include "HuffmanTree.h"

using namespace std;
static const int NUM_LETTERS = 26;

class HuffmanAlgorithm {
public:
    //------------------------------------------------------------------------
    // Default Constructor 
    // Preconditions: none
    // Postconditions: Member variable 'codeBook' filled with "" strings at 
    //      NUM_LETTERS index
    HuffmanAlgorithm();

    //------------------------------------------------------------------------
    // Constructor - inputs array indicating counts of each letter
    // Preconditions: array argument must have equivalent size as NUM_LETTERS [26]
    //          representing the frequency of each character
    // Postconditions: Huffman algorithm has created separate HuffmanTree objects
    //      containing the character and weights passed as the 'count' array. These
    //      HuffmanTree objects are then used to build a single encoded HuffmanTree
    //      using a minHeap as defined in Heap.h. The encoded tree is then traversed
    //      in order to populate the member variable 'codeBook'
    HuffmanAlgorithm(int(&counts)[NUM_LETTERS]);

    //------------------------------------------------------------------------
    // Destructor 
    // Preconditions: none
    // Postconditions: Dynamic memory has been handled - codebook cleared
    ~HuffmanAlgorithm();

    //------------------------------------------------------------------------
    // getWord - prints Huffman code for given string input
    // Preconditions: none
    // Postconditions: returns a string if 0/1's indicating the huffmant code for the 
    //      string passed. Ignores any non-alphabetical chars
    string getWord(string in) const;

    //------------------------------------------------------------------------
    // output operator << - streams contents of codebook 
    // Preconditions: none
    // Postconditions: Contents of codeBook have been streamed in alphabetical order     
    friend ostream& operator<<(ostream&, const HuffmanAlgorithm& myAlgo);

private:
    string codeBook[NUM_LETTERS]; // storage for huffman codes relating to each char

    // builds a minHeap from huffman tree nodes, returns root of tree
    //------------------------------------------------------------------------
    // encodeTree - builds a minHeap from array of Huffman tree pointers
    // Preconditions: none
    // Postconditions: Collection of HuffmanTree objects has been assembled into a 
    //      single tree, stored within a minHeap 
    const HuffmanTree* encodeTree(Heap<HuffmanTree>& priorityQ) const;
};