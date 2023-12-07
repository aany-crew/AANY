//
// Created by Noah Markowitz on 12/2/23.
//

#include "../include/BuildHuffmanTree.h"
#include "../include/HuffmanTreeNode.h"
#include <cmath>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

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

// Convert Huffman tree to array. Output is std::pair<uint8_t*, uint64_t*> and each represent char (as uint8_t) and frequency (as uint64_t)
std::pair< vector<uint8_t>, vector<uint64_t> > HuffmanTreeToArray(const std::shared_ptr<HuffmanTreeNode>& root){

    // Create the arrays that will be needed
    //uint8_t *characters = new uint8_t[max_n_nodes];
    //uint64_t *frequencies = new uint64_t[max_n_nodes];
    vector<uint8_t> characters;
    vector<uint64_t> frequencies;

    // Perform in-order traversal
    stack<shared_ptr<HuffmanTreeNode> >s;
    shared_ptr<HuffmanTreeNode> current_node = root;

    while (current_node != NULL || !s.empty()){

        // Keep traversing left
        while (current_node != NULL){
            s.push(current_node);
            current_node = current_node->left;
        }

        if (!s.empty()){

            // Get the node and pop it
            current_node = s.top();
            s.pop();

            // Store the data
            //frequencies[array_ptr] = current_node->freq;
            //characters[array_ptr] = current_node->has_value ? current_node->value : 0;
            //array_ptr++;
            if (current_node->has_value){
                frequencies.push_back(current_node->freq);
                //characters.push_back(current_node->has_value ? current_node->value : 0);
                characters.push_back(current_node->value);
            }


            current_node = current_node->right;
        }
    }

    // Create a pair to return the two arrays
    //pair<uint8_t*,uint64_t*> output (characters, frequencies);
    pair< vector<uint8_t>, vector<uint64_t> > output(characters, frequencies);
    return output;
}

// Convert arrays to Huffman tree
//std::shared_ptr<HuffmanTreeNode> ArrayToHuffmanTree(uint8_t* characters, uint64_t* frequency){
//
//}
