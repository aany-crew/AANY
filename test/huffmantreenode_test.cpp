//
// Created by Noah Markowitz on 12/1/23.
//

#include "../include/HuffmanTreeNode.h"
#include "../include/Text.h"
#include "../include/BuildHuffmanTree.h"
#include "../include/GenerateHuffmanCodes.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// g++ -std=c++17 test/huffmantreenode_test.cpp src/Text.cpp src/HuffmanTreeNode.cpp src/BuildHuffmanTree.cpp src/GenerateHuffmanCodes.cpp -o bin/huffmantreenode_test.out
using namespace std;



void printHuffmanTree(const std::shared_ptr<HuffmanTreeNode>& root, int depth = 0) {
    if (!root) {
        return;
    }

    const int spaces = 4;

    printHuffmanTree(root->right, depth + 1);

    for (int i = 0; i < depth * spaces; ++i) {
        std::cout << ' ';
    }

    if (root->has_value) {
        std::cout << root->value << " (" << root->freq << ")" << std::endl;
    } else {
        std::cout << '-' << " (" << root->freq << ")" << std::endl;
    }

    printHuffmanTree(root->left, depth + 1);
}

int main(int argc, char *argv[]){

    char c;
    int f, i;
    string c_code;

    ifstream testfile;
    testfile.open(argv[1], ios::in);
    if (!testfile)
    {
        cout << "Error opening file " << endl;
        return -1;
    }

    std::string example_text((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
    testfile.close();
    cout << example_text << endl;

    // Get frequency of each character
    int* arr = GetCharFrequency(example_text);

    // Build the tree
    std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(arr);

    // Generate numeric codes for each character in the tree
    unordered_map<char, string> HuffmanCodes = GenerateHuffmanCodes(root);

#pragma region Make into a function `ConvertToBits()`
    // Create array of booleans and calculate what size it should be
    int total_bit_size = 0;
    unordered_map<char, string>::iterator it;
    for (it=HuffmanCodes.begin(); it != HuffmanCodes.end(); it++){
        c = it->first;
        c_code = it->second;
        total_bit_size += arr[int(c)]*c_code.length();
    }
    vector<bool> bitarray(total_bit_size, false);


    int txt_ptr = 0;
    int bitarray_ptr=0;
    int c_code_len;
    int c_code_ptr=0;
    while (bitarray_ptr<total_bit_size){
        c = example_text[txt_ptr];
        c_code = HuffmanCodes[c];
        c_code_len = c_code.length();
        while (c_code_ptr<c_code_len){
            bitarray[bitarray_ptr] = c_code[c_code_ptr] == '1' ? true : false;
            c_code_ptr++;
            bitarray_ptr++;
        }
        c_code_ptr = 0;
        txt_ptr++;
    }
#pragma endregion

    // Print tree
    printHuffmanTree(root);

    cout << "Tree height: " << root->height << endl;
    cout << "Maximum number of nodes: " << pow(2,root->height+1) -1 << endl;

    // Print each character and its numeric code
    cout << "HUFFMAN CODES" << endl;
    //unordered_map<char, string>::iterator it;
    for (it=HuffmanCodes.begin(); it != HuffmanCodes.end(); it++){
        cout << it->first << ": " << it->second << endl;
    }

    cout << "Bitarray: ";
    vector<bool>::iterator vit;
    int bl = bitarray.size();
    for (vit = bitarray.begin(); vit != bitarray.end(); vit++){
        cout << (*vit ? '1' : '0');
    }
    cout << endl;


    //<< bitarray << endl;

    return 0;

}


