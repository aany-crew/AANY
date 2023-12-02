#ifndef NODE_H
#define NODE_H

#include <string>
#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BLOCK_SIZE 1024

enum operation {
	PUT,
	GET,
	CLEAR
};

class Node {
public:
        Node();
        int initialize(std::string server_ip, uint16_t port);
        int reveive_data(std::string filename); //put
        int send_data(std::string filename); // get
        int store_data(std::string filename); // clear
private:
        int server_soc;
	    struct sockaddr_in server_addr;
};

#endif /* NODE_H */