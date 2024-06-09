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
	Client* client = scClientCreate("unix_sock.client");
	scClientConnectServer(client, "unix_sock.server");

	char msg[18] = "Element recieved\0";
	Packet* out = new Packet(2U, msg, sizeof(msg));
	scClientSendDataToServer(client, out);

	Packet* in = scClientRecieveDataFromServer(client);
	UIText element = *((UIText*)in->data);
	std::cout 
		<< "Recieved UIText from server: "
		<< "\n  Text: \"" << element.text << '\"'
		<< "\n  Pos: " << element.pos.x << ", " << element.pos.y
		<< "\n  Scale: " << element.scale
		<< "\n  Visible: " << element.visible
	<< '\n';

	scClientDestroy(client);

	return 0;
}
