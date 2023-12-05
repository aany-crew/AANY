#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <memory>


struct HuffmanTreeNode {
    // his is a forward declaration of a struct representing a node in a Huffman Tree. 
    //The detailed structure is not provided in the snippet but presumably contains information relevant to
    // Huffman Tree nodes (like character, frequency, left and right children).
};
struct Metadata {
    std::shared_ptr<HuffmanTreeNode> huffmanTreeRoot;
    int nodeIndex;
    // Constructor for convenience
    Metadata(std::shared_ptr<HuffmanTreeNode> root, int index) : huffmanTreeRoot(root), nodeIndex(index) {}
        //Represents metadata associated with a Huffman Tree. It contains:
        //huffmanTreeRoot: A smart pointer (std::shared_ptr) to the root of a Huffman Tree. 
        //Using std::shared_ptr ensures automatic memory management and avoids memory leaks.
        //nodeIndex: An integer presumably representing a specific index or identifier within the Huffman Tree.
        //A constructor for initializing Metadata objects.
};

std::unordered_map<std::string, std::vector<Metadata>> metadata_map;
//A global variable, an unordered map (std::unordered_map) that associates filenames (strings) with a vector of Metadata objects. 
//This map stores metadata for multiple Huffman Trees related to different files.

void update_map(const std::string& filename, const Metadata& metadata) {
    metadata_map[filename].push_back(metadata);
}//Takes a filename and Metadata object as arguments. 
//It updates metadata_map by adding the Metadata object to the vector associated with the given filename. 
//This function is used to store metadata for a specific file.

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
}// Basically Iterates over metadata_map and prints the metadata associated with each file. For each file:
//The filename is printed.
//Each Metadata object in the associated vector is printed, showing the memory address of the Huffman Tree root (as indicated by the pointer) and the node index.

int main() {
    
    //std::shared_ptr<HuffmanTreeNode> root = /* ... */;
    //int nodeIndex = /* ... */;

    //update_map("example.txt", Metadata(root, nodeIndex));

    // Get and print metadata for all files
    get_metadata_for_all_files();

    return 0;
}
