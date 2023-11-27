//
// Created by Noah Markowitz on 11/27/23.
//

#include "../include/scanning.h"
#include <string>

using namespace std;

int* scan_text(string& text)
{
    int i;
    int n;
    char c;
    int l = text.length();

    // Generate array
    //int arr[128] = {0};
    int* arr = new int[128];
    for (i=0; i<128; i++){
        arr[i] = 0;
    }

    // Iterate over each character in the text
    for (i=0; i<l; i++){
        c = text[i];
        n = int(c);
        arr[n] += 1;
    }

    return arr;
}
