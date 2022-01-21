#pragma once
//--------------------------------------------------------------------
// ComparableBST.h
// Header of ComparableBST class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-23
//--------------------------------------------------------------------
#include <iostream>

#include "Comparable.h"

using namespace std;

class ComparableBST {
private:
    struct Node {
        Comparable* comparablePtr;
        Node* left;
        Node* right;

        /**   Default / Convenience Constructor for Node with dynamic memory allocation for a Comparable
         * @param Comparable* Pointer to dynamically allocaed memory for a Comparable Object
         * @pre none
         * @post Node has been created along with dynamically allocated Comparable and count set to 1
         * @return none
         * Functions called:  none  */
        ComparableBST::Node(Comparable* myComp = nullptr);

        /**   Copy Constructor for Node with dynamic memory allocation for a Comparable
          * @param Node& address of Node object to copy
          * @pre none
          * @post deep copy made of argument Node created
          * @return none
          * Functions called:  operator=()  */
        ComparableBST::Node(const Node& rhs);

        /**   Overload output stream operator "<<" to direct Node contents to an output stream
         * @param ostream& output stream to redirect contents of Node to
         * @param Node Node which to output the Comparable and ""
         * @pre none
         * @post Node has not been modified and contents have been streamed to the specified output
         * @return ostream which Node contents have been output to
         * Functions called:  none*/
        friend ostream& operator<<(ostream& os, const ComparableBST::Node* myNode);

        /**   overload of the assignment operator creating deep copy of Node referenced in parameter
          * @param Node& address of Node object to copy
          * @pre none
          * @post deep copy made of argument Node created
          * @return none
          * Functions called:  none  */
        void operator=(const Node* rhs);

        /**   overload of the assignment operator creating deep copy of Node referenced in parameter
          * @param Node& address of Node object to copy
          * @pre none
          * @post deep copy made of argument Node created
          * @return none
          * Functions called:  none  */
        bool operator==(const Node* rhs) const;

        /**   Overload comparison operator "<" to compare Nodes myCusts, this case Comparable's ASCII vals
          * @param Node& right hand side comparison of Node
          * @pre none
          * @post Nodes have been compared and bool result returned indicating if lhs ASCII < rhs
          * @return bool indication of "Comparable" comparison between two nodes
          * Functions called:  none  */
        bool operator<(const Node* rhs) const;
    };

    Node* root; // root of BBST

public:

    //------------------         INITIALIZATION + OPERATORS            ------------------------

        //-------------------------------------------------------------------------------------
        //  Default Constructor for ComparableBST, Initialize to empty tree
        //  pre: none
        //  post: Tree has been initialized with root = nullptr, no dynamic memory allocated
    ComparableBST();

    //-------------------------------------------------------------------------------------
    //  Convenience Constructor for ComparableBST, Initialize to tree with singl enode
    //  pre: none
    //  post: Tree has been initialized with Comparable Node
    ComparableBST(Comparable* newComparable) { insert(newComparable); };

    /**   Copy Constructor for ComparableBST, deep copy
      * @param ComparableBST& to copy from
      * @pre none
      * @post Tree has been copied with additional dynamic memory allocated and new pointers
      * @return none
      * Functions called:  operator=()  */
    ComparableBST(const ComparableBST& rhs);

    /**   Destructor, de-allocate all dynamic memory
      * @param none
      * @pre none
      * @post ComparableBST is deleted and memory is freed
      * @return none
      * Functions called:  makeEmpty() */
    ~ComparableBST();

    /**   Overload of assignment operator, creating deep copy
      * @param none
      * @pre none
      * @post Tree has been copied with additional dynamic memory allocated and new pointers
      * @return reference to right hand side tree passed
      * Functions called:  copyTree()  */
    const ComparableBST* operator=(const ComparableBST* rhs);

    bool operator==(const ComparableBST* rhs) const;


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
    friend ostream& operator<<(ostream& os, const ComparableBST* myTree);

    /**   Overload output stream operator "<<" to direct Node contents to an output stream
     * @param ostream& output stream to redirect contents of Node to
     * @param Node Node which to output the Comparable and ""
     * @pre none
     * @post Node has not been modified and contents have been streamed to the specified output
     * @return ostream which Node contents have been output to
     * Functions called:  none*/
    friend ostream& operator<<(ostream& os, const ComparableBST::Node* myNode);

    //---------------------          ACCESSORS           ----------------------------

    /**   Helper method for recursively printing contents of BST - hands off
     *       root member variable
     * @param none
     * @pre none
     * @post Helper call begun recursive count of BST Nodes
     * @return ostream Output stream of data
     * Functions called:  print(ostream&, root) */
    ostream& print(ostream& os) const;

    /**   Recursive call to stream contents of BST
      * @param os Output stream which contents will be streamed to
      * @param root Pointer to root of BST
      * @pre none
      * @post Method has traversed entire tree inorder, and streamed the
      *    contents to the output stream
      * @return ostream Data returned represents content of BST and may be
      *    handled by user as needed */
    ostream& print(ostream& os, const Node* root) const;

    /**   Recursive call to compare Node contents as tree is traversed
     * @param Node* to lhs Node for comparison
     * @param Node* to rhs Node for comparison
     * @pre none
     * @post both trees have been traversed, boolean value returned indicating equality as defined
     * @return boolean value indicating equivalent contents of each node in a tree and the same structure
     * Functions called:  compareBSTs()  */
    bool compareBSTs(const ComparableBST::Node* lhs, const ComparableBST::Node* rhs) const;


    //------------------------          MUTATORS           ---------------------------------

              //---------------------------------------------------------------------------------------------
       //   Helper method for recursive call of insert - hands off root
       //  pre: none
       //  post: Recursive call has been made to insert the Comparable argument into the BST
       //          and returns boolean indicating if a new node was created (true) or count
       //          was incremented on existing node (false)
    bool insert(Comparable* newComp);

