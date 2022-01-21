//------------------------------------------------------------------------
// HW2.CPP
// Driver code for testing Huffman class for creating Huffman codes
// Author:  Clark Olson
//------------------------------------------------------------------------
// This code tests all of the basic functionality of the Huffman class
// for CSS 502 HW 2.  It is not meant to exhaustively test the class.
//
// Assumptions:
//  - none
//------------------------------------------------------------------------
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include "HuffmanAlgorithm.h"	// Must have "const int NUM_LETTERS = 26;"
using namespace std;


//------------------------------------------------------------------------
// main
// Preconditions: none
// Postconditions: tests methods of the Huffman class using randomly
//					generated character counts
int main() {
	 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	 _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	 _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	 _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	 _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);

	 // default constructor test;
	 Heap<HuffmanTree*> test_HP1;
	 HuffmanAlgorithm test_HA1;
	 HuffmanTree test_HT1;

	 Heap<HuffmanTree*> Test_HP2;

	 // COPY CONSTRUCTORS

	 // Assignment operators = = rhs


	// // Create random counts
	int counts[NUM_LETTERS];
	for (int i = 0; i < NUM_LETTERS; i++) {
		counts[i] = rand() % 1000;
		 //counts[i] = 0;
	}

	// Construct Huffman codes and display table
	HuffmanAlgorithm code(counts);
	cout << code << endl;

	//// Simple test of encoding words
	cout << "test:  " << code.getWord("Test!!!HELDhelidjlaksdf flkj") << endl;
	cout << "least: " << code.getWord("@@3498fjlvkjdjfao") << endl;
	cout << endl;

	//return 0;
}