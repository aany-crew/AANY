#include "client.h"
#include "Text.h"
#include "GenerateHuffmanCodes.h"
#include "HuffmanTreeNode.h"
#include "BuildHuffmanTree.h"
#include <memory>

#ifdef DEBUG
	#define ERROR(function) error_at_line(0, errno, __FILE__, __LINE__, "%s: %s failed", __func__, function);
#else
	#define ERROR(function)
#endif

Client::Client() {
        server_soc = 0;
	memset(&server_addr, 0, sizeof(server_addr));
}

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

int Client::put(std::string filename)
{
	// check if file exists locally
	if (access(filename.c_str(), R_OK) == -1) {
		ERROR("access");
		return -1;
	}

	// connect to server
	if (connect(server_soc, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		ERROR("connect");
		return -1;
	}

	// send file name to server
	uint16_t filename_length = filename.size();
	if (send(server_soc, &filename_length, sizeof(filename_length), 0) != sizeof(filename_length)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}
	if (send(server_soc, filename.c_str(), filename_length, 0) != filename_length) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// tell server this is a put request
	uint8_t operation = PUT;
	if (send(server_soc, &operation, sizeof(operation), 0) != sizeof(operation)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// open local file and get length in bytes
	int32_t in_file;
	if ( (in_file = open(filename.c_str(), O_RDONLY, 0)) == -1 ) {
		ERROR("open");
		close(server_soc);
		return -1;
	}
	struct stat file_stats;
	if (fstat(in_file, &file_stats) == -1) {
		ERROR("fstat");
		close(in_file);
		close(server_soc);
		return -1;
	}
	uint64_t file_size = file_stats.st_size;

	std::unordered_map<uint8_t, uint64_t> freq_map;
	uint8_t data_block[BLOCK_SIZE] = {0};
	for (uint64_t bytes_read = 0; bytes_read < file_size; ++bytes_read) {
		if (read(file_size, data_block, BLOCK_SIZE) != BLOCK_SIZE) {
			ERROR("read");
			close(in_file);
			close(server_soc);
			return -1;
		}
		GetCharFrequency(freq_map, data_block);
	}
	if (lseek(in_file, 0, SEEK_SET) != 0) {
		ERROR("lseek");
		close(in_file);
		close(server_soc);
		return -1;
	}

	// Build tree and generate codes
	std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(freq_map);
	std::unordered_map<char, std::string> HuffmanCodes = GenerateHuffmanCodes(root);

/* BEGINNING OF FILE
uint64_t N
uint8_t huffman_elements[N]
huffman_elements[0]
huffman_elements[1]
huffman_elements[2]
.
.
.
huffman_elements[N]
START OF COMPRESSED DATA
 */

	std::vector<bool> compressed_data;
	for (uint64_t bytes_read = 0; bytes_read < file_size; ++bytes_read) {
		if (read(file_size, data_block, BLOCK_SIZE) != BLOCK_SIZE) {
			ERROR("read");
			close(in_file);
			close(server_soc);
			return -1;
		}
		CompressText(root, compressed_data, data_block);
	}

	file_size = compressed_data.size();

	// let server know how many bits the file is
	if (send(server_soc, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
		ERROR("send");
		close(in_file);
		close(server_soc);
		return -1;
	}

	// send blocks of data to server
	uint8_t byte = 0;
	uint32_t count;
	for (count = 0; count < file_size; ++count) {
		if (count % 8 == 0) {
			byte <<= 1;
			byte |= compressed_data[count];
		} else {
			if (send(server_soc, &byte, sizeof(byte), MSG_MORE) != sizeof(byte)) {
				ERROR("send");
				close(in_file);
				close(server_soc);
				return -1;
			}
			byte = 0;
		}
	}
	if (send(server_soc, &byte, sizeof(byte), 0) != sizeof(byte)) {
		ERROR("send");
		close(in_file);
		close(server_soc);
		return -1;
	}

	// confirm with server this operation is a success
	uint8_t confirmation = 0;
	if (recv(server_soc, &confirmation, sizeof(confirmation), 0) != sizeof(confirmation) || confirmation != 1) {
		ERROR("confirm");
		close(in_file);
		close(server_soc);
		return -1;
	}

	// close file descriptors
	close(in_file);
	close(server_soc);

	return 0;
}

int Client::get(std::string filename)
{
	// connect to server
	if (connect(server_soc, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		ERROR("connect");
		return -1;
	}

	// send file name to server
	uint16_t filename_length = filename.size();
	if (send(server_soc, &filename_length, sizeof(filename_length), 0) != sizeof(filename_length)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}
	if (send(server_soc, filename.c_str(), filename_length, 0) != filename_length) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// tell server this is a get request
	uint8_t operation = GET;
	if (send(server_soc, &operation, sizeof(operation), 0) != sizeof(operation)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// receive from server how many bytes the file is
	uint64_t file_size;
	if (recv(server_soc, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// open local file
	int32_t out_file;
	if ( (out_file = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0700)) == -1 ) {
		ERROR("open");
		close(server_soc);
		return -1;
	}

	// receive blocks of data from server
	uint8_t data_block[BLOCK_SIZE];
	for (; file_size > BLOCK_SIZE; file_size -= BLOCK_SIZE) {
		if (recv(server_soc, data_block, BLOCK_SIZE, MSG_WAITALL) != BLOCK_SIZE) {
			ERROR("send");
			close(out_file);
			close(server_soc);
			return -1;
		}
		if (write(out_file, data_block, BLOCK_SIZE) != BLOCK_SIZE) {
			ERROR("read");
			close(out_file);
			close(server_soc);
			return -1;
		}
	}
	if (recv(server_soc, data_block, file_size, MSG_WAITALL) != (int64_t) file_size) {
		ERROR("send");
		close(out_file);
		close(server_soc);
		return -1;
	}
	if (write(out_file, data_block, file_size) != (int64_t) file_size) {
		ERROR("read");
		close(out_file);
		close(server_soc);
		return -1;
	}

	// confirm with server this operation is a success
	uint8_t confirmation = 0;
	if (recv(server_soc, &confirmation, sizeof(confirmation), 0) != sizeof(confirmation) || confirmation != 1) {
		ERROR("confirm");
		close(out_file);
		close(server_soc);
		return -1;
	}

	// close file descriptors
	close(out_file);
	close(server_soc);

	return 0;
}

int Client::clear(std::string filename)
{
	// connect to server
	if (connect(server_soc, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		ERROR("connect");
		return -1;
	}

	// send file name to server
	uint16_t filename_length = filename.size();
	if (send(server_soc, &filename_length, sizeof(filename_length), 0) != sizeof(filename_length)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}
	if (send(server_soc, filename.c_str(), filename_length, 0) != filename_length) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// tell server this is a clear request
	uint8_t operation = CLEAR;
	if (send(server_soc, &operation, sizeof(operation), 0) != sizeof(operation)) {
		ERROR("send");
		close(server_soc);
		return -1;
	}

	// confirm with server this operation is a success
	uint8_t confirmation = 0;
	if (recv(server_soc, &confirmation, sizeof(confirmation), 0) != sizeof(confirmation) || confirmation != 1) {
		ERROR("confirm");
		close(server_soc);
		return -1;
	}

	// close file descriptor
	close(server_soc);

	return 0;
}

