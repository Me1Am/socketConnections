#include <iostream>

#include "../include/sockConn.h"

void scPacketSerialize(Packet* packet, char* out) {
	unsigned int *q = (unsigned int*)out;
	
	*q = packet->type;
	q++;
	*q = packet->size;
	q += 2;	// size_t will be 64bit

	// Copy data from packet to "out"
	char* p = (char*)q;	// "8bit" pointer
	for(int i = 0; i < packet->size; i++) {
		*p = ((char*)packet->data)[i];	// Read one byte
		p++;
	}
}

Packet* scPacketDeserialize(char* in) {
	unsigned int *q = (unsigned int*)in;

	unsigned int type = *q;
	q++;
	size_t size = *q;
	q += 2;

	char data[size];
	// Copy data from "in" to the packet
	char* p = (char*)q;
	for(int i = 0; i < size; i++) {
		data[i] = *p;
		p++;
	}

	return new Packet(type, data, size);
}