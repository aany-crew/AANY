#ifndef NODE_H
#define NODE_H

#include <string>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define PORT 8081
#define BLOCK_SIZE 1024

enum DataRequestType {
    PUT,
    GET,
    CLEAR
};

class Node {
public:
	Node();
	int initialize(std::string node_ip, uint16_t port);
	void handle_request();
	void recv_block_index(uint64_t&idx); // get block index from server
	void markBlockOccupied();
	void clearBlock();
	DataRequestType receive_request_type();
	void receive_data();	// get a block (1024 bytes) from server
	void send_data();		// send a block to server
	void store_data();		// store a block in the file
	void clear_data(); 		// clear a block in the file
	/* given a block index, find it on the bitmap, return the block */

private:
    int node_soc;
    struct sockaddr_in node_addr;
	std::vector<uint8_t> bitmap;
};

#endif /* NODE_H */
