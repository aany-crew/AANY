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
    int freq;
    bool has_value;
    char value;
    int height;

    // Leaf nodes
    std::shared_ptr<HuffmanTreeNode> left;
    std::shared_ptr<HuffmanTreeNode> right;

    // Constructors
    HuffmanTreeNode();
    HuffmanTreeNode(const HuffmanTreeNode&);
    HuffmanTreeNode(int);
    HuffmanTreeNode(char,int);
};

#endif //AANY_HUFFMANTREENODE_H
