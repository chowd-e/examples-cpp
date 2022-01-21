#pragma once

/** @file ComparableBST.cpp
 * UWB CSS 502 Data Structures and Algorithms
 * Program 1 - Improved Binary Search Tree
 * @author Chase Howard
 * @date 2021-01-10
 * Description: This contains the implemetnation of the ComparableBST class and
 *       functions for the nested Node structure defined in the .h file.
 */

#include "ComparableBST.h"
//#include "Comparable.h"


 //-------------------------------------------------------------------------
 //-------------------           NODE         -------------------------------
 //---------------------------------------------------------------------------

 /**   Default / Convenience Constructor for Node with dynamic memory allocation for a Comparable
  * @param Comparable* Pointer to dynamically allocaed memory for a Comparable Object
  * @pre none
  * @post Node has been created along with dynamically allocated Comparable and count set to 1
  * @return none
  * Functions called:  none  */
ComparableBST::Node::Node(Comparable* newComparable) : comparablePtr(newComparable), left(nullptr), right(nullptr) {};

/**   Copy Constructor for Node with dynamic memory allocation for a Comparable
 * @param Node& address of Node object to copy
 * @pre none
 * @post deep copy made of argument Node created
 * @return none
 * Functions called:  operator=()  */
ComparableBST::Node::Node(const Node& rhs)
{
    *this = rhs;
}

/**   overload of the assignment operator creating deep copy of Node referenced in parameter
 * @param Node& address of Node object to copy
 * @pre none
 * @post deep copy made of argument Node created
 * @return none
 * Functions called:  none  */
void ComparableBST::Node::operator=(const Node* rhs)
{
    if (!(*this == rhs))               // no self assignment
    {
        this->comparablePtr = rhs->comparablePtr;
        this->left = rhs->left;      // do I want to do this?
        this->right = rhs->right;
    }

}


/**   Overload output stream operator "<<" to direct Node contents to an output stream
 * @param ostream& output stream to redirect contents of Node to
 * @param Node Node which to output the Comparable and ""
 * @pre none
 * @post Node has not been modified and contents have been streamed to the specified output
 * @return ostream which Node contents have been output to
 * Functions called:  none*/
 ostream& operator<<(ostream& os, const ComparableBST::Node* myNode)
 {
     myNode->comparablePtr->print(os);
     return os;
 }

 /**   Overload comparison operator "<" to compare Nodes Comparables, this case Comparable's ASCII vals
  * @param Node& right hand side comparison of Node
  * @pre none
  * @post Nodes have been compared and bool result returned indicating if lhs ASCII < rhs
  * @return bool indication of "Comparable" comparison between two nodes
  * Functions called:  none  */
bool ComparableBST::Node::operator<(const Node* rhs) const
{
    return *(this->comparablePtr) < *rhs->comparablePtr;
}


/**   Overload equality operator "==" to compare two Nodes contents (Comparable and Count)
 * @param Node& right hand side comparison of Node
 * @pre none
 * @post Nodes have been compared and bool result returned indicating equality
 * @return bool indication of Node equality
 * Functions called:  none  */
bool ComparableBST::Node::operator==(const Node* rhs) const
{
    return *(this->comparablePtr) == *rhs->comparablePtr;
}


/*-----------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------           ComparableBST         ------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------*/

///////////////////////          INITIALIZATION + OPERATORS            \\\\\\\\\\\\\\\\\\\\\\\\\\\\

/**   Default Constructor for ComparableBST, Initialize to empty tree
 * @param none
 * @pre none
 * @post Tree has been initialized with root = nullptr, no dynamic memory allocated
 * @return none
 * Functions called:  none  */
ComparableBST::ComparableBST() : root(nullptr) {};

/**   Copy Constructor for ComparableBST, deep copy
 * @param ComparableBST& to copy from
 * @pre none
 * @post Tree has been copied with additional dynamic memory allocated and new pointers
 * @return none
 * Functions called:  operator=()  */
ComparableBST::ComparableBST(const ComparableBST& rhs)
{
    *this = rhs;
}

/**   Destructor, de-allocate all dynamic memory
 * @param none
 * @pre none
 * @post ComparableBST is deleted and memory is freed
 * @return none
 * Functions called:  makeEmpty() */
ComparableBST::~ComparableBST()
{
    makeEmpty();
}

/**   Overload of assignment operator, creating deep copy
 * @param none
 * @pre none
 * @post Tree has been copied with additional dynamic memory allocated and new pointers
 * @return reference to right hand side tree passed
 * Functions called:  copyTree()  */
const ComparableBST* ComparableBST::operator=(const ComparableBST* rhs)
{
    if (!(*this == rhs))
    {
        makeEmpty();    // clear before copying over

        if (rhs->root != nullptr)
        {
            this->root = new Node;
            *this->root = rhs->root;    // count initialized as 1
            copyTree(this->root, rhs->root);
        }
    }

    return rhs;
}

