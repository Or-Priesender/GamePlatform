/*
 * CommandListener.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#include "CommandListener.h"

//initialize the server socket
CommandListener::CommandListener(int port,Authenticator* auth) {
	socket = new TCPSocket(port);
	this->auth = auth;
	stop = false;
}

CommandListener::~CommandListener() {
	this->stopListening();
	socket->close();
	delete socket;
	delete auth;
}

//listen to incoming connections
void CommandListener::run() {
	int command;
	int size;
	char *password,*username;
	std::string usernameStr,passStr;
	char buffer[100] = {0};
	bool result = false;
	TCPSocket* ready;

	while(!stop && (ready=socket->listenAndAccept())!=NULL){
		//reads the command from the user
		ready->read(&command);
		ready->read(&size);
		if(size > 0){
			memset(buffer,0,100);
			ready->read(buffer,size);
		}
		if(command == REGISTER || command == LOGIN){
			//seperate the username and password
			username = strtok(buffer,":");
			password = strtok(NULL,":");
			if(strlen(username) > 0)
				usernameStr.assign(username,strlen(username));
			if(strlen(password) > 0)
				passStr.assign(password,strlen(password));
		}
		switch(command){
		case REGISTER:
			result = auth->registerUser(usernameStr,passStr,ready);
			break;
		case LOGIN:
			result = auth->loginUser(usernameStr,passStr,ready);
			break;
		}

	}
}

void CommandListener::stopListening() {
	stop = true;
	this->waitForThread();

}


