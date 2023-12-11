#include "server.h"

int main()
{
	std::unordered_set<std::string> ips;
	ips.insert("127.0.0.1");

	Server server(ips);

	server.run();

	return 0;
}
