#include "sixense/sixense.h"
#include "protobuf/VRCom.pb.h"
#include "easywsclient.hpp"

#include <iostream>
#include <cassert>
#include <sstream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {

	using easywsclient::WebSocket;
	WebSocket::pointer ws = NULL;
	
	sixenseControllerData data;
	sixenseInit();
	
	int numctrl;

	while (!(numctrl = sixenseGetNumActiveControllers()))
		;

	std::ostringstream msgbuf;
	VRCom::Update* updateMsg = new VRCom::Update();
	VRCom::Hydra* msg = new VRCom::Hydra();
	updateMsg->set_allocated_hydra(msg);
	VRCom::Joystick* joy = new VRCom::Joystick();
	VRCom::Position* pos = new VRCom::Position();
	VRCom::Rotation* rot = new VRCom::Rotation();
	msg->set_allocated_pos(pos);
	msg->set_allocated_rot(rot);
	msg->set_allocated_joy(joy);

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

    std::string websocketAddr = "ws://192.168.11.34:4567";

    if (argc > 1) {
    	websocketAddr = argv[1];
    }

    ws = WebSocket::from_url(websocketAddr);
    assert(ws);

	while(true) {
		for (int i = 0; i < numctrl; i++) {
			sixenseGetNewestData(i, &data);

			//std::cerr << "controller: " << i << std::endl;
			//std::cerr << "pos: " << data.pos[0] << ", " << data.pos[1] << ", " << data.pos[2] << std::endl;
			// std::cerr << "rot: " << data.rot_quat[0] << ", " << data.rot_quat[1] << ", " << data.rot_quat[2] << ", " << data.rot_quat[3] << std::endl;
			// std::cerr << "joystick: " << data.joystick_x << ", " << data.joystick_y << std::endl;
			// std::cerr << "trigger: " << data.trigger << std::endl;
			// std::cerr << "buttons: " << (data.buttons&SIXENSE_BUTTON_BUMPER) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_JOYSTICK) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_1) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_2) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_3) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_4) << " "
			// 						<< (data.buttons&SIXENSE_BUTTON_START) << std::endl;

			msg->set_ctrl_num(i);
			pos->set_x(data.pos[0]);
			pos->set_y(data.pos[1]);
			pos->set_z(data.pos[2]);
			rot->set_x(data.rot_quat[0]);
			rot->set_y(data.rot_quat[1]);
			rot->set_z(data.rot_quat[2]);
			rot->set_w(data.rot_quat[3]);
			joy->set_x(data.joystick_x);
			joy->set_y(data.joystick_y);
			msg->set_trigger(data.trigger);
			msg->set_buttons(data.buttons);

			updateMsg->SerializeToOstream(&msgbuf);
			ws->sendBinary(msgbuf.str());
			ws->poll();

			std::ostringstream().swap(msgbuf);
          	msgbuf.clear();

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			// msg.reset();
			//std::cerr << data.pos[0] << std::endl;
			//ws->send("5:1::{\"name\":\"hydra\",\"args\":\"bla\"}");
		
		}
	}

}