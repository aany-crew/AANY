//
// Created by Noah Markowitz on 12/1/23.
//

#include "../include/HuffmanTreeNode.h"

using namespace std;

HuffmanTreeNode::HuffmanTreeNode(){
    freq = 0;
    has_value = false;
    height = 0;
}

HuffmanTreeNode::HuffmanTreeNode(uint8_t c, uint64_t f){
    freq = f;
    value = c;
    has_value = true;
    height = 0;
}

HuffmanTreeNode::HuffmanTreeNode(uint64_t f){
    freq = f;
    has_value = false;
    height = 0;
}

HuffmanTreeNode::HuffmanTreeNode(const HuffmanTreeNode& h){
    freq = h.freq;
    if (h.has_value){
        value = h.value;
        has_value = true;
    } else {
        has_value = false;
    }
    height = 0;
}

