#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Store.h"
#include <iostream>

using namespace std;

int main() {
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);

    Store testStore;
    ifstream inFile;
    inFile.open("hw4inventory.txt");

    testStore.inputInventory(inFile);
    inFile.close();

    inFile.open("hw4customers.txt");
    testStore.inputCustomers(inFile);
    inFile.close();
   
    inFile.open("hw4commands.txt");
    testStore.inputCommands(inFile);
    inFile.close();

    return 0;
}