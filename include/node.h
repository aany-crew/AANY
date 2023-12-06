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
	int receive_request_type();
	int receive_data();
	int send_data();
	int store_data();

private:
    int node_soc;
    struct sockaddr_in node_addr;
};

#endif /* NODE_H */
