#include "Graph.h"
#include <iostream>
#include <fstream>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
using namespace std;

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    Graph test;
    test = test;

    // test public methods prior to filling graph
    test.findShortestPath();
    test.displayAll();
    test.display(2, 3);
    test.removeEdge(2, 3);
    test.insertEdge(1, 2, 3);

    // display all

    // display

    // build graph



    
    // find shortest path
    ifstream file;
    file.open("HW3.txt");
    test.buildGraph(file);

    // remove edge
    cout << "DNE Vertex insert: " << test.removeEdge(100, 2) << endl;
    cout << "Remove self edge: " << test.removeEdge(1, 1) << endl;
    cout << "DNE Adj Vertex: " << test.removeEdge(1, 100) << endl;
    cout << "Insert over INT_MAX: " << test.removeEdge(102, 2) << endl;
    test.findShortestPath();
    test.displayAll();

    // insertEdge
    cout << "DNE Vertex insert: " << test.insertEdge(100, 2, 5) << endl;
    cout << "Insert self edge: " << test.insertEdge(1, 1, 1) << endl;
    cout << "Insert negative weight " << test.insertEdge(1, 2, -1) << endl;
    cout << "DNE Adj Vertex: " << test.insertEdge(1, 100, 3) << endl;
    cout << "Insert over INT_MAX: " << test.insertEdge(102, 2, 3) << endl;
    // inserting an edge that already exists? different weight? let findshortestpath handle it?

    test.findShortestPath();
    test.displayAll();

    test.findShortestPath();
    test.displayAll();
    // removeEdge has been tested
    // insertEdge has been tested

    //  first node
    // last node
    // general case
    //
    Graph test2;
    Graph test3;
    test3 = test2 = test;
    test2.insertEdge(2, 7, 12);
    test3.insertEdge(2, 5, 7);
    test.insertEdge(2, 6, 19);

    return 0;
}