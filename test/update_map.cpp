#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint> // For uint64_t

struct Metadata {
    uint64_t numBlocks; // Number of blocks in the compressed file
    std::string storageLocation; // IP address
    std::unordered_map<std::string, std::unordered_set<uint64_t>> blockMap; // Map IP address to each block ID it holds

    // Constructor to initialize Metadata objects
    Metadata(uint64_t numBlocks, std::string storageLocation, 
             const std::unordered_map<std::string, std::unordered_set<uint64_t>>& blockMap)
        : numBlocks(numBlocks), storageLocation(storageLocation), blockMap(blockMap) {}
};

std::unordered_map<std::string, std::vector<Metadata>> metadata_map;

void update_map(const std::string& filename, const Metadata& metadata) {
    metadata_map[filename].push_back(metadata);
}

void get_metadata_for_all_files() {
    for (const auto& pair : metadata_map) {
        std::cout << "File: " << pair.first << std::endl;
        for (const auto& metadata : pair.second) {
            std::cout << "  Number of Blocks: " << metadata.numBlocks
                      << ", Storage Location: " << metadata.storageLocation << std::endl;
            for (const auto& ip_block_pair : metadata.blockMap) {
                std::cout << "  IP Address: " << ip_block_pair.first << " holds blocks: ";
                for (const auto& blockID : ip_block_pair.second) {
                    std::cout << blockID << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}

int main() {
    // Get filename from user
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    // Check if the filename exists in the metadata_map
    if (metadata_map.find(filename) != metadata_map.end()) {
        // Display metadata for the specified file
        std::cout << "Metadata for " << filename << ":\n";
        for (const auto& metadata : metadata_map[filename]) {
            std::cout << "  Number of Blocks: " << metadata.numBlocks
                      << ", Storage Location: " << metadata.storageLocation << std::endl;
            for (const auto& ip_block_pair : metadata.blockMap) {
                std::cout << "  IP Address: " << ip_block_pair.first << " holds blocks: ";
                for (const auto& blockID : ip_block_pair.second) {
                    std::cout << blockID << " ";
                }
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "No metadata found for " << filename << std::endl;
    }

    return 0;
}
