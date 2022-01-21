//--------------------------------------------------------------------
// Graph.h
// Header of Graph class
// Author: Chase Howard
// Class: CSS 502B [University of Washington Bothell]
// Date: 2021-02-11
//--------------------------------------------------------------------
// Graph class: 
//	Description:
//		  This program will accept an input file to build a graph of 
//		  connected verteces with an associateddistance or 'weight' between them. 
//		  Graph will calculate and display the shortest path from each 
//		  vertex to all other vertices in which a path is possible. This 
//		  information is output to cout and may be redirected by the client
//		  as needed.
//		  This class contains a number of internal structures to assist in the
//		  processing, including: EdgeNode, VertexNode, Table, and HeapFood.

//		  EdgeNode : Indicates and Edge between two vertices with an associated weight 
//		  VertexNode : Node inidcating name of vertex and associated list of edges
//		  Table : For use in storing infomration resulting from the execution of Dijsktras 
//					 Algorithm
//		  HeapFood : For insertion to the priority Q used within Dijsktras Algorith.
//					 used for storing vertex indices with an associated wieght to determine
//					 minimum value to remove from priority Q.

// Input:
//		  Accepts input from a formatted text file as as follows. The first 
//		  line will contain the number of vertices n. Following are text 
//		  descriptions of the vertices 1 through n (50 chars max length). 
//		  After that, each line consists of 3 ints representing an edge. 
//		  If there is a directed edge from vertex 1 to vertex 2 with a weight 
//		  of 10, the line will be: 1 2 10. A zero for the first integer signals 
//		  the end of the data. This is a valid example: 

/*
	 4
	 Olson’s office
	 Stiber's office
	 STEM office
	 The Commons
	 1 2 10
	 1 3  5
	 2 4 10
	 2 1 15
	 3 1  5
	 3 4 20
	 0 0  0
*/

//	Output:
//		  Example output will from the input above will be formatted as follows:

/*
	 Description                  From  To    Dist  Path
	 Olson’s office
					 1     2     10    1 2
					 1     3	    5     1 3
					 1	    4	    20    1 2 4
	 Stiber's office
					 2	    1	    15    2 1
					 2     3	    20    2 1 3
					 2	    4     10    2 4
	 STEM office
					 3	   1		 5     3 1
					 3	   2		 15    3 1 2
					 3	   4		 20    3 4
	 The Commons
					 4	   1		 --
					 4	   2		 --
					 4	   3		 --
*/

// Assumptions:
//		  Assumes all input is formatted properly
//		  Can only accept MAX_VERTICES number of vertices 100
//		  Assumes findShortestpath() is called anytime a Edge is
//				inserted or removed
//--------------------------------------------------------------------

#include <limits>
#include <vector>
#include "Vertex.h"

class Graph {

private:
	 static const int MAX_VERTICES = 101;

	 //--------------------------------------------------------------------------
	 //   EdgeNode : Indicates and Edge between two vertices. Included in an 
	 //		adjacency list from each vertex. EdgeNodes contain a weight or
	 //		cost for travel, an int indicating adjacent vertex index within
	 //		member variable 'vertices', and a pointer to another edge from
	 //		the same vertex
	 struct EdgeNode {			
		  int adjVertex = INT_MAX;			// subscript of the adjacent vertex 
		  int weight = INT_MAX;				// weight of edge
		  EdgeNode* nextEdge = nullptr;
	 };

	 //--------------------------------------------------------------------------
	 //   VertexNode : Structure for storing Vertex Name and pointer to adjacency 
	 //				  list for all adjacent edges [EdgeNodes]
	 struct VertexNode {
		  EdgeNode* edgeHead = nullptr;		// head of the list of edges
		  Vertex* data = nullptr;		// store vertex data here
	 };

	