/**   Overload Equality operator for ComparableBST, compared contents and tree structure
 * @param rhs tree to test equality against
 * @pre none
 * @post boolean value returned indicating equality as defined, no change to either tree
 * @return boolean value indicating equivalent contents of each node in a tree and the same structure
 * Functions called:  compareBSTs()  */
bool ComparableBST::operator==(const ComparableBST* rhs) const
{
    return compareBSTs(this->root, rhs->root);
}


/**   Method for streaming contents of ComparableBST. Will traverse contents
 *    of tree 'inorder' while streaming Comparable and counts of each node
 *    to the desired output
 * @param ostream&   Stream to output contents of tree to
 * @param ComparableBST& BST containing desired info to output
 * @pre none
 * @post Has output the entirety of the BST to the identified stream in order as defined
 *      by the Comparable object with a count of the number of occurances
 * @return ostream Output stream of informatoin
 * Functions called:  print(ostream) */
ostream& operator<<(ostream& os, const ComparableBST& myTree)
{
    return myTree.print(os);
}

//////////////////////////////////          ACCESSORS           \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/**   Recursive call to compare Node contents as tree is traversed
 * @param Node* to lhs Node for comparison
 * @param Node* to rhs Node for comparison
 * @pre none
 * @post both trees have been traversed, boolean value returned indicating equality as defined
 * @return boolean value indicating equivalent contents of each node in a tree and the same structure
 * Functions called:  compareBSTs()  */
bool ComparableBST::compareBSTs(const ComparableBST::Node* lhs, const ComparableBST::Node* rhs) const
{
    if (lhs == nullptr || rhs == nullptr)   // one nullptr, test for 2 indicating leaf
        return (lhs == nullptr && rhs == nullptr); // so zen
    else if (!(lhs == rhs))                  // only verifies Comparable and Count, not pointers
        return false;
    else
        return compareBSTs(lhs->left, rhs->left) && compareBSTs(lhs->right, rhs->right);
}

/**   Retrieves and returns pointer to a Comparable equivalent to the Comparable passed as an arg
 * @param Comparable& to be searched for within the calling ComparableBST
 * @pre none
 * @post ComparableBST has been traversed and pointer to a Comparable returned if found, nullptr if not
 * @return Comparable* indicating found comparablePtr in ComparableBST, nullptr returned if not contained within tree
 * Functions called:  retrieve */
Comparable* ComparableBST::retrieve(Comparable* comparablePtr) const
{
    Node* temp = retrieve(comparablePtr, root);
    return (temp == nullptr ? nullptr : temp->comparablePtr);
}

/**   Recursive call to retrieves and returns pointer to a Node containing an
*       equivalent Comparable passed as an arg
 * @param Comparable& to be searched for within the calling ComparableBST
 * @param Node* Pointer to Node inidicating position within tree througout traversal
 * @pre none
 * @post ComparableBST has been traversed and pointer to a Node returned if found, nullptr if not
 * @return Node* indicating found equavalent Comparable in ComparableBST,
 *      nullptr returned if not contained within tree
 * Functions called:  retrieve */
ComparableBST::Node* ComparableBST::retrieve(Comparable* comparablePtr, ComparableBST::Node* root) const
{
    if (root == nullptr)
        return root;                // return nullptr
    else if (*(root->comparablePtr) == *comparablePtr)
    {
        Node* found = root;
        return found;
    }
    else
        return (*comparablePtr < *root->comparablePtr ? retrieve(comparablePtr, root->left) : retrieve(comparablePtr, root->right));
}

/**   Recursive call to traverse as far left down a subtree as possible returning the leaf node
 * @param Node*& Node by which to begin the traversal, current position in tree during recursive call
 * @pre none
 * @post Traversed the furthest left down an identified subtree and returned the leaf node
 * @return Node* indicating left most leaf Node of subTree
 * Functions called:  findLeftMost(root)    */
ComparableBST::Node*& ComparableBST::findLeftMost(ComparableBST::Node*& root)
{
    return (root->left == nullptr ? root : findLeftMost(root->left));
}

/**   Helper method for determining the depth of a Comparable in a ComparableBST
 * @param Comparable& comparablePtr wishing to locate within a tree to count depth
 * @pre none
 * @post Helper call begun recursive call to locate Comparabel
 * @return int indicating the depth of the Comparable
 * Functions called:  retrieve(comparablePtr, root), depth(comparablePtr, root)*/
int ComparableBST::depth(Comparable* comparablePtr) const
{
    return (retrieve(comparablePtr) == nullptr ? -1 : depth(comparablePtr, root));
}

