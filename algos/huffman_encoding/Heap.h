//--------------------------------------------------------------------
// HEAP.H
// Declaration and definition of the template Heap class
// Author: Olson + Howard (borrowing from a previous edition of Carrano/Henry)
//--------------------------------------------------------------------
// Heap class: 
//	Implements a priority queue with the following methods:
//		insert, deleteMin, findMin, heapify
//  The heap is stored in a vector (see the CSS 502 lecture notes)
//  Assumptions:
//	 Can only store data for which operator< exists
//	 (hence the Comparable type variable)
//--------------------------------------------------------------------

#pragma once
#include <vector>
#include <iostream>
using namespace std;

template <typename Comparable>
class Heap {
public:

	 //------------------------------------------------------------------------
	 // Heap() - initializes to heap with zero elements and empy vecot
	 // Preconditions: none
	 // Postconditions: Heap has been initialized, contains no elements, vector
	 //				  is empty with no dummy node
	 Heap(){
		  numElements = 0;
		  items.push_back(nullptr); //insert dummy node
	 }

	 // ------------------------------------------------------------------------
	 // Copy Constuctor - Builds new heap object by deep copy 
	 // Preconditions: rhs must not contain nullptrs
	 // Postconditions: new Heap created as a deep copy of the HEAP passed
	 Heap(Heap<Comparable>& rhs) {
		  numElements = 0;
		  items.push_back(nullptr); // push back dummy node
		  for (int i = 1; i <= rhs.size(); i++) { 
				Comparable* newItem = new Comparable; //allocate new Comparable
				*newItem = *rhs.items[i];		
				insert(newItem);
		  }
	 }

	 // ------------------------------------------------------------------------
	 // Constuctor - Builds new heap object based on array of Comparable ptrs
	 // Preconditions: none
	 // Postconditions: new Heap created and ordered based on priority determined
	 //			 by Comparable operator<(). Contains all Comparable pointers passed.
	 //			 nullptr is stored in items[0] at the dummy position
	 Heap(Comparable** array, int count) {
		  if (items.size() < 1) items.push_back(nullptr);
		  else items[0] = nullptr;	 // dummy node

		  numElements = count;
		  items.insert(items.end(), &array[0], &array[count]); // array -> vector
		  heapify();
	 }

	 // ------------------------------------------------------------------------
	 // Destructor - Deallocates all memory in heap
	 // Preconditions: none
	 // Postconditions: Heap is now empty, numElemnts = 0, vector still contains
	 //			 nullptr at item[0] location
	 ~Heap() {
		  clear();
	 }

	 // ------------------------------------------------------------------------
	 // Assignment operator - copy element of rhs Heap to lhs
	 // Preconditions: address of two objects cannot be equal, dynamic memory
	 //		allocation is handled in Comparable constructor
	 // Postconditions: Rhs Heap has been copied over to lhs, rhs is passed a
	 //			 return value
	 Heap<Comparable>& operator=(const Heap<Comparable>& rhs) {
		  if (this != &rhs) // test equivalency
		  {
				clear(); // clear LHS, leave dummy node
				if (items.size() < 1) items.push_back(nullptr);

				for (int i = 1; i <= rhs.size(); i++) { // deep cpy
					 // no dynamic allocation here - let Comparable take care of it
					 Comparable* newItem = new Comparable;
					 *newItem = *rhs.items[i]; // use comparables assignment operator
					 insert(newItem);
				}
		  }
		  return *this;
	 }

	//------------------------------------------------------------------------
	// insert - adds a single item to the heap
	// Preconditions: the initial heap has sufficient memory allocated
	//					and satisfies the heap order property
	// Postconditions: inserts the item into the queue, such that a new
	//					heap is created containing the union of the elements
	void insert(Comparable *c) {
		if (c == nullptr) return;

		// Add item in position 0 (dummy position) to prevent percolating up from root 
		if (items.size() < 1) items.push_back(c);
		else items[0] = c;

		// Ensure we have enough space
		numElements++;
		while ((int) items.size() <= numElements)
			items.push_back(nullptr);

		// Percolate up
		int position = numElements;
		while (*c < *items[position / 2]) {
			items[position] = items[position / 2];
			position = position / 2;
		}
		items[position] = c;
	}


	//------------------------------------------------------------------------
	// findMin - returns a const pointer to the minimum value in the heap
	// Preconditions: items[0] exists if numElements is larger than zero
	// Postconditions: Returns nullptr if the heap is empty, otherwise returns
	//	the item at the root of the heap (presumably the minimum priority).
	const Comparable * findMin() const {
		if (numElements <= 0) return nullptr;
		else return items[1];
	}


	//------------------------------------------------------------------------
	// deleteMin - delete the minimum and return a pointer to it
	// Preconditions: items[0] exists if numElements is larger than zero
	// Postconditions: Returns nullptr if the heap is empty, otherwise returns
	//  the item at the root of the heap (presumably the minimum priority).
	//  That element is deleted from the heap and the heap order property is
	//  restored.
	Comparable * deleteMin() {
		if (numElements == 0) return nullptr;

		// Give memory back to user
		Comparable *toReturn = items[1];
		items[1] = items[numElements];
		numElements--;

		percolateDown(1);
		return toReturn;
	}

	//------------------------------------------------------------------------
	// heapify - modifies any set of data into a heap
	// Preconditions: items has been allocated space for numElements pointers
	// Postconditions: The items stored form a heap
	void heapify() {
		for (int i = numElements / 2; i > 0; i--)
			percolateDown(i);
	}

	//------------------------------------------------------------------------
	// size
	// Preconditions: none
	// Postconditions: returns the size of the heap
	int size() const {
		return numElements;
	}

	//------------------------------------------------------------------------
	// isEmpty
	// Preconditions: none
	// Postconditions: returns whether the heap is empty (zero elements)
	bool isEmpty() const {
		return numElements == 0;
	}

private:

	//------------------------------------------------------------------------
	// percolateDown - used to restore the heap order property after deleteMin
	// Preconditions: Both subtrees of the root are heaps
	// Postconditions: Restores the heap order property by shifting the root
	//  down until it reaches an appropriate node
	void percolateDown(int position) {
		int child = position * 2;
		if (child > numElements) return;
		if (child != numElements && *items[child + 1] < *items[child]) child++;
		if (*items[child] < *items[position]) {
			swap(items[child], items[position]);
			percolateDown(child);
		}
	}

	// ------------------------------------------------------------------------
	 // clear - Empties and deallocates all memory in heap
	 // Preconditions: none
	 // Postconditions: Heap is now empty, numElemnts = 0, vector still contains
	 //			 nullptr at item[0] location
	void clear() {
		 while (!isEmpty())
		 {	 // Empty LHS
			  Comparable* temp = deleteMin();
			  delete temp;
			  temp = nullptr;
		 }
	}

	int numElements = 0;			// Number of elements in the heap
	vector <Comparable *> items;	// The elements in the heap
};