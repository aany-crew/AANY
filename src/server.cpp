#include "server.h"

#ifdef DEBUG
	#define ERROR(function) error_at_line(0, errno, __FILE__, __LINE__, "%s: %s failed", __func__, function)
#else
	#define ERROR(function)
#endif

Server::Server(std::unordered_set<std::string> &ips)
{
	client_soc = -1;
	server_soc = -1;
	node_soc = -1;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	memset(&node_addr, 0, sizeof(struct sockaddr_in));
	client_addr_len = sizeof(client_addr);
	for (const std::string &ip : ips) {
		node_ips.insert(ip);
	}
}

void Server::run()
{
	if (setup_socket() != 0) {
		ERROR("setup_socket");
		return;
	}

	while (1) {
		if ( (client_soc = accept(server_soc, (struct sockaddr *) &client_addr, &client_addr_len)) < 0 ) {
			ERROR("accept");
			continue;
		}

		std::string filename;
		if (get_filename(filename) != 0) {
			ERROR("get_filename");
			close(client_soc);
			continue;
		}
		
		uint8_t op;
		if (recv(server_soc, &op, sizeof(op), 0) != sizeof(op)) {
			ERROR("recv");
			close(client_soc);
			continue;
		}

		switch (op) {
		case PUT:
			put(filename);
			break;
		case GET:
			get(filename);
			break;
		case CLEAR:
			clear(filename);
			break;
		default:
			ERROR("switch");
			close(client_soc);
			continue;
		}
	}

	return;
}

inline int Server::setup_socket()
{
	if ( (server_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		ERROR("socket");
		return -1;
	}

	int opt = 1;
	if (setsockopt(server_soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		ERROR("setsockopt");
		close(server_soc);
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	
	if (bind(server_soc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		ERROR("bind");
		close(server_soc);
		return -1;
	}

	if (listen(server_soc, 3) < 0) {
		ERROR("listen");
		close(server_soc);
		return -1;
	}
/*
	if ( (node_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		ERROR("socket");
		return -1;
	}

	node_addr.sin_family = AF_INET;
	node_addr.sin_port = htons(PORT+1);
*/
	return 0;
}

inline int Server::get_filename(std::string &filename)
{
	uint16_t filename_length;
	if (recv(server_soc, &filename_length, sizeof(filename_length), 0) != sizeof(filename_length)) {
		ERROR("recv");
		return -1;
	}
	filename.resize(filename_length);
	if (recv(server_soc, filename.data(), filename_length, MSG_WAITALL) != filename_length) {
		ERROR("recv");
		return -1;
	}

	return 0;
}

inline int Server::put(std::string &filename)
{
	int64_t file_size;	/* in bits */
	if (recv(client_soc, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
		ERROR("recv");
		return -1;
	}

	// recv blocks of data from client
	uint8_t *compressed_data = (uint8_t *) malloc((file_size >> 3) + ((file_size & 7) != 0));
	if (recv(client_soc, compressed_data, (file_size >> 3) + ((file_size & 7) != 0), MSG_WAITALL) != (file_size >> 3) + ((file_size & 7) != 0)) {
		ERROR("send");
		return -1;
	}

	if (metadata_map.find(filename) != metadata_map.end()) {
		metadata_map.erase(filename);
	}
	metadata_map[filename].num_bits = file_size;
	metadata_map[filename].mp

	return 0;
}

inline int Server::get(std::string &filename)
{
	filename = filename;
	return 0;
}

inline int Server::clear(std::string &filename)
{
	filename = filename;
	return 0;
}

