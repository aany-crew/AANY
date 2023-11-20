#ifndef CLIENT_H
#define CLIENT_H

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

class Client {
public:
        Client();
        int initialize(std::string server_ip, uint16_t port);
        int put(std::string filename);
        int get(std::string filename);
        int clear(std::string filename);
private:
        int server_soc;
	struct sockaddr_in server_addr;
};

#endif /* CLIENT_H */
