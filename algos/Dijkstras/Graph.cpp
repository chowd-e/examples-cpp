//--------------------------------------------------------------------
// Graph.cpp
// Implementation of Graph class
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
//		  processing, including: EdgeNode, VertexNode, Table, and HeapFood. See
//		  Graph.h for more elaborate description.

#include <fstream>
#include <string>
#include "Graph.h"
#include "Heap.h"
using namespace std;

//------------------------------------------------------------------------
// Default Constructor
// Preconditions: None
// Postconditions: New Graph object created with size of zero. Table T and
//		  vertices have been initialized to size of MAX_VERTICES with siren
//		  data indicating they are empty
Graph::Graph() {
	 size = 0;
}

//------------------------------------------------------------------------
// Copy Constructor 
// Preconditions: None
// Postconditions: Created a deep copy of rhs into calling object. All 
//		  contents are cleared prior to re-assignment. Member variable T
//		  is populated in calling object by a call to findShortestPath() 
//		  after assignment
Graph::Graph(const Graph& rhs) {
	 *this = rhs;
}

//------------------------------------------------------------------------
// Assignment Operator
// Preconditions: Calling Graph != right hand side
// Postconditions: Created a deep copy of rhs into calling object. All 
//		  contents are cleared prior to re-assignment. Member variable T
//		  is populated in calling object by a call to findShortestPath() 
//		  after assignment
const Graph& Graph::operator=(const Graph& rhs) {
	 if (this != &rhs) { // equivalent?
		  clearGraph();
		  size = rhs.size;

		  // insert vertices before edges, considered invalid if vertex you
		  // create an edge to doesn't exit
		  for (int i = 1; i <= size; i++) { // copy vertex + edges
				vertices[i].data = new Vertex;
				*vertices[i].data = *rhs.vertices[i].data;
		  }
		  // copy edges after vertices have been initialized
		  for(int i = 1; i <= size; i++)
				copyEdges(i, rhs.vertices[i].edgeHead);

		  findShortestPath(); // populate T accordingly
	 }
	 return rhs;
}

//------------------------------------------------------------------------
// Destructor
// Preconditions: None
// Postconditions: All vertices and associated edges have been deleted, 
//		  Table data has been reset to default and size set to zero.
Graph::~Graph() {
	 clearGraph();
}

//------------------------  ACCESSORS   ----------------------------------

//------------------------------------------------------------------------
// Overload HeapFood operator<()
// Preconditions: None
// Postconditions: returns boolean indicating which object is less indicated
//		  by the weight member variable in each object. For use by Priority Q
bool Graph::HeapFood::operator<(const HeapFood& rhs) const {
	 return(this->weight < rhs.weight);
}

//------------------------------------------------------------------------
// validInput()
// Preconditions: None
// Postconditions: returns boolean indicating if arguments src and dest are
//		  positive values less than the defined MAX_VERTICES, src and dest are not
//		  indicating a self-edge, and the vertex at src has been initialized
bool Graph::validInput(int src, int dest) const {
	 return (src > 0 && dest > 0 && src <= MAX_VERTICES && dest <= MAX_VERTICES && src != dest && vertices[src].data != nullptr && vertices[dest].data != nullptr);
}

//------------------------------------------------------------------------
// displayAll() 
// Preconditions: at least one vertex populated
// Postconditions: streams contents to cout. Includes the name of each vertex
//		  as well as the vertex going from and vertex heading to. Indicates the
//		  the path of vertices to follow in order to reach destination. This info
//		  is displayed for each vertex, including all available paths to other vertices
void Graph::displayAll() const {
	 if (size > 0) {
		  cout << "Description\t\tFrom\tTo\tDist\tPath" << endl;
		  vector<int> dummy; // 
		  for (int i = 1; i <= size; i++) {
				cout << *vertices[i].data << endl; // vertex name
				for (int j = 1; j <= size; j++)
					 if (i != j) printVertex(i, j, dummy);	// no self edging
		  }
	 }
}

//------------------------------------------------------------------------
// display() 
// Preconditions: must contain valid src and dest values as indicated by validInput(),
//				must have called findShortestPath() prior to displaying information
// Postconditions: Infomration regarding shortes path from src to dest has been 
//				displayed to the console. Additional data displayed includes the name of
//				each vertex in the order of the shortest path.
void Graph::display(int src, int dest) const {
	 vector<int> locations;
	 if(validInput(src, dest))
		  // was a path found? if yes print locations
		  if (printVertex(src, dest, locations))
				for (auto i : locations)
					 cout << *vertices[i].data << endl;
		  
	 cout << endl;
}

