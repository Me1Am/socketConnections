#include <iostream>
#include <cstring>
#include <iosfwd>
#include <fstream>
#include <memory>

#include <sockConn.h>

int main(int argc, char** argv) {
	std::cout << "start\n";

	char serial[MAX_BUF_SIZE];
	std::fill(std::begin(serial), std::end(serial), 0);
	{	// C String
		char msg[36] = "This is a null-terminated C-string\0";
		std::cout << "Sending: " << msg << '\n';
		Packet* packet = new Packet(12U, msg, sizeof(msg));
		
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		
		char data[out->size];
		char* p = ((char*)out->data);
		for(int i = 0; i < out->size; i++) {
			data[i] = *p;
			p++;
		}
		std::cout << "Receiving: " << data << '\n';
	}
	{	// String
		std::string msg = "This is a string";
		std::cout << "Sending: " << msg << '\n';
		Packet* packet = new Packet(12U, &msg, sizeof(msg));
		
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		
		std::string data = *((std::string*)out->data);
		std::cout << "Receiving: " << data << '\n';
	}
	{	// Int
		int a = 10;
		std::cout << "Sending: " << a << '\n';
		Packet* packet = new Packet(1U, &a, sizeof(a));
		
		std::fill(std::begin(serial), std::end(serial), 0);
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		int data = *((int*)out->data);
		std::cout << "Receiving: " << data << '\n';
	}
	{	// Float
		float a = 3.14159265;
		std::cout << "Sending: " << a << '\n';
		Packet* packet = new Packet(1U, &a, sizeof(a));
		
		std::fill(std::begin(serial), std::end(serial), 0);
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		float data = *((float*)out->data);
		std::cout << "Receiving: " << data << '\n';
	}
	{	// Struct
		struct Tester { int id; char msg[19]; };
		Tester test = { 0b11111111U, "HELLO FROM SERIAL\0" };
		std::cout << "Sending: \"Tester\" struct: " << test.id << ", " << test.msg << '\n';
		Packet* packet = new Packet(1U, &test, sizeof(test));
		
		std::fill(std::begin(serial), std::end(serial), 0);
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		Tester data = *((Tester*)out->data);
		std::cout << "Receiving: \"Tester\" struct: " << test.id << ", " << test.msg << '\n';
	}

	std::cout << "end\n";

	return 0;
}
