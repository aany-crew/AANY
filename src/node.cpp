// created by Yuqi Jin Dec.2 2023
// the functionality of node class is to receive the request_type ---> get(), put(), clear()

/* the class has three functions: receive_data(), send_data(), store_data()
the function receive_request type() is to decide which functionality will be used

Dec 4th 2023
Node: by Yuqi 
	the client.cpp as reference
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

int Node::initialize(std::string node_ip, uint16_t port) {
    if ((node_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("node: socket failed");
        return -1;
    }

    node_addr.sin_family = AF_INET;
    node_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, node_ip.c_str(), &node_addr.sin_addr) != 1) {
        perror("node: inet_pton failed");
        close(node_soc);
        return -1;
    }

    return 0;
}

int Node::get() {
    if (connect(node_soc, (struct sockaddr *)&node_addr, sizeof(node_addr)) == -1) {
        perror("node: connect");
        return -1;
    }

    // Receive data request type from server
    DataRequestType request_type;
    if (recv(node_soc, &request_type, sizeof(request_type), 0) != sizeof(request_type)) {
        perror("node: recv data request type");
        close(node_soc);
        return -1;
    }

    if (request_type == GET) {
        // Receive data from server
        // (Assuming file_size is received from the server)
        uint64_t file_size;
        if (recv(node_soc, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
            perror("node: recv file size");
            close(node_soc);
            return -1;
        }

        // Get data from disk (read from file)
        std::ifstream infile("file_to_get.txt", std::ios::binary | std::ios::ate);
        std::streamsize fileSize = infile.tellg();
        infile.seekg(0);

        if (fileSize < 0) {
            perror("node: file not found or empty");
            infile.close();
            close(node_soc);
            return -1;
        }

        // Send data to server
        uint8_t data_block[BLOCK_SIZE];
        while (!infile.eof() && fileSize > 0) {
            infile.read(reinterpret_cast<char *>(data_block), std::min(BLOCK_SIZE, static_cast<int64_t>(fileSize)));
            int bytes_sent = send(node_soc, data_block, infile.gcount(), 0);
            if (bytes_sent <= 0) {
                perror("node: send data");
                infile.close();
                close(node_soc);
                return -1;
            }
            fileSize -= bytes_sent;
        }
        infile.close();
    }

    close(node_soc);
    return 0;
}

int Node::put() {
    if (connect(node_soc, (struct sockaddr *)&node_addr, sizeof(node_addr)) == -1) {
        perror("node: connect");
        return -1;
    }

    // Receive data request type from server
    DataRequestType request_type;
    if (recv(node_soc, &request_type, sizeof(request_type), 0) != sizeof(request_type)) {
        perror("node: recv data request type");
        close(node_soc);
        return -1;
    }

    if (request_type == PUT) {
        // Receive data from server
        // Assuming file_size is received from the server
        uint64_t file_size;
        if (recv(node_soc, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
            perror("node: recv file size");
            close(node_soc);
            return -1;
        }

        // Put data in disk (write to file)
        std::ofstream outfile("file_to_put.txt", std::ios::binary | std::ios::trunc);
        uint8_t data_block[BLOCK_SIZE];
        while (file_size > 0) {
            int bytes_received = recv(node_soc, data_block, std::min(BLOCK_SIZE, static_cast<int64_t>(file_size)), 0);
            if (bytes_received <= 0) {
                perror("node: recv data");
                outfile.close();
                close(node_soc);
                return -1;
            }
            outfile.write(reinterpret_cast<char *>(data_block), bytes_received);
            file_size -= bytes_received;
        }
        outfile.close();

        // Confirm with server
        uint8_t confirmation = 1;
        if (send(node_soc, &confirmation, sizeof(confirmation), 0) != sizeof(confirmation)) {
            perror("node: send confirmation");
            close(node_soc);
            return -1;
        }
    }

    close(node_soc);
    return 0;
}

int Node::clear() {
    if (connect(node_soc, (struct sockaddr *)&node_addr, sizeof(node_addr)) == -1) {
        perror("node: connect");
        return -1;
    }

    // Receive data request type from server
    DataRequestType request_type;
    if (recv(node_soc, &request_type, sizeof(request_type), 0) != sizeof(request_type)) {
        perror("node: recv data request type");
        close(node_soc);
        return -1;
    }

    if (request_type == CLEAR) {
        // Perform necessary operations to clear data on disk
        // For example: Remove or truncate the file
        // Note: Implementation of clearing data depends on specific requirements
        // In this example, let's assume clearing the file means removing it
        std::string file_to_clear = "file_to_clear.txt";
        if (remove(file_to_clear.c_str()) != 0) {
            perror("node: remove file");
            close(node_soc);
            return -1;
        }

        // Confirm with server
        uint8_t confirmation = 1;
        if (send(node_soc, &confirmation, sizeof(confirmation), 0) != sizeof(confirmation)) {
            perror("node: send confirmation");
            close(node_soc);
            return -1;
        }
    }

    close(node_soc);
    return 0;
}
