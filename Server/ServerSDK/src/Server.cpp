/*
 * Server.cpp
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#include "Server.h"

Server::Server() {
	users = new UserHandler();
	auth = new Authenticator(users);
	listener = new CommandListener(SDK_PORT, auth);

}

Server::~Server() {

	delete users;
	delete listener;
	delete auth;

}

void Server::run() {
	listener->start();
	cout << "Server has started listening" << endl;
}

//wait for the listener thread
void Server::waitForThread() {
	MThread::waitForThread();
	listener->waitForThread();
}

//print the users list
void Server::printUsers() {
	vector<GameUser*> users = this->users->listUsers();
	for (unsigned int i = 0; i < users.size(); i++) {
		cout << users[i]->getUsername() << endl;
	}
}