/**   Recursive call to traverse from root to the desired node, counting the depth from root
 * @param Comparable& comparablePtr which you are attempting to ifnd / access the depth information on
 * @param Node* currentl location in tree traversal during recursive call
 * @pre none
 * @post Traversed down to the desired node, or end of the tree to find the Comparable of interest
 * @return int indicating the depth of the comparablePtr passed as an arg, or -1 if the Comparable wasn't found
 * Functions called:  depth(comparablePtr, root)    */
int ComparableBST::depth(Comparable* comparablePtr, ComparableBST::Node* root) const
{
    if (root == nullptr)
        return -1;
    else if (*(root->comparablePtr) == *comparablePtr)
        return 0;
    else
    {
        int count = 0;
        count += 1 + (*comparablePtr < *root->comparablePtr ? depth(comparablePtr, root->left) : depth(comparablePtr, root->right));
        return count;
    }
}

/**   Helper method for recursively determining the number of nodes below an initial node (descendants)
 * @param Comparable& comparablePtr wishing to locate within a tree to count descendants
 * @pre none
 * @post Helper call begun recursive count of BST Nodes below startign location
 * @return int indicating the number of descendants of the initial argument
 * Functions called:  retrieve(comparablePtr, root), descendants(root)*/
int ComparableBST::descendants(Comparable* comparablePtr) const
{
    Node* startNode = retrieve(comparablePtr, root);
    // decrement descendants as the count is inclusive of root
    return (startNode == nullptr ? -1 : descendants(startNode) - 1);
}

/**   Recursive call to traverse from a starting node to count the number of descendants
 * @param Node* starting / current location in tree traversal during recursive call
 * @pre none
 * @post Traversed down the tree and counted the nodes visited
 * @return int indicating the number of descendants of the initial argument
 * Functions called:  descendants(comparablePtr, root)    */
int ComparableBST::descendants(Node* node) const
{
    if (node != nullptr)
    {   //traverse in pre order and count num of nodes
        int count = 1;                      // visit node first, count if not nullptr
        count += descendants(node->left);
        count += descendants(node->right);
        return count;
    }
    else
        return 0;                           // end of a branch, bring it back now yall
}

/**   Helper method for recursively printing contents of BST - hands off
 *       root member variable
 * @param none
 * @pre none
 * @post Helper call begun recursive count of BST Nodes
 * @return ostream Output stream of data
 * Functions called:  print(ostream&, root) */
ostream& ComparableBST::print(ostream& os) const
{
    return print(os, root);         // helper method to pass root
}

/**   Recursive call to stream contents of BST
 * @param os Output stream which contents will be streamed to
 * @param root Pointer to root of BST
 * @pre none
 * @post Method has traversed entire tree inorder, and streamed the
 *    contents to the output stream
 * @return ostream Data returned represents content of BST and may be
 *    handled by user as needed */
ostream& ComparableBST::print(ostream& os, const Node* root) const
{
    if (root != nullptr)
    {
        print(os, root->left);
        os << root->comparablePtr;            // printing in order - hit root before branching right
        print(os, root->right);
    }
    return os;
}


//////////////////////////////////           MUTATORS           \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/**   Recursive call to copy contents of a tree (rhsNode) into another (lhsNode)
 * @param lhsNode Node in which Comparable and count is copied into
 * @param rhsNode Node in which Comparable and count is copied from
 * @pre none
 * @post Tree has recursively been copied with additional dynamic memory allocated and new pointers
 * @return reference to new tree copied
 * Functions called:  copyTree()  */
const ComparableBST& ComparableBST::copyTree(ComparableBST::Node*& lhsNode, const ComparableBST::Node* rhsNode)
{
    if (rhsNode->left == nullptr || rhsNode->right == nullptr) {
        //one of them is null - find out which
        if (rhsNode->left == nullptr && rhsNode->right == nullptr)
            return *this;                         // both nullptr, reached a leaf
        else if (rhsNode->left == nullptr)
        {    // left null, traverse right
            lhsNode->right = new Node;                      //  allocate 
            *lhsNode->right = *rhsNode->right;              // assign
            copyTree(lhsNode->right, rhsNode->right);       // copy right
        }
        else
        {   // right side null, traverse left
            lhsNode->left = new Node;               // allocate
            *lhsNode->left = *rhsNode->left;        // assign 
            copyTree(lhsNode->left, rhsNode->left); // continue copy left
        }
    }
    else
    {   // Neither left nor right are null - copy both
        lhsNode->left = new Node;               // allocate
        *lhsNode->left = *rhsNode->left;        // assign 
        copyTree(lhsNode->left, rhsNode->left); //copy lhs
        // right branch next
        lhsNode->right = new Node;                  // allocate 
        *lhsNode->right = *rhsNode->right;          // assign
        copyTree(lhsNode->right, rhsNode->right);   // copy rhs
    }

    return *this;
}

