#include "client.h"

int main()
{
	Client client;
	client.initialize("127.0.0.1", 8080);

	return 0;
}
