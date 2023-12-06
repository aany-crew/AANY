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
#include <utility>

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

    // Map for character frequency
    unordered_map<uint8_t, uint64_t> frequency_map;

    cout << "PASS1" << endl;
    // Get frequency of each character
    GetCharFrequency(frequency_map, example_text);

    cout << "PASS2" << endl;
    // Build the tree
    std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(frequency_map);

    cout << "PASS3" << endl;
    // Generate numeric codes for each character in the tree
    unordered_map<char, string> HuffmanCodes = GenerateHuffmanCodes(root);

    cout << "PASS4" << endl;

    // Convert huffman tree to array
    pair< vector<uint8_t>, vector<uint64_t> > HuffmanArrayRep = HuffmanTreeToArray(root);

    cout << "PASS5" << endl;

    // Convert array(s) back to unordered map
    unordered_map<uint8_t, uint64_t> frequency_map2;
    vector<uint8_t> characters = HuffmanArrayRep.first;
    vector<uint64_t> frequencies = HuffmanArrayRep.second;
    GetCharFrequency(frequency_map2,characters,frequencies);

    cout << "PASS6" << endl;

    // Make a second Huffman tree to compare
    std::shared_ptr<HuffmanTreeNode> root2 = BuildHuffmanTree(frequency_map2);

    cout << "PASS7" << endl;

    // Print tree
    printHuffmanTree(root);
    printHuffmanTree(root2);

    // Tree info print
    cout << "Tree height: " << root->height << endl;
    cout << "Maximum number of nodes: " << pow(2,root->height+1) -1 << endl;

    // Print each character and its numeric code
    cout << "HUFFMAN CODES" << endl;
    unordered_map<char, string>::iterator it;
    for (it=HuffmanCodes.begin(); it != HuffmanCodes.end(); it++){
        cout << it->first << ": " << it->second << endl;
    }

    // Check huffman codes of reconstructed huffman tree
    cout << "HUFFMAN CODES2" << endl;
    for (it=HuffmanCodes.begin(); it != HuffmanCodes.end(); it++){
        cout << it->first << ": " << it->second << endl;
    }

    // Compress text
    vector<bool> chunk(1024,false);
    int textptr = 0;
    int NValidBits = CompressText(root, chunk, 1024, textptr, example_text);
    cout << "PASS5" << endl;

    // Print bits
    cout << "Bitarray: ";
    for (i=0; i<NValidBits; i++){
        cout << (chunk[i] ? '1' : '0');
    }
    cout << endl;

    // Decompress text
    string decompressed_text = DecompressText(root, chunk, NValidBits);
    cout << "Decompressed Text: " << decompressed_text << endl;

    return 0;

}


