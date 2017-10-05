/*
 * GameUser.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#include "GameUser.h"

GameUser::GameUser(std::string username,TCPSocket* socket) {
	this->username = username;
	this->socket = socket;
	inSession = false;
}

GameUser::~GameUser() {
	delete socket;
}

TCPSocket* GameUser::getSocket() {
	return this->socket;
}

void GameUser::setSocket(TCPSocket* socket) {
	this->socket = socket;
}

std::string GameUser::getUsername() {
	return username;
}

void GameUser::setInSession(bool b) {
	inSession = b;
}

bool GameUser::isInSession() {
	return inSession;
}
