// updated by Yuqi Jin Dec.5 2023

/* the class has three functions: receive_data(), send_data(), store_data()
the function receive_request type() is to decide which functionality will be used


	recv data_request_type (put, get, clear)
get:
	get data from disk (read() syscall, or <fstream>)
	send data to server
put:
	recv data from server
	put data in disk (write() syscall, or <fstream>)

*/ 

#include "node.h"
#include <iostream>
#include <fstream>

Node::Node() {
    node_soc = 0;
    memset(&node_addr, 0, sizeof(node_addr));
}

int Node::initialize(std::string server_ip, uint16_t port) {
    if ((node_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("node: socket failed");
        return -1;
    }

    node_addr.sin_family = AF_INET;
    node_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &node_addr.sin_addr) != 1) {
        perror("node: inet_pton failed");
        close(node_soc);
        return -1;
    }

    return 0;
}


enum DataRequestType Node::receive_request_type() {
    int request_type;
    recv(node_soc, &request_type, sizeof(request_type), 0);
    return static_cast<DataRequestType>(request_type);
}


// add the switch function, so can handle different request type put / get 
void Node::handle_request() {
    DataRequestType request_type = receive_request_type();

    switch (request_type) {
        case GET:
            // get the data from the disk
            receive_data();
            // send data to server
            send_data();
            break;

        case PUT:
            receive_data();
            store_data();
            break;

        case CLEAR:
            // Handle CLEAR request if needed
            break;

        default:
            // Handle unsupported request type or error
            break;
    }
}




/* IF GET REQUEST */
int Node::send_data() {
    std::ifstream infile("filename", std::ios::binary);
    if (!infile.is_open()) {
        // Handle error: File not found or unable to open
        return -1;
    }

    // Determine the size of the file
    infile.seekg(0, std::ios::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // Allocate buffer to read the file content
    char* buffer = new char[file_size];
    infile.read(buffer, file_size);
    infile.close();

    // Send file content over the socket
    send(node_soc, buffer, file_size, 0);

    delete[] buffer;

    return 0;
}

/* IF PUT REQUEST */
int Node::receive_data() {
    uint8_t data_buffer[BLOCK_SIZE] = {0};

    // Receive data from the server
    recv(node_soc, data_buffer, BLOCK_SIZE, 0);

    // Write received data to disk
    std::ofstream outfile("filename", std::ios::binary);
    if (!outfile.is_open()) {
        // Handle error: Unable to create/open file for writing
        return -1;
    }

    outfile.write(reinterpret_cast<char*>(data_buffer), BLOCK_SIZE);
    outfile.close();

    // Send confirmation to the server
    int confirmation = 1;
    send(node_soc, &confirmation, sizeof(confirmation), 0);

    return 0;
}

/* IF PUT REQUEST */
int Node::store_data() {
    std::ifstream infile("filename", std::ios::binary);
    if (!infile.is_open()) {
        // Handle error: File not found or unable to open
        return -1;
    }

    // Determine the size of the file
    infile.seekg(0, std::ios::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // Allocate buffer to read the file content
    char* buffer = new char[file_size];
    infile.read(buffer, file_size);
    infile.close();

    // Send file content to the server
    send(node_soc, buffer, file_size, 0);

    delete[] buffer;

    // Wait for confirmation from the server
    int confirmation;
    recv(node_soc, &confirmation, sizeof(confirmation), 0);

    return confirmation; // Return confirmation status to the caller
}


// The following Note from Austin
// enum DataRequestType Node::receive_request_type()
// {
// 	recv
// }

// /* ONLY IF PUT REQUEST */
// int Node::receive_data()
// {
// 	recv something that uniquely identifies that block with that file

// 	uint8_t data_buffer[BLOCK_SIZE] = {0};
// 	recv
// 	write("filename")
// }

// /* ONLY IF GET REQUEST */
// int Node::send_data()
// {
// 	read("filename")
// 	send()
// }

// /* ONLY IF PUT REQUEST */
// int Node::store_data()
// {
// 	write("filename")
// 	send confirmation
// }
