#include <iostream>

#include <sockConn.h>

int main(int argc, char** argv) {
	Client* client = scClientCreate("unix_sock.client");
	scClientConnectServer(client, "unix_sock.server");
	
	const char msg[19] = "HELLO FROM CLIENT\0";
	scClientSendDataToServer(client, msg);
	
	char buf[256];
	std::fill(std::begin(buf), std::end(buf), 0);
	scClientRecieveDataFromServer(client, buf);
	std::cout << buf << '\n';

	scClientDestroy(client);

	return 0;
}
