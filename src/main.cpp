#include <iostream>
#include "TrafficControlCenter.h"

using namespace std;

int main() {
    cout << "========================================================\n";
    cout << "      TRAFFIC CONTROL CENTER - PHASE 2\n";
    cout << "      Data Structures & Algorithms Project\n";
    cout << "========================================================\n\n";
    TrafficControlCenter tcc;


    string inputFilename = tcc.getUIInputFilename();


    string outputFilename = tcc.getUIOutputFilename();

    cout << "\nLoading configuration and events from " << inputFilename << "...\n";
    if (!tcc.loadFile(inputFilename.c_str())) {
        cerr << "\n[ERROR] Failed to load input file: " << inputFilename << endl;
        cerr << "Please make sure the file exists and is readable.\n";
        return 1;
    }

    cout << "Initialization complete.\n\n";

    tcc.simulate(outputFilename.c_str());

    cout << "\nSimulation completed successfully!\n";
    cout << "Output written to " << outputFilename << endl;

    return 0;
}