//
// Created by Noah Markowitz on 11/27/23.
//

// g++ -std=c++17 test/scanning_test.cpp src/scanning.cpp  -o bin/test_scan_text.out

#include "../include/scanning.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

    //string fname = "../examples/lorem_ipsum.txt";
    //string example_text;

    ifstream testfile;
    testfile.open(argv[1], ios::in);
    if (!testfile)
    {
        cout << "Error opening file " << endl;
        return -1;
    }

    //testfile.open(fname, fstream::in);
    //while(getline(testfile,example_text)) {
    //    cout << example_text << endl;
    //}
    //testfile>>example_text;
    std::string example_text((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
    testfile.close();
    cout << example_text << endl;

    int* arr = scan_text(example_text);

    // Print output
    int i;
    for (i=0;i<128;i++) {
        cout << char(i) << ": " << arr[i] << endl;
    }

    return 0;

}
