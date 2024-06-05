#include <iostream>

#include "../include/sockConn.h"

#define MAX_BUF_SIZE 512

/**
 * @brief Serialize a `Packet` struct into a passed pointer
 * @param packet The packet to serialize
 * @param out A pointer for the serialized packet
 * @note Can only serialize packets with primitives as its data
*/
void scPacketSerialize(Packet* packet, char* out) {
	int *q = (int*)out;
	
	*q = packet->type;
	q++;
	*q = packet->size;
	q++;

	// Copy data from packet to "out"
	char* p = (char*)q;
	char data[sizeof(packet->data)];
	std::copy(
		static_cast<const char*>(static_cast<const void*>(&packet->data)),
		static_cast<const char*>(static_cast<const void*>(&packet->data)) + sizeof(packet->data),
		data
	);
	for(int i = 0; i < MAX_BUF_SIZE; i++) {
		*p = data[i];
		p++;
	}
}

/**
 * @brief Deserialize char* to a passed in `Packet` pointer
 * @param in A pointer to a serialized `Packet` struct
 * @param packet A pointer to the `Packet` struct to write into
 * @note Assumes that the serialized packet's data is a primitive
*/
void scPacketDeserialize(char* in, Packet* packet) {
	int *q = (int*)in;
	
	packet->type = *q;
	q++;
	packet->size = *q;
	q++;

	// Copy data from "in" to the packet
	char* p = (char*)q;
	for(int i = 0; i < MAX_BUF_SIZE; i++) {
		((char*)packet->data)[i] = *p;
		p++;
	}
}