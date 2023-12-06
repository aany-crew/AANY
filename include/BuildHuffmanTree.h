//
// Created by Noah Markowitz on 12/2/23.
//

#ifndef AANY_BUILDHUFFMANTREE_H
#define AANY_BUILDHUFFMANTREE_H

#include "HuffmanTreeNode.h"
#include <memory>
#include <unordered_map>

std::shared_ptr<HuffmanTreeNode> BuildHuffmanTree(unordered_map<uint8_t, uint64_t> &frequency_map);

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
