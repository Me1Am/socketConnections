#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>

#include "../include/socketConnections.h"

Client* scClientCreate(const std::string& socketPath) {
	Client* client = new Client();
	
	// Open a client socket
	client->socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(client->socket == -1){
		std::cerr << "CLIENT: Socket error: " << strerror(errno) << '\n';
		return nullptr;
	}

	// Bind client address to filesystem
	client->addr.sun_family = AF_UNIX;
	strcpy(client->addr.sun_path, socketPath.c_str());

	unlink(socketPath.c_str());

	int len = sizeof(client->addr);
	int status = bind(client->socket, (sockaddr*)&client->addr, len);
	if(status == -1){
		std::cerr << "CLIENT: Client binding error. " << strerror(errno) << '\n';
		close(client->socket);
		return nullptr;
	}

	return client;
}

void scClientDestroy(Client* client) {
	close(client->socket);
	remove(CLIENT_SOCK_PATH);

	delete client;
}

void scClientConnectServer(Client* client, const std::string& socketPath) {
	sockaddr_un serverAddr;
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, socketPath.c_str());
	
	int len = sizeof(client->addr);
	int status = connect(client->socket, (sockaddr*)&serverAddr, len);
	if(status == -1)
		std::cerr << "CLIENT: Connect error. " << strerror(errno) << '\n';
	
	#ifdef DEBUG
		std::cout << "CLIENT: Connected to server at: " << serverAddr.sun_path << '\n';
	#endif
}

void scClientSendDataToServer(Client* client, const void* data) {
	char buf[256];
	std::fill(std::begin(buf), std::end(buf), 0);
	strcpy(buf, CLIENT_MSG);
	
	int status = send(client->socket, &buf, sizeof(buf), 0);
	if(status == -1)
		std::cerr << "CLIENT: Send error. " << strerror(errno) <<'\n';
}

void scClientRecieveDataFromServer(Client* client, char* buf) {
	int responseStatus = recv(client->socket, buf, sizeof(char[256]), 0);
	if(responseStatus == -1)
		std::cerr << "CLIENT: Error when recieving message: " << strerror(errno) << '\n';
}

int main(int argc, char** argv) {
	Client* client = scClientCreate(CLIENT_SOCK_PATH);
	scClientConnectServer(client, SERVER_SOCK_PATH);
	scClientSendDataToServer(client, CLIENT_MSG);
	
	char buf[256];
	std::fill(std::begin(buf), std::end(buf), 0);
	scClientRecieveDataFromServer(client, buf);
	std::cout << buf << '\n';

	scClientDestroy(client);

	return 0;
}
