#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <memory>


struct HuffmanTreeNode {
    // ... node details ...
};
struct Metadata {
    std::shared_ptr<HuffmanTreeNode> huffmanTreeRoot;
    int nodeIndex;
    // Constructor for convenience
    Metadata(std::shared_ptr<HuffmanTreeNode> root, int index) : huffmanTreeRoot(root), nodeIndex(index) {}
};

std::unordered_map<std::string, std::vector<Metadata>> metadata_map;

void update_map(const std::string& filename, const Metadata& metadata) {
    metadata_map[filename].push_back(metadata);
}

// Function to get and print metadata for all files
void get_metadata_for_all_files() {
    for (const auto& pair : metadata_map) {
        std::cout << "File: " << pair.first << std::endl;
        for (const auto& metadata : pair.second) {
            // Print details of each metadata entry
            std::cout << "  Huffman Tree Root: " << metadata.huffmanTreeRoot
                      << ", Node Index: " << metadata.nodeIndex << std::endl;
        }
    }
}

int main() {
    
    //std::shared_ptr<HuffmanTreeNode> root = /* ... */;
    //int nodeIndex = /* ... */;

    //update_map("example.txt", Metadata(root, nodeIndex));

    // Get and print metadata for all files
    get_metadata_for_all_files();

    return 0;
}
