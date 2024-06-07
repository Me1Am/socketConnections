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

void scServerSendDataToClient(Server* server, ServerClient* client, const void* data) {
	Packet packet = Packet(0U, data, sizeof(data));
	
	char buf[256];
	std::fill(std::begin(buf), std::end(buf), 0);
	strcpy(buf, "HELLO FROM SERVER");

	int status = send(client->clientFD, buf, strlen(buf), 0);
	if(status == -1)
		std::cerr << "SERVER: Unable to send message to client.\n";
}

void scServerRecieveDataFromClient(Server* server, ServerClient* client, char* buf) {
	int responseStatus = recv(client->clientFD, buf, sizeof(char[256]), 0);
		if(responseStatus == -1){
			std::cerr << "SERVER: Error when receiving message: " << strerror(errno) << '\n';
			#ifdef DEBUG	// Error messages pulled from the man page
				switch(errno) {
					case EAGAIN:	// EWOULDBLOCK
						std::cerr << "The socket is nonblocking, or a receive timeout was set and the timeout expired\n";
						break;
					case EBADF:
						std::cerr << "Invalid file descriptor for the argument \"sockfd\"\n";
						break;
					case ECONNREFUSED:
						std::cerr << "Connection refused\n";
						break;
					case EFAULT:
						std::cerr << "The receive buffer pointer(s) point outside the process's address space\n";
						break;
					case EINTR:
						std::cerr << "The receive was interrupted by delivery of a signal before any data was available, see man page: signal(7)\n";
						break;
					case EINVAL:
						std::cerr << "Invalid argument\n";
						break;
					case ENOMEM:
						std::cerr << "Could not allocate memory for recvmsg()\n";
						break;
					case ENOTCONN:
						std::cerr << "The socket is associated with a connection-oriented protocol and has not been connected, see man pages: connect(2) and accept(2)\n";
						break;
					case ENOTSOCK:
						std::cerr << "The \"sockfd\" file descriptor does not refer to a socket\n";
						break;
				}
			#endif
	}
}