//------------------------------------------------------------------------
// printVertex() 
// Preconditions: must contain valid src and dest values as indicated by validInput(),
//				must have called findShortestPath() prior to displaying information
// Postconditions: Infomration regarding shortes path from src to dest has been 
//				displayed to the console. Indices of vertices along the path has filled
//				the vector designated as 'locs' within the parameters to be handled by
//				calling function if desired
bool Graph::printVertex(int src, int dest, vector<int>& locs) const {
	 string dist = "--";

	 if (!validInput(src, dest))
		  return false;
	 if (T[src][dest].dist != INT_MAX)
		  dist = to_string(T[src][dest].dist);

	 cout << "\t\t\t" << src << "\t" << dest << "\t" << dist << "\t";

	 string path = printPath(src, dest, locs);
	 cout << path << endl;
	 return path == "" ? false : true;
	 }

//------------------------------------------------------------------------
// printPath() 
// Preconditions: 
// Postconditions: Information regarding the vertices visited along the path
//		  from src to dest has been returned in a string. If there is no valid
//		  path from src to dest, this method returns an empty string
string Graph::printPath(int src, int dest, vector<int>& locs) const {
	 
	 string path;
	 if (dest == INT_MAX) {
		  return ""; // no path found
	 }
	 if (dest == src) {
		  locs.push_back(src);
		  return to_string(src) + " ";
	 }
	 else {
		  path = printPath(src, T[src][dest].path, locs);
		  if(path.length() > 0) path += to_string(dest) + " ";
		  locs.push_back(dest);
		  return path;
	 }
}

//------------------------  MUTATORS   ----------------------------------

//------------------------------------------------------------------------
// clearGraph() 
// Preconditions: None
// Postconditions: All vertices and associated edges have been deleted, 
//		  Table data has been reset to default and size set to zero.
void Graph::clearGraph() {
	 for (int i = 1; i <= size; i++) { // reset vertices to default constructor state
		  delete vertices[i].data; // delete Vertex 
		  vertices[i].data = nullptr;
		  clearEdges(vertices[i].edgeHead); // remove edges
	 }

	 // clear Table Array Row-wise
		  Table emptyTable;
		  for (int i = 1; i <= size; i++) 
				fill_n(T[i], MAX_VERTICES, emptyTable);
	 
		  size = 0;
}

//------------------------------------------------------------------------
// clearEdges()
// Preconditions: Vertex mustn't have nullptr at edgeHead
// Postconditions: Starting at the argument passed for head, all EdgeNodes
//		  after have been removed with dynamical memory handled
void Graph::clearEdges(EdgeNode*& head) {
	 if (head == nullptr)
		  return;
	 while (head->nextEdge != nullptr) {
		  EdgeNode* temp = head;
		  head = head->nextEdge;
		  delete temp; // delete head node
	 }
	 // delete last node
	 delete head;
	 head = nullptr;
	 return;
}

//------------------------------------------------------------------------
// copyEdges()
// Preconditions: rhs EdgeNode != nullptr
// Postconditions: iterates through all edgeNodes in a separate Graph, copying
//		  and inserting the edges into the calling graph. Requires user to call
//		  findShortestPath() if desired after call, this method does not update
//		  Table T after calling.
void Graph::copyEdges(int src, const EdgeNode* rhsEdge) {
	 while (rhsEdge != nullptr) {
		  insertEdge(src, rhsEdge->adjVertex, rhsEdge->weight);
		  rhsEdge = rhsEdge->nextEdge;
	 }
}

//-------------------------------- buildGraph ---------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: One graph is read from infile and stored in the object
//					 Requires user to call
//					 findShortestPath() if desired after call, this method does not 
//					 update Table T after calling.
void Graph::buildGraph(ifstream& infile) {

	infile >> size;                          // data member stores array size
	if (infile.eof())
		return;
	infile.ignore();                         // throw away '\n' to go to next line

	// get descriptions of vertices
	for (int v = 1; v <= size; v++) {
		vertices[v].data = new Vertex;
		infile >> *vertices[v].data;
	}

	// fill cost edge array
	int src = 1, dest = 1, weight = 1;
	for (;;) {
		infile >> src >> dest >> weight;
		if (src == 0 || infile.eof() || src > MAX_VERTICES)
			break;
		insertEdge(src, dest, weight);
	}
}

