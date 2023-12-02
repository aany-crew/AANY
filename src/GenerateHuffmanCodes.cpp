//
// Created by Noah Markowitz on 12/2/23.
//

#include "../include/GenerateHuffmanCodes.h"

using namespace std;

unordered_map<char, string> GenerateHuffmanCodes(const shared_ptr<HuffmanTreeNode>& root){

    // Map for character to codes
    unordered_map<char,string> huffmanCodes;

    GenerateHuffmanCodesDriver(root, "", huffmanCodes);

    return huffmanCodes;
}

void GenerateHuffmanCodesDriver(const shared_ptr<HuffmanTreeNode>& root, string code, unordered_map<char,string>& huffmanCodes){
    if (!root){
        return;
    }

    if (root->has_value){
        huffmanCodes[root->value] = code;
    }

    // Traverse left (0) and append '0' to the code
    GenerateHuffmanCodesDriver(root->left, code + '0', huffmanCodes);

    // Traverse right (1) and append '1' to the code
    GenerateHuffmanCodesDriver(root->right, code + '1', huffmanCodes);
}
