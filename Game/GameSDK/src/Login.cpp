/*
 * Login.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#include "Login.h"

GameUser* Login::registerOrLoginUser(std::string username, std::string password,std::string action) {
	char buffer[100];
	int result = 0;
	int cmd;

	//connect to the server's login listener
	TCPSocket* socket = new TCPSocket(SERVER_IP, SDK_PORT);

	//login or register
	if (action.compare("login") == 0) {
		cmd = LOGIN;
	} else if (action.compare("register") == 0)
		cmd = REGISTER;
	else
		return NULL;

	//write details to the server
	socket->write(cmd);
	sprintf(buffer, "%s:%s", username.c_str(), password.c_str());
	socket->write(strlen(buffer));
	socket->write(buffer, strlen(buffer));

	//perform handshake with server
	socket->read(&result);
	if (result == LOGIN_FAILED || result == REGISTER_FAILED)
		return NULL;

	else if (result == LOGIN_APPROVED) {
		socket->read(&result);
		if (result == MOVE_TO_NEW_PORT) {
			//reading the new port
			socket->read(&result);
			sleep(1);
			//the port we got is logical
			if (result < 25000 && result > 1000) {
				//connect to the new port
				TCPSocket* newSock = new TCPSocket(SERVER_IP, result);
				if (newSock != NULL) { //connection to new port was successful
					socket->close();
					return new GameUser(username, newSock);
				} else
					return NULL;

			} else
				return NULL;
		}

	} else
		return NULL;
}

