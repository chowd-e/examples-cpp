#pragma once
//--------------------------------------------------------------------
// HuffmanTree.h
// Declaration of HuffmanTree class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-01-25
//--------------------------------------------------------------------
// HuffmanTree class: 
//	Description:
//  Builds a Binary tree consisting of Nodes as defined within 
//      the internal struct int the class. Trees can be merged to create
//      a new single tree for use in the creation of an encoded huffman
//      tree within a minHeap. Tree can be traversed to generate 
//      huffman codes - otherwise no client interaction other than comparison
//      construction and assignment
//--------------------------------------------------------------------

#include <iostream>
using namespace std;


class HuffmanTree {
private:

    struct Node {
        char ch;        // hold character data
        int count;     // frequency of occurance
        Node* left;     // left Node ptr
        Node* right;    // right Node ptr

        //------------------------------------------------------------------------
        // Default Constructor 
        // Preconditions: none
        // Postconditions: Node has been created with empty character data, and a 
        //      frequency of '0'
        Node() : ch(' '), count(0), left(nullptr), right(nullptr) {};

        //------------------------------------------------------------------------
        // Copy Constructor
        // Preconditions: none
        // Postconditions: creates and dynamically allocates memory for a new node
        //      equivalent to the 'ch' and 'count' of the argument
        Node(const Node* rhs);

        //------------------------------------------------------------------------
        // Assignment operator
        // Preconditions: none
        // Postconditions: lhs 'ch' and 'count' have been assigned equivalent values
        //      to that of the rhs
        const Node& operator=(const Node& rhs);
    };

    //--------------------------    MUTATORS    -----------------------------

    //------------------------------------------------------------------------
    // deleteNode
    // Preconditions: none
    // Postconditions: deletes the Huffman tree recursively in post-order
    //      No nodes remain after call
    void deleteNode(HuffmanTree::Node*& root);

    //------------------------------------------------------------------------
    // constructNode - create and dynamically allocate memory for a node
    // Preconditions: none
    // Postconditions: A node has been created with the corespoinding 'ch' and
    //          'counts' passed as args
    void constructNode(char ch, int count);

    //------------------------------------------------------------------------
    // copyTree - create a deep copy of a tree begining at rhsNode [Helper]
    // Preconditions: none
    // Postconditions: lhsNode has been created/dynamically allocated and 
    //      assigned to rhsNode. A recursive call
    //      continues down tree until reaching a nullptr
    void copyTree(Node*& lhsNode, Node* rhsNode);

    //-----------------------   ACCESSORS   -------------------------------

    //------------------------------------------------------------------------
    // findLeaf - recursively traverse tree to generate HuffmanCode
    // Preconditions: none
    // Postconditions: A string has been build in 'code' thorugh recursive calls,
    //      and once a leaf is reached, code assigned to codeBook corresponding to leafs 'ch'
    void findLeaf(string* codeBook, Node* root, string code) const;

    Node* root; // pointer to root node in tree

public:
    //------------------------------------------------------------------------
    // default constructor - constructs tree with root node dynamically allocated to dummy
    // Preconditions: none
    // Postconditions: constructs tree with root node dynamically allocate. 'ch' set to
    //          ' ' and count set to '0'
    HuffmanTree() ;

    //------------------------------------------------------------------------
    // constructor - merges two trees with shared parent node
    // Preconditions: cannot pass nullptrs
    // Postconditions: two separate trees are joined by parent node containing
    //          the sum of the two counts and 'ch' = ' '. memory handled of old
    //          trees within this method.
    HuffmanTree(HuffmanTree*& lhs, HuffmanTree*& rhs);

    //------------------------------------------------------------------------
    // constructor - creates a HuffmanTree with single root node as specified
    // Preconditions: none
    // Postconditions: tree has been created with single, dynamically allocate dnode
    //          with members 'ch' and 'count' set by parameters passed.
    HuffmanTree(char ch, int count);

    //------------------------------------------------------------------------
    // copy constructor - creates a HuffmanTree identical in structure and contents to rhs
    // Preconditions: none
    // Postconditions: new tree has been created through a deep copy, all nodes dynamically
    //      allocated. structure and contents are consistent with rhs tree. recursive call
    HuffmanTree(HuffmanTree& rhs); 

    //------------------------------------------------------------------------
    // Destructor 
    // Preconditions: none
    // Postconditions: Dynamic memory has been handled - all nodes deleted    
    ~HuffmanTree();

    //------------------------------------------------------------------------
    // Assignment operator - deep copy of tree, rhs returned 
    // Preconditions: none
    // Postconditions: new tree has been created through a deep copy, all nodes dynamically
    //      allocated. structure and contents are consistent with rhs tree. recursive call     
    const HuffmanTree& operator=(const HuffmanTree& rhs);

    //------------------------------------------------------------------------
    // Comparison overload [<] - compare root node
    // Preconditions: none
    // Postconditions: root nodes of both trees have been compared. If lhs has
    //      a smaller count than rhs, function will return true, if counts are
    //      equal, fxn will return the 'ch' with the lesser ASCII value, false otherwise
    bool operator<(const HuffmanTree& rhs) const;

    //------------------------------------------------------------------------
    // createCodes - recursive call to traverse tree for HuffmanCodes
    // Preconditions: cannot pass nullptr
    // Postconditions: a recursive call to traverse the tree will target leaf
    //      nodes and store the appropriate huffman codes in the array passed
    //      as an argument
    void createCodes(string* codeBook) const;
};