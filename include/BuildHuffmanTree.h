//
// Created by Noah Markowitz on 12/2/23.
//

#ifndef AANY_BUILDHUFFMANTREE_H
#define AANY_BUILDHUFFMANTREE_H

#include "HuffmanTreeNode.h"
#include <memory>

std::shared_ptr<HuffmanTreeNode> BuildHuffmanTree(int*);

struct CompareNodes {
    bool operator()(const std::shared_ptr<HuffmanTreeNode>& a, const std::shared_ptr<HuffmanTreeNode>& b) const {
        if (a->freq == b->freq){
            return int(a->value) > int(b->value);
        } else{
            return a->freq > b->freq;
        }
    }
};

#endif //AANY_BUILDHUFFMANTREE_H
