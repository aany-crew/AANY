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

/*
put: put a block into the file, (file is array data structure), if the index from the file is empty, can 
put the block into that space, and mark 1. 
clear: find the index of block from the file (array), remove that block and mark from 1 to 0
get: get the exact index of block from the file (array), some cases if it is empty (0), return -1
write a function, recv_block_index(uint64_t&inx) {
                    recv(socket, &idx, sizeat(idx), 0);
                   }
        write a function, switch(operation):
                            case Get, PUT, CLEAR
                            whatever, which case is called, always use the function recv_block_index(uint64_t&inx) first
                            if this is Get: also write a function send_data()
                            if this is PUT: also write a function receive_data() and store_data()
                            if this is Clear: also write a function clear_data()
*/
#include "node.h"
#include <iostream>
#include <fstream>
#include <vector>

Node::Node() : node_soc(-1), bitmap(1024, 0) {
    memset(&node_addr, 0, sizeof(node_addr));
    // Other constructor initialization if needed
}


/* INITIALIZE */
int Node::initialize() {
    if ((node_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("node: socket failed");
        return -1;
    }

    node_addr.sin_family = AF_INET;
    node_addr.sin_addr.s_addr = INADDR_ANY;
    node_addr.sin_port = htons(PORT);

    if (bind(node_soc, (struct sockaddr *) &node_soc, sizeof(node_soc)) == -1) {
        ERROR("bind");
        close(node_soc);
        return -1;
    }

    if (listen(server_soc, 3) < 0) {
        ERROR("listen");
        close(node_soc);
        return -1;
    }

    return 0;
}

/* Mark Block Occpuied*/
void markBlockOccupied(int index) {
        bitmap[index] = 1;
}
/* Mark Block is empty*/
void clearBlock(int index) {
        bitmap[index] = 0;
}

/* DATAREQUEST TYPE */
enum DataRequestType Node::receive_request_type() {
    int request_type;
    recv(node_soc, &request_type, sizeof(request_type), 0);
    return static_cast<DataRequestType>(request_type);
}

/* RECEIVE BLOCK INDEX FUNCTION */
void Node::recv_block_index(uint64_t&idx) {
    recv(socket, &idx, sizeat(idx), 0);
}

/* SWITCH */
void Node::handle_request() {
    DataRequestType request_type = receive_request_type();

    switch (request_type) {
        case GET:
            recv_block_index(uint64_t&idx);
            send_data();
            break;

        case PUT:
            recv_block_index(uint64_t&idx);
            receive_data();
            store_data();
            break;

        case CLEAR:
            recv_block_index(uint64_t&idx);
            clear_data();
            break;

        default:
            // Handle unsupported request type or error
            break;
    }
}



/* IF GET REQUEST */
// receive index of block first, then send_data() to server
void Node::send_data(int idx) {
    // Check if the block is within file size
        if (index >= 0 && index < FILE_SIZE) {
            // Your code to send block to the server at 'index'
            // Example: Sending block content from the file
            std::ifstream infile(filename, std::ios::binary);
            infile.seekg(index * BLOCK_SIZE);
            char* buffer = new char[BLOCK_SIZE];
            infile.read(buffer, BLOCK_SIZE);
            infile.close();

            // Example code to send 'buffer' to the server
            // send(node_soc, buffer, BLOCK_SIZE, 0);

            delete[] buffer;

            // Mark the block as occupied in the bitmap
            markBlockOccupied(index);
        }
}

/* IF PUT REQUEST */
void Node::receive_data(int index) {
    // Your code to receive block from the server at 'index'
        // Example: Receiving block content from the server
        char* received_data = nullptr; // Placeholder for received data

        // Example code to receive block content from the server
        // recv(node_soc, received_data, BLOCK_SIZE, 0);

        // Writing received block to the file at 'index'
        std::ofstream outfile(filename, std::ios::binary | std::ios::in | std::ios::out);
        outfile.seekp(index * BLOCK_SIZE);
        outfile.write(received_data, BLOCK_SIZE);
        outfile.close();

        delete[] received_data;

        // Mark the block as occupied in the bitmap
        markBlockOccupied(index);
}

/* IF PUT REQUEST */
void Node::store_data(int index, const char* data) {
    // Write 'data' to the file at 'index'
        std::ofstream outfile(filename, std::ios::binary | std::ios::in | std::ios::out);
        outfile.seekp(index * BLOCK_SIZE);
        outfile.write(data, BLOCK_SIZE);
        outfile.close();

        // Mark the block as occupied in the bitmap
        markBlockOccupied(index);
}

/* CLEAR REQUEST*/
void Node::clear_data(int index) {
    // Empty the block in the file at 'index'
        std::ofstream outfile(filename, std::ios::binary | std::ios::in | std::ios::out);
        outfile.seekp(index * BLOCK_SIZE);
        char* empty_data = new char[BLOCK_SIZE](); // Fill with zeros
        outfile.write(empty_data, BLOCK_SIZE);
        outfile.close();

        delete[] empty_data;

        // Mark the block as empty in the bitmap
        clearBlock(index);
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