    /**   Helper method for makeEmpty recursive call to clear BST, includes passing of root
      * @param none
      * @pre none
      * @post passed root to recursive call to clear contents of tree
      * @return none
      * Functions called:  makeEmpty(root) */
    void makeEmpty();

    /**   Helper method for recursive call of remove() to remove a single Comparable occurance
      * @param Comparable* pointer to myComp being removed
      * @pre none
      * @post Recursive call has been made to remove the Comparable argument in the BST
      * @return boolean indicating if a new node was removed (true) or if it wasn't found (false)
      * Functions called:  retrieve(myComp), remove(myComp, root)) */
    bool remove(Comparable* myComp);

    /**   Retrieves and returns pointer to a Comparable equivalent to the Comparable passed as an arg
 * @param Comparable& to be searched for within the calling ComparableBST
 * @pre none
 * @post ComparableBST has been traversed and pointer to a Comparable returned if found, nullptr if not
 * @return Comparable* indicating found comparablePtr in ComparableBST, nullptr returned if not contained within tree
 * Functions called:  retrieve */
    Comparable* retrieve(Comparable* comparablePtr) const;


    /**   Recursive call to retrieves and returns pointer to a Node containing an
    *       equivalent Comparable passed as an arg
     * @param Comparable& to be searched for within the calling ComparableBST
     * @param Node* Pointer to Node inidicating position within tree througout traversal
     * @pre none
     * @post ComparableBST has been traversed and pointer to a Node returned if found, nullptr if not
     * @return Node* indicating found equavalent Comparable in ComparableBST,
     *      nullptr returned if not contained within tree
     * Functions called:  retrieve */
    Node* retrieve(Comparable* comparablePtr, ComparableBST::Node* root) const;

    /**   Recursive call to remove a single occurance of Comparable within the ComparableBST
     * @param Comparable* pointer to myComp being searched for and removed
     * @pre none
     * @post an instance of Comparable has been removed from the ComparableBST, either decremented count
     *      or removed a node and reconfigured the structure to maintain a proper BST
     * @return Node* indicating the value to assign to the parent node in the recursive call
     * Functions called:  remove(myComp, root)), findLeftMost(root) */
    Node* remove(Comparable* myComp, Node*& root);

    /**   Recursive call to traverse as far left down a subtree as possible returning the leaf node
     * @param Node*& Node by which to begin the traversal, current position in tree during recursive call
     * @pre none
     * @post Traversed the furthest left down an identified subtree and returned the leaf node
     * @return Node* indicating left most leaf Node of subTree
     * Functions called:  findLeftMost(root)    */
    ComparableBST::Node*& findLeftMost(ComparableBST::Node*& root);

    /**   Helper method for determining the depth of a Comparable in a ComparableBST
 * @param Comparable& comparablePtr wishing to locate within a tree to count depth
 * @pre none
 * @post Helper call begun recursive call to locate Comparabel
 * @return int indicating the depth of the Comparable
 * Functions called:  retrieve(comparablePtr, root), depth(comparablePtr, root)*/
    int depth(Comparable* comparablePtr) const;

    /**   Recursive call to traverse from root to the desired node, counting the depth from root
 * @param Comparable& comparablePtr which you are attempting to ifnd / access the depth information on
 * @param Node* currentl location in tree traversal during recursive call
 * @pre none
 * @post Traversed down to the desired node, or end of the tree to find the Comparable of interest
 * @return int indicating the depth of the comparablePtr passed as an arg, or -1 if the Comparable wasn't found
 * Functions called:  depth(comparablePtr, root)    */
    int depth(Comparable* comparablePtr, Node* root) const;

    /**   Helper method for recursively determining the number of nodes below an initial node (descendants)
     * @param Comparable& comparablePtr wishing to locate within a tree to count descendants
     * @pre none
     * @post Helper call begun recursive count of BST Nodes below startign location
     * @return int indicating the number of descendants of the initial argument
     * Functions called:  retrieve(comparablePtr, root), descendants(root)*/
    int descendants(Comparable* comparablePtr) const;

    /**   Recursive call to traverse from a starting node to count the number of descendants
     * @param Node* starting / current location in tree traversal during recursive call
     * @pre none
     * @post Traversed down the tree and counted the nodes visited
     * @return int indicating the number of descendants of the initial argument
     * Functions called:  descendants(comparablePtr, root)    */
    int descendants(Node* node) const;

    /**   Recursive call to copy contents of a tree (rhsNode) into another (lhsNode)
     * @param lhsNode Node in which Comparable and count is copied into
     * @param rhsNode Node in which Comparable and count is copied from
     * @pre none
     * @post Tree has recursively been copied with additional dynamic memory allocated and new pointers
     * @return reference to new tree copied
     * Functions called:  copyTree()  */
    const ComparableBST& copyTree(ComparableBST::Node*& lhsNodePtr, const ComparableBST::Node* rhsNodePtr);

    /**   Recursive call to delete all nodes within a BST
     * @param root referenced Pointer to Node
     * @pre none
     * @post BST has been deleted (post-order) with all dynamic memory handled
     * @return none
     * Functions called:  makeEmpty(root) */
    void makeEmpty(Node*& root);

    /**   Adds a new Node to the BST in the appropriate location determined by contents of Node->myComp
     * @param root Pointer to BST Node by which to compare
     * @param newNode pointer reference to node being added
     * @pre none
     * @post Recursive call has inserted the newNode into the BST, handling any required dynamic mem
     * @return bool indicating if a new node was created (true) or count was incremented on existing node (false)
     * Functions called:  insert(root, newNode) */
    bool insert(Comparable* newCust, Node*& root);
};