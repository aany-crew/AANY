//
// Created by Noah Markowitz on 12/1/23.
//

#include "../include/HuffmanTreeNode.h"

using namespace std;

HuffmanTreeNode::HuffmanTreeNode(){
    freq = 0;
    has_value = false;
}

HuffmanTreeNode::HuffmanTreeNode(char c, int f){
    freq = f;
    value = c;
    has_value = true;
}

HuffmanTreeNode::HuffmanTreeNode(int f){
    freq = f;
    has_value = false;
}

HuffmanTreeNode::HuffmanTreeNode(const HuffmanTreeNode& h){
    freq = h.freq;
    if (h.has_value){
        value = h.value;
        has_value = true;
    } else {
        has_value = false;
    }
}

