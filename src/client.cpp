#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>

#include "../include/sockConn.h"

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
	remove(client->addr.sun_path);

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

void scClientSendDataToServer(Client* client, const Packet* packet) {
	char buf[MAX_BUF_SIZE];
	std::fill(std::begin(buf), std::end(buf), 0);
	scPacketSerialize(packet, buf);
	
	int status = send(client->socket, buf, MAX_BUF_SIZE, 0);
	if(status == -1)
		std::cerr << "CLIENT: Send error. " << strerror(errno) <<'\n';
}

Packet* scClientRecieveDataFromServer(Client* client) {
	char buf[MAX_BUF_SIZE];
	int responseStatus = recv(client->socket, buf, MAX_BUF_SIZE, 0);
	if(responseStatus == -1)
		std::cerr << "CLIENT: Error when recieving message: " << strerror(errno) << '\n';

	return scPacketDeserialize(buf);
}