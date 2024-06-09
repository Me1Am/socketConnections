#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>

#include "../include/sockConn.h"

Server* scServerCreate(const std::string& socketPath) {
	Server* server = new Server();

	// Open server socket as SOCK_STREAM
	server->socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(server->socket == -1){
		std::cerr << "SERVER: Error when opening server socket.\n";
		return nullptr;
	}

	// Bind to an address on file system
	server->addr.sun_family = AF_UNIX;
	strcpy(server->addr.sun_path, socketPath.c_str());

	unlink(socketPath.c_str());	// Unlink the file so it can be bound

	int status = bind(server->socket, (sockaddr*)&server->addr, sizeof(server->addr));
	if(status == -1){
		std::cerr << "SERVER: Server bind error: " << strerror(errno) << '\n';
		close(server->socket);
		return nullptr;
	}

	std::fill(server->clients, server->clients+MAX_CLIENTS, nullptr);
	return server;
}

void scServerDestroy(Server* server) {
	close(server->socket);
	for(int i = 0; i < MAX_CLIENTS; i++) {
		if(server->clients[i] != nullptr){
			close(server->clients[i]->clientFD);
			delete server->clients[i];
		}
	}
	remove(server->addr.sun_path);	// Delete socket file

	delete server;
}

void scServerConnectionOpen(Server* server) {
	// Listen and accept client connection
	// Set the server in the "listen" mode and maximum pending connected clients in queue
	int status = listen(server->socket, MAX_CLIENTS);
	if(status == -1)
		std::cerr << "SERVER: Listen error: " <<  strerror(errno) << '\n';
	
	#ifdef DEBUG
		std::cout << "SERVER: Socket listening...\n";
	#endif
}

void scServerConnectionAccept(Server* server) {
	int len = sizeof(server->addr);

	// Get pointer to next unused client spot
	auto firstOpen = std::find(server->clients, server->clients+MAX_CLIENTS, nullptr);
	if(firstOpen == server->clients+MAX_CLIENTS) throw std::runtime_error("Server already has the maximum number of clients connected");
	int index = std::distance(server->clients, firstOpen);
	ServerClient** client = &(server->clients[index]);
	(*client) = new ServerClient();

	#ifdef WIN32
		(*client)->clientFD = accept(server->socket, (sockaddr*)&(*client)->addr, (int*)&len);
	#else
		(*client)->clientFD = accept(server->socket, (sockaddr*)&(*client)->addr, (socklen_t*)&len);
	#endif
	if((*client)->clientFD == -1){
		std::cerr << "SERVER: Accept error: " << strerror(errno) << '\n';
		close(server->socket);
		close((*client)->clientFD);
		exit(1);
	}
	
	#ifdef DEBUG
		std::cout << "SERVER: Connected to client at: " << server->clients[index]->addr.sun_path << '\n';
	#endif
}

void scServerSendDataToClient(ServerClient* client, const Packet* packet) {
	char buf[MAX_BUF_SIZE];
	std::fill(std::begin(buf), std::end(buf), 0);
	scPacketSerialize(packet, buf);

	int status = send(client->clientFD, buf, MAX_BUF_SIZE, 0);
	if(status == -1)
		std::cerr << "SERVER: Send error. " << strerror(errno) <<'\n';
}

Packet* scServerRecieveDataFromClient(ServerClient* client) {
	char buf[MAX_BUF_SIZE];
	int responseStatus = recv(client->clientFD, buf, MAX_BUF_SIZE, 0);
	if(responseStatus == -1)
		std::cerr << "SERVER: Error when receiving message: " << strerror(errno) << '\n';
	
	return scPacketDeserialize(buf);
}