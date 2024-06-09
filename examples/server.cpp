#include <iostream>

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
	Server* server = scServerCreate("unix_sock.server");
	scServerConnectionOpen(server);
	scServerConnectionAccept(server);

	UIText* element = new UIText({ "HELLO FROM SERVER\0", { 50.f, 50.f }, 1.f, true });
	Packet* out = new Packet(1U, element, sizeof(*element));
	scServerSendDataToClient(server->clients[0], out);

	Packet* in = scServerRecieveDataFromClient(server->clients[0]);
	char msg[18];
	strcpy(msg, ((char*)(in->data)));
	std::cout << msg << '\n';

	scServerDestroy(server);

	return 0;
}