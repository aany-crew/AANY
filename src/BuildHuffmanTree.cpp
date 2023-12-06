//
// Created by Noah Markowitz on 12/2/23.
//

#include "../include/BuildHuffmanTree.h"
#include "../include/HuffmanTreeNode.h"
#include <queue>
#include <algorithm>

// Build Huffman Tree accepting input as int array telling frequency of each character
std::shared_ptr<HuffmanTreeNode> BuildHuffmanTree(unordered_map<uint8_t, uint64_t>& frequency_map){

    // Make priority queue PQ
    std::priority_queue<std::shared_ptr<HuffmanTreeNode>, std::vector<std::shared_ptr<HuffmanTreeNode> >, CompareNodes> PQ;

    //int i;

//    // For characters with freq greater than 1, make into a node and push into PQ
//    for (i=0; i<128; i++){
//        if (arr[i] > 0){
//            PQ.push(std::make_shared<HuffmanTreeNode>(char(i), arr[i]));
//        }
//    }

    unordered_map<uint8_t, uint64_t>::iterator it;
    for (it=frequency_map.begin(); it != frequency_map.end(); it++){
        PQ.push(std::make_shared<HuffmanTreeNode>(it->first, it->second));
    }

    // Keep popping and combining smallest nodes until only one is left to be the root
    while (PQ.size()>1){
        auto tmp_node1 = PQ.top();
        PQ.pop();
        auto tmp_node2 = PQ.top();
        PQ.pop();

        auto mergedNode = std::make_shared<HuffmanTreeNode>(tmp_node1->freq+tmp_node2->freq);
        mergedNode->height = max(tmp_node1->height, tmp_node2->height) + 1;
        mergedNode->left = tmp_node1;
        mergedNode->right = tmp_node2;
        PQ.push(mergedNode);
    }
    std::shared_ptr<HuffmanTreeNode> root = PQ.top();
    PQ.pop();
    return root;
}