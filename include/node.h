#ifndef NODE_H
#define NODE_H

#include <string>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    int get();
    int put();
    int clear();

private:
    int node_soc;
    struct sockaddr_in node_addr;
};

#endif /* NODE_H */
