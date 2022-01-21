//--------------------------------------------------------------------
// HuffmanAlgorithm.cpp
// Implementation of HuffmanAlgorithm class
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

#include "HuffmanAlgorithm.h"
#include "Heap.h"
#include <stdexcept>

//------------------------------------------------------------------------
// Default Constructor 
// Preconditions: none
// Postconditions: Member variable 'codeBook' filled with "" strings at 
//      NUM_LETTERS index
HuffmanAlgorithm::HuffmanAlgorithm() {
    fill_n(codeBook, NUM_LETTERS, "");
}

//------------------------------------------------------------------------
// Constructor - inputs array indicating counts of each letter
// Preconditions: array argument must have equivalent size as NUM_LETTERS [26]
//          representing the frequency of each character
// Postconditions: Huffman algorithm has created separate HuffmanTree objects
//      containing the character and weights passed as the 'count' array. These
//      HuffmanTree objects are then used to build a single encoded HuffmanTree
//      using a minHeap as defined in Heap.h. The encoded tree is then traversed
//      in order to populate the member variable 'codeBook'
HuffmanAlgorithm::HuffmanAlgorithm(int(&counts)[NUM_LETTERS])
{
    HuffmanTree* htPtrs[NUM_LETTERS]; // array of huffmanTree ptrs
  
    for (int i = 0; i < NUM_LETTERS; i++) {
        HuffmanTree* ht = new HuffmanTree((i + 97), counts[i]); // convert i to ASCII values ['a' -> 'z']
        htPtrs[i] = ht; 
    }

    Heap<HuffmanTree> priorityQ(htPtrs, NUM_LETTERS); // construct initial heap
    const HuffmanTree* codifiedTree = encodeTree(priorityQ);
    codifiedTree->createCodes(codeBook);
}

//------------------------------------------------------------------------
// Destructor 
// Preconditions: none
// Postconditions: Dynamic memory has been handled - codebook cleared
HuffmanAlgorithm::~HuffmanAlgorithm() {
    fill_n(codeBook, NUM_LETTERS, "");
}

// builds a minHeap from huffman tree nodes, returns root of tree
//------------------------------------------------------------------------
// encodeTree - builds a minHeap from array of Huffman tree pointers
// Preconditions: none
// Postconditions: Collection of HuffmanTree objects has been assembled into a 
//      single tree, stored within a minHeap 
const HuffmanTree* HuffmanAlgorithm::encodeTree(Heap<HuffmanTree>& priorityQ) const
{
    while (priorityQ.size() > 1) { // turn into huffman Tree
        HuffmanTree* lhs = priorityQ.deleteMin(); // remove dynamically allocated HT
        HuffmanTree* rhs = priorityQ.deleteMin();
        HuffmanTree* newHT = new HuffmanTree(lhs, rhs); // insert into new Tree
        priorityQ.insert(newHT);
    }
    return priorityQ.findMin();
}

//------------------------  ACCESSORS   ----------------------------------

//------------------------------------------------------------------------
// getWord - prints Huffman code for given string input
// Preconditions: none
// Postconditions: returns a string if 0/1's indicating the huffmant code for the 
//      string passed. Ignores any non-alphabetical chars
string HuffmanAlgorithm::getWord(string in) const
{
    string out;
    for (unsigned i = 0; i < in.length(); i++) {
        int idx = tolower(in[i]);
        idx -= 97; // subtract 97 from ascii value to index codeBook
        if (idx >= 0 && idx < NUM_LETTERS)
            out += codeBook[idx];
    }
    return out;
}

//------------------------------------------------------------------------
// output operator << - streams contents of codebook 
// Preconditions: none
// Postconditions: Contents of codeBook have been streamed in alphabetical order   
ostream& operator<<(ostream& os, const HuffmanAlgorithm& myAlgo) {
    for (int i = 0; i < NUM_LETTERS; i++) {
        char ch = i + 97;
        os <<  ch << ' ' << myAlgo.codeBook[i] << endl;
    }
    return os;
}