	 //--------------------------------------------------------------------------
	 //   Table : Structure for storing path information, to be filled during
	 //		call to findShortestPath() and used to display information for printing.
	 struct Table {
		  bool visited = false;			// whether vertex has been visited
		  int dist = INT_MAX;				// shortest known distance from source
		  int path = INT_MAX;				// previous vertex in path of min dist
	 };

	 //--------------------------------------------------------------------------
	//   HeapFood : Structure for storing information needed in the formation and 
	 //			 access of minHeap during population of Table T during findShortestPath()
	 struct HeapFood {
		  HeapFood(int tableCol = 0, int weight = 0) : tableCol(tableCol), weight(weight) {};
		  int tableCol; // index of vertices in Table T relating to column being accessed
		  int weight; // distance from source vertex [column 1 in same row of tableCol]

		  //------------------------------------------------------------------------
		  // Overload HeapFood operator<()
		  // Preconditions: None
		  // Postconditions: returns boolean indicating which object is less indicated
		  //		  by the weight member variable in each object. For use by Priority Q
		  bool operator<(const HeapFood& rhs) const;
	 };

	 int size = 0;	// number of vertices in the graph

	 VertexNode vertices[MAX_VERTICES]; // storage for vertices

	 Table T[MAX_VERTICES][MAX_VERTICES]; // storage of path information

	 //------------------------  PRIVATE ACCESSORS   ---------------------------

	 //------------------------------------------------------------------------
	 // validInput()
	 // Preconditions: None
	 // Postconditions: returns boolean indicating if arguments src and dest are
	 //		  positive values less than the defined MAX_VERTICES, src and dest are not
	 //		  indicating a self-edge, and the vertex at src has been initialized
	 bool validInput(int src, int dest) const;
	 
	 //------------------------------------------------------------------------
	 // printVertex() 
	 // Preconditions: must contain valid src and dest values as indicated by validInput(),
	 //				must have called findShortestPath() prior to displaying information
	 // Postconditions: Infomration regarding shortes path from src to dest has been 
	 //				displayed to the console. Indices of vertices along the path has filled
	 //				the vector designated as 'locs' within the parameters to be handled by
	 //				calling function if desired
	 bool printVertex(int src, int dest, vector<int>& locs) const;

	 //------------------------------------------------------------------------
	 // printPath() 
	 // Preconditions: 
	 // Postconditions: Information regarding the vertices visited along the path
	 //		  from src to dest has been returned in a string. If there is no valid
	 //		  path from src to dest, this method returns an empty string
	 string printPath(int src, int dest, vector<int>& locs) const;

	 //------------------------  PRIVATE MUTATORS   ----------------------------

	 //------------------------------------------------------------------------
	 // clearGraph() 
	 // Preconditions: None
	 // Postconditions: All vertices and associated edges have been deleted, 
	 //		  Table data has been reset to default and size set to zero.
	 void clearGraph();

	 //------------------------------------------------------------------------
	 // copyEdges()
	 // Preconditions: rhs EdgeNode != nullptr
	 // Postconditions: iterates through all edgeNodes in a separate Graph, copying
	 //		  and inserting the edges into the calling graph. Requires user to call
	 //		  findShortestPath() if desired after call, this method does not update
	 //		  Table T after calling.
	 void copyEdges(int src, const EdgeNode* rhsEdge);

	 //------------------------------------------------------------------------
	 // clearEdges()
	 // Preconditions: Vertex mustn't have nullptr at edgeHead
	 // Postconditions: Starting at the argument passed for head, all EdgeNodes
	 //		  after have been removed with dynamical memory handled
	 void clearEdges(EdgeNode*& head);

public:

	 //------------------------------------------------------------------------
	 // Default Constructor
	 // Preconditions: None
	 // Postconditions: New Graph object created with size of zero. Table T and
	 //		  vertices have been initialized to size of MAX_VERTICES with siren
	 //		  data indicating they are empty
	 Graph();

