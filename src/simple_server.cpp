/* write a simple_server.cpp 

> it has the ability to let the local laptop connect it
> below lines are some examples shown in the terminal, 
>> Enter Filename: xxx.txt 
>> if I type the put operate, put the filemap and store on the server (nOTE: Filemap is unordered_map<string, pair< vector<uint8_t>, vector<uint64_t> > > filemap)
>> compress: note(this line calls the function from Text.h to do the compression process for the input txt file)
>> Total time: shown the total compression time
>> Enter Filename: xxx.txt 
>> if I type the get operate, it will get the file from the server
>> total time
*/

// g++ -std=c++17 src/simple_server.cpp src/Text.cpp src/HuffmanTreeNode.cpp src/BuildHuffmanTree.cpp src/GenerateHuffmanCodes.cpp -o bin/simple_server.out
#include "../include/Text.h"
#include "../include/GenerateHuffmanCodes.h"
#include "../include/BuildHuffmanTree.h"
#include "../include/HuffmanTreeNode.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <sstream>
#include <queue>
#include <fstream> // Include the file handling library
#include <string>



void store_on_server(const std::string& filename, std::unordered_map<std::string, std::pair<std::vector<uint8_t>, std::vector<uint64_t> > >& filemap, std::unordered_map<std::string, std::shared_ptr<HuffmanTreeNode> > &treemap, std::unordered_map<std::string, std::pair<int, vector<bool> > > &datamap) {
    std::cout << "Store on server\n";
    
    // Open file and read contents
    ifstream infile;
    infile.open(filename, ios::in);
    if (!infile)
    {
        cout << "Error opening file " << endl;
        return;
    }
    std::string example_text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();

    cout << example_text << endl;

    // Map for character frequency and get freq of each character
    unordered_map<uint8_t, uint64_t> frequency_map;
    GetCharFrequency(frequency_map, example_text);

    // Build Huffman tree
    std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(frequency_map);
    treemap[filename] = root;

    // Generate Huffman codes from tree
    unordered_map<char, string> HuffmanCodes = GenerateHuffmanCodes(root);
    cout << "HUFFMAN CODES" << endl;
    unordered_map<char, string>::iterator it2;
    for (it2=HuffmanCodes.begin(); it2 != HuffmanCodes.end(); it2++){
        cout << it2->first << ": " << it2->second << endl;
    }

    // Create two arrays from tree
    pair< vector<uint8_t>, vector<uint64_t> > HuffmanArrayRep = HuffmanTreeToArray(root);
    filemap[filename] = HuffmanArrayRep;

    // Compress text. First create a vector<bool> that has enough space for all the bits
    int n_bits = 0;
    unordered_map<uint8_t , uint64_t>::iterator it;
    char c;
    int f;
    for (it=frequency_map.begin(); it != frequency_map.end(); ++it){
        c = (char)it->first;
        f = it->second;
        n_bits += f*HuffmanCodes[c].length();
    }
    vector<bool> chunk(n_bits,false);


    // Actually compress
    int textptr = 0;
    int NValidBits = CompressText(root, chunk, n_bits, textptr, example_text);
    //datamap[filename] = std::pair< NValidBits,chunk>;
    datamap[filename].first = n_bits;
    datamap[filename].second = chunk;

    cout << "Bitarray: ";
    int i;
    for (i=0; i<n_bits; i++){
        cout << (chunk[i] ? '1' : '0');
    }
    cout << endl;

    // compressFile(filename, filemap);
}

void retrieve_file(const std::string& filename, std::unordered_map<std::string, std::pair<std::vector<uint8_t>, std::vector<uint64_t> > >& filemap, std::unordered_map<std::string, std::shared_ptr<HuffmanTreeNode> > &treemap, std::unordered_map<std::string, std::pair<int, vector<bool> > > &datamap) {
    std::cout << "Retrieve file\n";

    // Get everything
    //std::shared_ptr<HuffmanTreeNode> root = treemap[filename];
    std::pair<std::vector<uint8_t>, std::vector<uint64_t> > arrays = filemap[filename];
    int NValidBits = datamap[filename].first;
    vector<bool> chunk = datamap[filename].second;
    //vector<bool> data = datamap[filename];

    // Convert array(s) back to unordered map
//    unordered_map<uint8_t, uint64_t> frequency_map;
//    vector<uint8_t> characters = arrays.first;
//    vector<uint64_t> frequencies = arrays.second;
//    GetCharFrequency(frequency_map,characters,frequencies);
//
//    // Build tree again
//    std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(frequency_map);

    std::shared_ptr<HuffmanTreeNode> root = treemap[filename];

    cout << "Bitarray: ";
    int i;
    for (i=0; i<NValidBits; i++){
        cout << (chunk[i] ? '1' : '0');
    }
    cout << endl;

    // Decompress
    std::string decompressed_text = DecompressText(root, chunk, NValidBits);

    cout << decompressed_text << endl;

}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    // int addrlen = sizeof(address);
    // char buffer[1024] = {0};
    std::unordered_map<std::string, std::pair<std::vector<uint8_t>, std::vector<uint64_t> > > filemap;
    std::unordered_map<std::string, std::shared_ptr<HuffmanTreeNode> > treemap;
    std::unordered_map<std::string, std::pair<int, vector<bool> > > datamap;
    std::unordered_map<std::string, std::shared_ptr<HuffmanTreeNode> >::iterator it;
    // Creating socket file descriptor
    // ... (Socket creation and setup)

    while (true) {
        std::string command;
        std::string filename;

        std::cout << "Enter Filename: ";
        std::getline(std::cin, filename); // Read the filename from user input

        std::cout << "Enter Command (PUT/GET): ";
        std::getline(std::cin, command); // Read the command from user input

        if (command == "PUT") {
            store_on_server(filename, filemap, treemap, datamap);
        } else if (command == "GET") {
            if (filemap.find(filename) == filemap.end())
            {
                std::cout << "No file by that name here!" << endl;

            } else {
                retrieve_file(filename, filemap, treemap, datamap);
                std::cout << "Retrieve file\n";
            }

            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulating retrieval time
        } else {
            std::cout << "Invalid command\n";
        }

        std::cout << "Here's what we have: " << endl;
        for (it=treemap.begin(); it!=treemap.end(); it++){
            std::cout << it->first << endl;
        }
    }
    return 0;
}
