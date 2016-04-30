#include "sixense/sixense.h"

#include "sioclient/include/sio_client.h"

#include <sstream>
#include <chrono>
#include <thread>

int main() {

	std::ostringstream oss;
	sio::client ws;
	ws.connect("ws://192.168.11.34:4567/socket.io/?EIO=4&transport=websocket");

	sixenseControllerData data;
	sixenseInit();
	
	int numctrl;

	while (!(numctrl = sixenseGetNumActiveControllers()))
		;

	std::string ctrlname = "hydra";
	sio::message::ptr msg = sio::array_message::create();
	while(true) {
		for (int i = 0; i < numctrl; i++) {
			sixenseGetNewestData(i, &data);
			msg = sio::array_message::create();
			std::vector<sio::message::ptr>& vec = msg->get_vector();
          	vec.push_back(sio::string_message::create(ctrlname+std::to_string(i)));
          	vec.push_back(sio::double_message::create(data.pos[0]));
          	vec.push_back(sio::double_message::create(data.pos[1]));
          	vec.push_back(sio::double_message::create(data.pos[2]));
			ws.socket()->emit("obj", msg);
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			// msg.reset();
			//std::cerr << data.pos[0] << std::endl;
			//ws->send("5:1::{\"name\":\"hydra\",\"args\":\"bla\"}");
		
		}
	}

}