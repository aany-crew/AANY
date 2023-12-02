//
// Created by Noah Markowitz on 12/2/23.
//

#ifndef AANY_GENERATEHUFFMANCODES_H
#define AANY_GENERATEHUFFMANCODES_H

#include "HuffmanTreeNode.h"
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

void GenerateHuffmanCodesDriver(const shared_ptr<HuffmanTreeNode>& root, string code, unordered_map<char, string>& huffmanCodes);

unordered_map<char, string> GenerateHuffmanCodes(const shared_ptr<HuffmanTreeNode>& root);

#endif //AANY_GENERATEHUFFMANCODES_H
