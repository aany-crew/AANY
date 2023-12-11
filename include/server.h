#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

enum operation {
	PUT,
	GET,
	CLEAR
};

struct Metadata {
	int64_t num_bits;
	std::map<uint8_t, unordered_set<std::string>> block_map;
};

class Server {
public:
	Server(std::unordered_set<std::string> &ips);

	void run();

private:
	int client_soc, server_soc, node_soc;
	struct sockaddr_in server_addr, client_addr, node_addr;
	socklen_t client_addr_len;

	std::unordered_map<std::string, Metadata> metadata_map;
	std::unordered_set<std::string> node_ips;

	int setup_socket();
	int get_filename(std::string &filename);
	int put(std::string &filename);
	int get(std::string &filename);
	int clear(std::string &filename);
};

#endif /* SERVER_H */
