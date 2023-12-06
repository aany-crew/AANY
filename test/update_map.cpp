#include <fstream>
#include <sstream>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <string>

class Metadata {
public:
    uint64_t num_encodings;
    uint64_t num_blocks;
    std::unordered_map<std::string, std::unordered_set<uint64_t>> mp;
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

        // Add block IDs to the set for this IP
        while (iss >> block_id) {
            metadata_map[ip].mp[ip].insert(block_id);
            metadata_map[ip].num_blocks = metadata_map[ip].mp[ip].size();
        }
    }

    file.close();
}
