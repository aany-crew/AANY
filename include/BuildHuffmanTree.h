//
// Created by Noah Markowitz on 12/2/23.
//

#ifndef AANY_BUILDHUFFMANTREE_H
#define AANY_BUILDHUFFMANTREE_H

#include "HuffmanTreeNode.h"
#include <memory>
#include <unordered_map>
#include <utility>

std::shared_ptr<HuffmanTreeNode> BuildHuffmanTree(unordered_map<uint8_t, uint64_t> &frequency_map);

// Convert Huffman tree to array. Output is std::pair<uint8_t*, uint64_t*> and each represent char (as uint8_t) and frequency (as uint64_t)
std::pair< vector<uint8_t>, vector<uint64_t> > HuffmanTreeToArray(const std::shared_ptr<HuffmanTreeNode>& a);

// Convert arrays to Huffman tree
//std::shared_ptr<HuffmanTreeNode> ArrayToHuffmanTree(uint8_t* characters, uint64_t* frequency);

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