//------------------------------------------------------------------------
// insertEdge :  add edgeNode to adjacency list from vertex Node
// Preconditions: src and dest < 1, weight > -1, src and dest within 
//		  MAX_VERTICES limit, no self edges or Vertex initialized as nullptr
// Postconditions: A new edge has been created, dynamically allocated and 
//		  inserted at the front of the adjacency list of the desired vertex
//		  with the indicated weight and adjacent vertex. Requires user to call
//		  findShortestPath() after calling, this method does not update Table T
bool Graph::insertEdge(int src, int dest, int weight) {
	 // input verification, ranging vertices, forcing positive weight, no self edge, no unitialized
	 if (weight < 0 || !validInput(src, dest))
		  return false;

	 EdgeNode*& head = vertices[src].edgeHead;
	 EdgeNode* temp = new EdgeNode; // create new edge

	 // insert to front of list
	 temp->adjVertex = dest;
	 temp->weight = weight;
	 temp->nextEdge = head;
	 head = temp;

	 return true;
}

//------------------------------------------------------------------------
// removeEdge :  find and remove an indicated Edge from an indicated vertex
// Preconditions: src and dest < 1, weight > -1, src and dest within 
//		  MAX_VERTICES limit, no self edges or Vertex initialized as nullptr
// Postconditions: Edge has been found and removed from the indicated Vertex.
//		  if an edge is found a boolean true is returned, false otherwise 
//		  successfully removed from the calling object.Requires user to call
//		  findShortestPath() after calling, this method does not update Table T
bool Graph::removeEdge(int src, int dest) {
	 if (!validInput(src, dest))
		  return false;
	 
	 EdgeNode* curr = vertices[src].edgeHead;

	 if (curr == nullptr)
		  return false; // check for empty list
	 
	 if (curr->adjVertex == dest) { // case for first node
		  EdgeNode*& head = vertices[src].edgeHead;
		  head = curr->nextEdge;
		  delete curr;
		  curr = nullptr;
	 }
	 else { // general case - non-first node removed

		  while (curr->nextEdge != nullptr && curr->nextEdge->adjVertex != dest)
				curr = curr->nextEdge;

		  if (curr->nextEdge == nullptr) // ended at nullptr, not found
				return false;
		  else {   // delete and link to remaining list
					 EdgeNode* remainingList = curr->nextEdge->nextEdge;
					 delete curr->nextEdge;
					 curr->nextEdge = remainingList;
		  }
	 }  
	 return true; // success
}

//------------------------------------------------------------------------
// findShortestPath :  Find shortest paths to each vertex from each vertex
// Preconditions: For sucessful Population of Member variable T -> Graph
//		  buildGraph must have been called previously
// Postconditions: Member Variable T has been populated with information
//		  needed to build a path to each vertex in the shortest path
void Graph::findShortestPath() { 
	 // iterate through each vertex // don't need to check last index
	 Heap<HeapFood> priorityQ;
	 
	 for (int row = 1; row <= size; row++) {
		  T[row][row].dist = 0;
		  
		  HeapFood* startNode = new HeapFood(row, 0);
		  priorityQ.insert(startNode);
		  
		  while (!priorityQ.isEmpty()) {
				HeapFood* min = priorityQ.deleteMin();
				int src = min->tableCol;
				// mark source node visited
				T[row][src].visited = true;
				// get the edge between source and various sink nodes
				EdgeNode* v = vertices[src].edgeHead;

				while (v != nullptr) {
					 // src is index of initial vertex
					 // v is the edge from src to snk
					 // snk is the index of the terminating vertex
					 // src->v->snk
					 // v->weight is the distance from src->sink
					 int snk = v->adjVertex;

					 if (!T[row][snk].visited && T[row][snk].dist > (T[row][src].dist + v->weight)) {
						  T[row][snk].dist = T[row][src].dist + v->weight;
						  T[row][snk].path = src;

						  HeapFood* newVert = new HeapFood(snk, T[row][snk].dist); // dynamically allocate for PQ
						  priorityQ.insert(newVert); // enqueue vertices to Pq
					 }
					 v = v->nextEdge; // iterate through edges
				}
				delete min; // delete node popped off priority Q
				min = nullptr;
		  }
	 }
	 return;
}