/**   Helper method for recursive call of remove() to remove a single Comparable occurance
 * @param Comparable* pointer to comparablePtr being removed
 * @pre none
 * @post Recursive call has been made to remove the Comparable argument in the BST
 * @return boolean indicating if a new node was removed (true) or if it wasn't found (false)
 * Functions called:  retrieve(comparablePtr), remove(comparablePtr, root)) */
bool ComparableBST::remove(Comparable* comparablePtr)
{
    if (retrieve(comparablePtr) == nullptr)
        return false;
    root = remove(comparablePtr, root);      // recursively assign return vals from remove call, end on root
    return true;
}

/**   Recursive call to remove a single occurance of Comparable within the ComparableBST
 * @param Comparable* pointer to comparablePtr being searched for and removed
 * @pre none
 * @post an instance of Comparable has been removed from the ComparableBST, either decremented count
 *      or removed a node and reconfigured the structure to maintain a proper BST
 * @return Node* indicating the value to assign to the parent node in the recursive call
 * Functions called:  remove(comparablePtr, root)), findLeftMost(root) */
ComparableBST::Node* ComparableBST::remove(Comparable* comparablePtr, ComparableBST::Node*& root)
{
    if (root == nullptr)
        return nullptr; // not found in tree
    else if (*comparablePtr < *root->comparablePtr)
        root->left = remove(comparablePtr, root->left);
    else if (!(*comparablePtr < *root->comparablePtr) && (!(*comparablePtr == *root->comparablePtr)))
        root->right = remove(comparablePtr, root->right);
    else
    {   // Comparables are equivalent, this is the node to remove
        if (root->comparablePtr->getCount() > 1) // decrement or remove node
            root->comparablePtr->decrement();
        // determine how to properly remove node
        if (root->left == nullptr && root->right == nullptr)
        {   // leaf node, just delete
            root->comparablePtr->remove();
            root->comparablePtr = nullptr;
            delete root;
            root = nullptr; // set parent to nullptr;
        }
        else if (root->left == nullptr || root->right == nullptr)
        {   // one is nullptr, pull in other
            Node* temp = root;
            root = root->left == nullptr ? root->right : root->left;
            temp->comparablePtr->remove();
            temp->comparablePtr = nullptr;
        }
        else
        {
            Node* tempNode = findLeftMost(root->right); // left most leaf in right branch
            *(root->comparablePtr) = *tempNode->comparablePtr;              // assign leftmost value / count to deleted location
            remove(comparablePtr, tempNode);                     // remove leaf 
        }
    }
    return root;
}

/**   Helper method for recursive call of insert - hands off root
 * @param Comparable* pointer to comparablePtr being added
 * @pre none
 * @post Recursive call has been made to insert the Comparable argument into the BST
 * @return boolean indicating if a new node was created (true) or count was incremented on existing node (false)
 * Functions called:  insert(root, newComparable)) */
bool ComparableBST::insert(Comparable* newComparable)
{
    return insert(newComparable, this->root);
}

/**   Adds a new Node to the BST in the appropriate location determined by contents of Node->comparablePtr
 * @param root Pointer to BST Node by which to compare
 * @param newNode pointer reference to node being added
 * @pre none
 * @post Recursive call has inserted the newNode into the BST, handling any required dynamic mem
 * @return bool indicating if a new node was created (true) or count was incremented on existing node (false)
 * Functions called:  insert(root, newNode) */
bool ComparableBST::insert(Comparable* newComparable, Node*& root)
{
    if (root == nullptr)
    {   // settle down here
        Node* newNode = new Node(newComparable);
        newComparable = nullptr;
        root = newNode; // set new root
        return true;
    }
    else if (*(root->comparablePtr) == *newComparable)
    {   // found the same word, no duplicate Comparables
        root->comparablePtr->increment(newComparable->getCount());
        return false;       // memory handling of Comparable not handled by tree
    }
    else
        return (*newComparable < *root->comparablePtr ? insert(newComparable, root->left) : insert(newComparable, root->right));
}

/**   Helper method for makeEmpty recursive call to clear BST, includes passing of root
 * @param none
 * @pre none
 * @post passed root to recursive call to clear contents of tree
 * @return none
 * Functions called:  makeEmpty(root) */
void ComparableBST::makeEmpty()
{
    makeEmpty(root);
}

/**   Recursive call to delete all nodes within a BST
 * @param root referenced Pointer to Node
 * @pre none
 * @post BST has been deleted (post-order) with all dynamic memory handled
 * @return none
 * Functions called:  makeEmpty(root) */
void ComparableBST::makeEmpty(Node*& root)
{
    // case for both pointer pointing to a node
    if (root == nullptr) return;

    makeEmpty(root->left);
    makeEmpty(root->right);

    // deleting post order, hit node last
    root->comparablePtr->remove();
    //delete root->comparablePtr;
    root->comparablePtr = nullptr;
    delete root;
    root = nullptr;
}