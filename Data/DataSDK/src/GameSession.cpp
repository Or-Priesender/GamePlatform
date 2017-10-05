/*
 * GameSession.cpp
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#include "GameSession.h"

GameSession::GameSession(GameUser* one,GameUser* two) {
	this->playerOne = one;
	this->playerTwo = two;
}

GameSession::~GameSession() {
	// TODO Auto-generated destructor stub
}

