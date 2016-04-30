#include "sixense/sixense.h"
#include "easywsclient.hpp"
#include <cassert>
#include <sstream>
#include <chrono>
#include <thread>

int main() {

	using easywsclient::WebSocket;
	WebSocket::pointer ws = NULL;
	std::ostringstream oss;
	oss.precision(7);
	

	sixenseControllerData data;
	sixenseInit();
	
	int numctrl;

	while (!(numctrl = sixenseGetNumActiveControllers()))
		;

    // open the websocket server connection
    #ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
      printf("WSAStartup Failed.\n");
      return 1;
    }
    #endif

    ws = WebSocket::from_url("ws://192.168.11.34:4567/socket.io/?EIO=4&transport=websocket");
    assert(ws);

	while(true) {
		for (int i = 0; i < numctrl; i++) {
			sixenseGetNewestData(i, &data);
			oss << "42[\"obj\"]";
			// oss << "42[\"obj\",[\"hydra" << i << "\",";
			// oss << data.pos[0] << ",";
			// oss << data.pos[1] << ",";
			// oss << data.pos[2] << "]]";
			ws->send(oss.str());
			ws->poll();
			std::ostringstream().swap(oss);
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			// msg.reset();
			//std::cerr << data.pos[0] << std::endl;
			//ws->send("5:1::{\"name\":\"hydra\",\"args\":\"bla\"}");
		
		}
	}

}