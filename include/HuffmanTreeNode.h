//
// Created by Noah Markowitz on 12/1/23.
//

#ifndef AANY_HUFFMANTREENODE_H
#define AANY_HUFFMANTREENODE_H

#include <memory>
#include <iostream>

using namespace std;

class HuffmanTreeNode{
public:
    // Values
    //int freq;
    uint64_t freq;
    bool has_value;
    //char value;
    uint8_t value;
    int height;

    // Leaf nodes
    std::shared_ptr<HuffmanTreeNode> left;
    std::shared_ptr<HuffmanTreeNode> right;

    // Constructors
    HuffmanTreeNode();
    HuffmanTreeNode(const HuffmanTreeNode&);
    HuffmanTreeNode(uint64_t);
    HuffmanTreeNode(uint8_t ,uint64_t);

};

#endif //AANY_HUFFMANTREENODE_H
