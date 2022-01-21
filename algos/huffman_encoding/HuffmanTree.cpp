//--------------------------------------------------------------------
// HuffmanTree.cpp
// Implemetnation of HuffmanTree class
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
#include "HuffmanTree.h"
#include "HuffmanAlgorithm.h"

using namespace std;

//------------------    NODE    --------------------

//------------------------------------------------------------------------
// Copy Constructor
// Preconditions: none
// Postconditions: creates and dynamically allocates memory for a new node
//      equivalent to the 'ch' and 'count' of the argument
HuffmanTree::Node::Node(const Node* rhs)
{
    *this = *rhs;
}

//------------------------------------------------------------------------
// Assignment operator
// Preconditions: none
// Postconditions: lhs 'ch' and 'count' have been assigned equivalent values
//      to that of the rhs
const HuffmanTree::Node& HuffmanTree::Node::operator=(const Node& rhs)
{
    if (this == nullptr || &rhs == nullptr) return rhs;
    this->ch = rhs.ch;
    this->count = rhs.count;
    return rhs;
}

//-----------------HUFFMAN TREE--------------------

// Constructors + Operator Overloads 

//------------------------------------------------------------------------
// default constructor - constructs tree with root node dynamically allocated to dummy
// Preconditions: none
// Postconditions: constructs tree with root node dynamically allocate. 'ch' set to
//          ' ' and count set to '0'
HuffmanTree::HuffmanTree() {
    constructNode(' ', 0);
}

//------------------------------------------------------------------------
// constructor - creates a HuffmanTree with single root node as specified
// Preconditions: none
// Postconditions: tree has been created with single, dynamically allocate dnode
//          with members 'ch' and 'count' set by parameters passed.
HuffmanTree::HuffmanTree(char ch, int count) {
    constructNode(ch, count);
}

//------------------------------------------------------------------------
// constructor - merges two trees with shared parent node
// Preconditions: cannot pass nullptrs
// Postconditions: two separate trees are joined by parent node containing
//          the sum of the two counts and 'ch' = ' '. memory handled of old
//          trees within this method.
HuffmanTree::HuffmanTree(HuffmanTree*& lhs, HuffmanTree*& rhs) {
    if (lhs == nullptr || rhs == nullptr || lhs->root == nullptr || rhs->root == nullptr) {
        root = nullptr;
        return;
    }
    constructNode(' ', lhs->root->count + rhs->root->count); // dynamically allocate node here
    root->left = lhs->root;   // hand over memory to new tree
    root->right = rhs->root;

    lhs->root = nullptr;     // disconnect roots from tree
    rhs->root = nullptr;
    delete lhs;              // delete old trees
    delete rhs;
    lhs = nullptr;
    rhs = nullptr;
}

//------------------------------------------------------------------------
// copy constructor - creates a HuffmanTree identical in structure and contents to rhs
// Preconditions: none
// Postconditions: new tree has been created through a deep copy, all nodes dynamically
//      allocated. structure and contents are consistent with rhs tree. recursive call
HuffmanTree::HuffmanTree(HuffmanTree& rhs)
{
    copyTree(root, rhs.root);
}

//------------------------------------------------------------------------
// Destructor 
// Preconditions: none
// Postconditions: Dynamic memory has been handled - all nodes deleted  
HuffmanTree::~HuffmanTree()
{
    deleteNode(root);
}

//------------------------------------------------------------------------
// Assignment operator - deep copy of tree, rhs returned 
// Preconditions: none
// Postconditions: new tree has been created through a deep copy, all nodes dynamically
//      allocated. structure and contents are consistent with rhs tree. recursive call
const HuffmanTree& HuffmanTree::operator=(const HuffmanTree& rhs)
{
    if (this != &rhs) {
        deleteNode(root); // clear out lhs
        copyTree(root, rhs.root); // copy rhs to lhs
    }
    return rhs;
}

//------------------------------------------------------------------------
// Comparison overload [<] - compare root node
// Preconditions: none
// Postconditions: root nodes of both trees have been compared. If lhs has
//      a smaller count than rhs, function will return true, if counts are
//      equal, fxn will return the 'ch' with the lesser ASCII value, false otherwise
//      nullptrs are considered less than any comparable value
bool HuffmanTree::operator<(const HuffmanTree& rhs) const
{
    if (&rhs == nullptr || rhs.root == nullptr) return false;
    if (this == nullptr || root == nullptr) return true;
    if (root->count == rhs.root->count) return root->ch < rhs.root->ch;
    return (root->count < rhs.root->count);
}



//-------------------      MUTATORS     -----------------------------------

//------------------------------------------------------------------------
// deleteNode
// Preconditions: none
// Postconditions: deletes the Huffman tree recursively in post-order
//      No nodes remain after call
void HuffmanTree::deleteNode(HuffmanTree::Node*& root) {
    if (root == nullptr) {
        return;
    }
    else if (root->left != nullptr && root->right != nullptr) {
        deleteNode(root->left);
        deleteNode(root->right);
    }
    else if (root->left != nullptr || root->right != nullptr) {
        if (root->left == nullptr) {
            deleteNode(root->right);
        }
        else {
            deleteNode(root->left);
        }
    }
    // deleting post order, hit node last
    root->left = nullptr;
    root->right = nullptr;
    delete root;
    root = nullptr;
    return;
}

//------------------------------------------------------------------------
// constructNode - create and dynamically allocate memory for a node
// Preconditions: none
// Postconditions: A node has been created with the corespoinding 'ch' and
//          'counts' passed as args
void HuffmanTree::constructNode(char ch, int count) {
    root = new Node;
    root->ch = ch;
    root->count = count;
}

//------------------------------------------------------------------------
// copyTree - create a deep copy of a tree begining at rhsNode [Helper]
// Preconditions: none
// Postconditions: lhsNode has been created/dynamically allocated and 
//      assigned to rhsNode. A recursive call
//      continues down tree until reaching a nullptr
void HuffmanTree::copyTree(HuffmanTree::Node*& lhsNode, HuffmanTree::Node* rhsNode) {
    if (rhsNode == nullptr) return;
    lhsNode = new Node; // dynamically allocate memory for copied node
    *lhsNode = *rhsNode; // assign to rhs
    copyTree(lhsNode->left, rhsNode->left);
    copyTree(lhsNode->right, rhsNode->right);
}

//------------------      ACCESSORS     --------------------------------

//------------------------------------------------------------------------
// createCodes - recursive call to traverse tree for HuffmanCodes
// Preconditions: cannot pass nullptr
// Postconditions: a recursive call to traverse the tree will target leaf
//      nodes and store the appropriate huffman codes in the array passed
//      as an argument
void HuffmanTree::createCodes(string* codeBook) const{
    if (this == nullptr || root == nullptr || codeBook == nullptr) return;
    string code = "";
    findLeaf(codeBook, root, code);
}

//------------------------------------------------------------------------
// findLeaf - recursively traverse tree to generate HuffmanCode
// Preconditions: none
// Postconditions: A string has been build in 'code' thorugh recursive calls,
//      and once a leaf is reached, code assigned to codeBook corresponding to leafs 'ch'
void HuffmanTree::findLeaf(string* codeBook, Node* root, string code)  const{
    if (root == nullptr) return;
    if (root->left == nullptr && root->right == nullptr) {
        // at leaf node, set codeBook entry
        codeBook[root->ch - 97] = code;
        return;
    }
    findLeaf(codeBook, root->left, code + "0");
    findLeaf(codeBook, root->right, code + "1");
    
    return;
}