	 //------------------------------------------------------------------------
	 // Destructor
	 // Preconditions: None
	 // Postconditions: All vertices and associated edges have been deleted, 
	 //		  Table data has been reset to default and size set to zero.
	 ~Graph();

	 //------------------------------------------------------------------------
	 // Copy Constructor 
	 // Preconditions: None
	 // Postconditions: Created a deep copy of rhs into calling object. All 
	 //		  contents are cleared prior to re-assignment. Member variable T
	 //		  is populated in calling object by a call to findShortestPath() 
	 //		  after assignment
	 Graph(const Graph& rhs);

	 //------------------------------------------------------------------------
	 // Assignment Operator
	 // Preconditions: Calling Graph != right hand side
	 // Postconditions: Created a deep copy of rhs into calling object. All 
	 //		  contents are cleared prior to re-assignment. Member variable T
	 //		  is populated in calling object by a call to findShortestPath() 
	 //		  after assignment
	 const Graph& operator=(const Graph& rhs);

	 //------------------------  ACCESSORS   ----------------------------------

	 //------------------------------------------------------------------------
	 // displayAll() 
	 // Preconditions: at least one vertex populated
	 // Postconditions: streams contents to cout. Includes the name of each vertex
	 //		  as well as the vertex going from and vertex heading to. Indicates the
	 //		  the path of vertices to follow in order to reach destination. This info
	 //		  is displayed for each vertex, including all available paths to other vertices
	 void displayAll() const;

	 //------------------------------------------------------------------------
	 // display() 
	 // Preconditions: must contain valid src and dest values as indicated by validInput(),
	 //				must have called findShortestPath() prior to displaying information
	 // Postconditions: Infomration regarding shortes path from src to dest has been 
	 //				displayed to the console. Additional data displayed includes the name of
	 //				each vertex in the order of the shortest path.
	 void display(int src, int dest) const;

	 //-------------------------------- buildGraph ---------------------------------
	 // Builds a graph by reading data from an ifstream
	 // Preconditions:  infile has been successfully opened and the file contains
	 //                 properly formated data (according to the program specs)
	 // Postconditions: One graph is read from infile and stored in the object
	 //					 Requires user to call
	 //					 findShortestPath() if desired after call, this method does not 
	 //					 update Table T after calling.
	 void buildGraph(ifstream& infile);

	 //------------------------------------------------------------------------
	 // insertEdge :  add edgeNode to adjacency list from vertex Node
	 // Preconditions: src and dest < 1, weight > -1, src and dest within 
	 //		  MAX_VERTICES limit, no self edges or Vertex initialized as nullptr
	 // Postconditions: A new edge has been created, dynamically allocated and 
	 //		  inserted at the front of the adjacency list of the desired vertex
	 //		  with the indicated weight and adjacent vertex. Requires user to call
	 //		  findShortestPath() after calling, this method does not update Table T.
	 //		  returns boolean true on successful insert, false otherwise
	 bool insertEdge(int src, int dest, int weight);

	 //------------------------------------------------------------------------
	 // removeEdge :  find and remove an indicated Edge from an indicated vertex
	 // Preconditions: src and dest < 1, weight > -1, src and dest within 
	 //		  MAX_VERTICES limit, no self edges or Vertex initialized as nullptr
	 // Postconditions: Edge has been found and removed from the indicated Vertex.
	 //		  if an edge is found a boolean true is returned, false otherwise 
	 //		  successfully removed from the calling object.Requires user to call
	 //		  findShortestPath() after calling, this method does not update Table T.
	 //		  returns boolean true on successful insert, false otherwise.
	 bool removeEdge(int src, int dest);

	 //------------------------------------------------------------------------
	 // findShortestPath :  Find shortest paths to each vertex from each vertex
	 // Preconditions: For sucessful Population of Member variable T -> Graph
	 //		  buildGraph must have been called previously
	 // Postconditions: Member Variable T has been populated with information
	 //		  needed to build a path to each vertex in the shortest path
	 void findShortestPath();
};
