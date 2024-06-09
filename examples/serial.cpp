#include <iostream>
#include <cstring>
#include <iosfwd>
#include <fstream>
#include <memory>

#include <sockConn.h>

template<typename T>
struct Vec2D {
	T x;
	T y;
};
typedef Vec2D<float> Vec2Df;

struct UIText {
	char text[64];
	
	Vec2Df pos;	
	float scale;
	bool visible;
};

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
		UIText* element = new UIText({ "SENDIN'\0", { 50.f, 50.f }, 1.f, true });
		std::cout 
			<< "Recieved UIText from server: "
			<< "\n  Text: \"" << element->text << '\"'
			<< "\n  Pos: " << element->pos.x << ", " << element->pos.y
			<< "\n  Scale: " << element->scale
			<< "\n  Visible: " << element->visible
		<< '\n';
		Packet* packet = new Packet(1U, element, sizeof(*element));

		std::fill(std::begin(serial), std::end(serial), 0);
		scPacketSerialize(packet, serial);

		Packet* out = scPacketDeserialize(serial);

		UIText data = *((UIText*)out->data);
		std::cout 
			<< "Recieved UIText from server: "
			<< "\n  Text: \"" << data.text << '\"'
			<< "\n  Pos: " << data.pos.x << ", " << data.pos.y
			<< "\n  Scale: " << data.scale
			<< "\n  Visible: " << data.visible
		<< '\n';
	}

	std::cout << "end\n";

	return 0;
}
