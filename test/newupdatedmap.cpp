#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdint>

class Metadata {
public:
    // Removed num_encodings and num_blocks
    std::unordered_map<std::string, std::vector<uint64_t>> mp;
};

std::unordered_map<std::string, Metadata> metadata_map;

void update_map(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string ip;
        uint64_t block_id;
        iss >> ip;

        // Create Metadata object if it doesn't exist
        if (metadata_map.find(ip) == metadata_map.end()) {
            metadata_map[ip] = Metadata();
        }

        // Add block IDs to the vector for this IP
        while (iss >> block_id) {
            metadata_map[ip].mp[ip].push_back(block_id);
        }
    }

    file.close();
}

void print_metadata_map() {
    for (const auto& pair : metadata_map) {
        std::cout << "IP Address: " << pair.first << "\n";

        // Iterate over the map inside Metadata
        for (const auto& ip_pair : pair.second.mp) {
            std::cout << "IP: " << ip_pair.first << " -> Blocks: ";
            
            // Iterate over the vector of block IDs
            for (const auto& id : ip_pair.second) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }
    }
}

int main() {
    update_map("test_data.txt"); // Update the file name as needed
    print_metadata_map();
    return 0;
}
