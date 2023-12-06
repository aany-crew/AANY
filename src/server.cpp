

/*
int Client::initialize(std::string server_ip, uint16_t port)
{
	if ( (server_soc = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		perror("client: socket failed");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) != 1) {
		perror("client: inet_pton failed");
		close(server_soc);
		return -1;
	}

	return 0;
}
 */

#define PORT 8080

int client_soc, server_soc, node_soc;
struct sockaddr_in server_addr, client_addr, node_addr;
socklen_t client_addr_len = sizeof(client_addr);

int main()
{
	if (setup_socket() != 0) {
		ERROR("setup_socket");
		exit(EXIT_FAILURE);
	}

	while (1) {
		LISTEN:
		if ( (client_soc = accept(server_soc, (struct sockaddr *) &client_addr, &client_addr_len)) < 0 ) {
			ERROR("accept");
			continue;
		}

		uint16_t filename_length;
		if (recv(server_soc, &filename_length, sizeof(filename_length), 0) != sizeof(filename_length)) {
			ERROR("recv");
			close(server_soc);
			goto LISTEN;
		}
		std::string filename;
		filename.resize(filename_length);
		if (recv(server_soc, filename.c_str(), filename_length, MSG_WAITALL) != filename_length) {
			ERROR("recv");
			close(server_soc);
			goto LISTEN;
		}
		
		uint8_t operation;
		if (recv(server_soc, &operation, sizeof(operation), 0) != sizeof(operation)) {
			ERROR("recv");
			close(server_soc);
			goto LISTEN;
		}

/*
	This is where the update_map function should go.
	It should take a filename as an input, and output 
	the updated map between IP address and which blocks 
	that IP address stores.
 */

// IP address -> set of block ids		std::string -> std::unordered_set<uint64_t>
// multiple IP addressses

		class Metadata {
			uint64_t num_encodings;
			uint64_t num_blocks;
			std::unordered_map<std::string, std::unordered_set<uint64_t>> mp;
		};

		std::unordered_map<std::string, Metadata> metadata_map;

		update_map(std::string &filename);

		if (inet_pton(AF_INET, node_ip.c_str(), &server_addr.sin_addr) <= 0) {
			ERROR("inet_pton");
			close(socket_fd);
			return -1;
		}

		if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0 ) {
			ERROR("connect");
			return -1;
		}

		switch (operation) {
		case PUT:
			break;
		case GET:
			break;
		case CLEAR:
			break;
		default:
			ERROR("operation");
			close(server_soc);
			goto LISTEN;
		}
	}

	return 0;
}

inline int setup_socket()
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

	if ( (node_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		ERROR("socket");
		return -1;
	}

	node_addr.sin_family = AF_INET;
	node_addr.sin_port = htons(PORT+1);

	return 0;
}
