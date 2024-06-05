#include <iostream>

#include "sockConn.h"

int main(int argc, char** argv) {
	Server* server = scServerCreate(SERVER_SOCK_PATH);
	scServerConnectionOpen(server);
	scServerConnectionAccept(server);
	scServerSendDataToClient(server, server->clients[0], SERVER_MSG);
	
	char buf[256];
	std::fill(std::begin(buf), std::end(buf), 0);
	scServerRecieveDataFromClient(server, server->clients[0], buf);
	std::cout << buf << '\n';

	scServerDestroy(server);

	return 0;
}