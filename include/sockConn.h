#include <string>

#ifdef WIN32	// TODO Implement functionality for windows with winsock
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <sys/un.h>
#endif

#define MAX_CLIENTS 10	// Maximum number of client connections for a server

// DEMO defines
#define SERVER_SOCK_PATH "unix_sock.server"
#define CLIENT_SOCK_PATH "unix_sock.client"
#define SERVER_MSG "HELLO FROM SERVER"
#define CLIENT_MSG "HELLO FROM CLIENT"





/**
 * @brief Client struct for `Server` structs
 * @attention This struct should not be altered
*/
struct ServerClient {
	sockaddr_un addr;	
	int clientFD;
};

/**
 * @brief Independent server struct, 
 * @brief holds a host socket address, socket descriptor, and array of `ServerClient` structs
*/
struct Server {
	sockaddr_un addr;
	int socket;

	ServerClient* clients[MAX_CLIENTS];
};

/**
 * @brief Independent client struct, 
 * @brief holds a host socket address and socket descriptor
*/
struct Client {
	sockaddr_un addr;
	int socket;
};

/**
 * @brief Packet struct, used to pass data between a servers and clients, 
 * 
 * @attention NOT IMPLEMENTED
*/// TODO Fully implement, including serializing and deserializing
struct Packet {
	const void* data;	// Data
	const size_t size;	// The size of the data

	Packet(const void* data, const size_t size) : data(data), size(size) {}
};





/**************************
 *-------< Server >-------*
 **************************/

/**
 * @brief Create a server
 * @param socketPath The path for the socket file to live
 * @return A pointer to the constructed server struct
*/
Server* scServerCreate(const std::string& socketPath);

/**
 * @brief Sends a packet to a client
 * @param server The server for the data to originate from
 * @param client The recieving client
 * @param data The data to be sent
*/
void scServerSendDataToClient(Server* server, ServerClient* client, const void* data);

/**
 * @brief Recieves a packet from a client
 * @param server The server accepting the data
 * @param client The sending client, from the `server->clients` array
 * @param buf The buffer to write into
*/
void scServerRecieveDataFromClient(Server* server, ServerClient* client, char* buf);

/**
 * @brief Open server for connections
 * @param server The server to open connections to
*/
void scServerConnectionOpen(Server* server);

/**
 * @brief Accept a connection request for the given server
 * @param server The server to accept the connection to
*/
void scServerConnectionAccept(Server* server);

/**
 * @brief Close the server and dispose of its clients
 * @param server Server to destroy
 * @note Calls delete on the server pointer
*/
void scServerDestroy(Server* server);





/**************************
 *-------< Client >-------*
 **************************/

/**
 * @brief Create a client
 * @param socketPath The path for the socket file to live
 * @return A pointer to the constructed client struct
*/
Client* scClientCreate(const std::string& socketPath);

/**
 * @brief Close the client
 * @param client Server to destroy
 * @note Calls delete on the client pointer
*/
void scClientDestroy(Client* client);

/**
 * @brief Connect to a server
 * @param client The client to open a connection from
 * @param socketPath The path to the server's socket
*/
void scClientConnectServer(Client* client, const std::string& socketPath);

/**
 * @brief Sends a packet to the connected server
 * @param client The client to send the data from
 * @param data The data to be sent
*/
void scClientSendDataToServer(Client* client, const void* data);

/**
 * @brief Recieves a packet from the connected server
 * @param client The recieving client
 * @param buf The buffer to write into
*/
void scClientRecieveDataFromServer(Client* client, char* buf);
