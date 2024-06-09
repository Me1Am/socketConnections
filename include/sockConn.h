#pragma packed(1)

#include <string>

#ifdef WIN32	// TODO Implement functionality for windows with winsock
	#include <winsock2.h>
	#include "C:\Users\JTK6759\Documents\apps\msys2\ucrt64\include\afunix.h"
#else
	#include <sys/socket.h>
	#include <sys/un.h>
#endif

#define MAX_CLIENTS 10	// Maximum number of client connections for a server
#define MAX_BUF_SIZE 512





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
 * @brief Packet struct, used to pass data between a servers and clients
*/
struct Packet {
	const unsigned int type;
	const size_t size;	// The size of the data
	const void* data;		// Data

	Packet(const unsigned int type, const void* data, const size_t size) : type(type), data(data), size(size) {}
}__attribute__((packed));





/**************************
 *-------< Serial >-------*
 **************************/

/**
 * @brief Serialize a `Packet` struct into a passed pointer
 * @param packet The packet to serialize
 * @param out A pointer for the serialized packet
 * @note Can only serialize packets with primitives as its data
*/
void scPacketSerialize(const Packet* packet, char* out);
/**
 * @brief Deserialize char* to a passed in `Packet` pointer
 * @param in A pointer to a serialized `Packet` struct
 * @return A pointer to the a deserialized `Packet` struct
 * @note Assumes that the serialized packet's data is a primitive
*/
Packet* scPacketDeserialize(char* in);






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
 * @param client The recieving client
 * @param packet A packet to send
*/
void scServerSendDataToClient(ServerClient* client, const Packet* packet);

/**
 * @brief Recieves a packet from a client
 * @param client The sending client, from the `server->clients` array
 * @return A pointer to the received packet
*/
Packet* scServerRecieveDataFromClient(ServerClient* client);

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
 * @param packet A packet to send
*/
void scClientSendDataToServer(Client* client, const Packet* packet);

/**
 * @brief Recieves a packet from the connected server
 * @param client The recieving client
 * @return A pointer to the received packet
*/
Packet* scClientRecieveDataFromServer(Client* client);
