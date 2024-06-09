#include <iostream>

#include "../include/sockConn.h"

void scPacketSerialize(const Packet* packet, char* out) {
	unsigned int *q = (unsigned int*)out;
	
	*q = packet->type;
	q++;
	*q = packet->size;
	q += 2;	// size_t will be 64bit

	// Copy data from packet to "out"
	char* p = (char*)q;	// "8bit" pointer
	memcpy(p, packet->data, packet->size);
}

Packet* scPacketDeserialize(char* in) {
	unsigned int *q = (unsigned int*)in;

	unsigned int type = *q;
	q++;
	size_t size = *q;
	q += 2;

	// Copy data from "in" to the packet
	char* data = new char[size];
	char* p = (char*)q;
	memcpy(data, p, size);

	return new Packet(type, data, size);
}