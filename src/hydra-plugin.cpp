#include "sixense/sixense.h"

#include "sioclient/include/sio_client.h"

#include <sstream>

int main() {

	std::ostringstream oss;
	sio::client ws;
	ws.connect("ws://192.168.11.34:4567/socket.io/?EIO=4&transport=websocket");

	sixenseControllerData data;
	sixenseInit();
	
	int numctrl;

	while (!(numctrl = sixenseGetNumActiveControllers()))
		;


	while(true) {
		for (int i = 0; i < numctrl; i++) {
			sixenseGetNewestData(i, &data);
			oss << "{\"controller\":" << i << ",";
			oss << "\"position\":{";
            oss << "\"x\":" << data.pos[0] << ",";
            oss << "\"y\":" << data.pos[1] << ",";
            oss << "\"z\":" << data.pos[2] << "}}" << std::ends;
			ws.socket()->emit("hydra", oss.str());
			oss.str(std::string());
			oss.clear();
			//std::cerr << data.pos[0] << std::endl;
			//ws->send("5:1::{\"name\":\"hydra\",\"args\":\"bla\"}");
		
		}
	}

}