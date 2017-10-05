/*
 * MyInvitationHandler.cpp
 *
 *  Created on: Sep 1, 2017
 *      Author: user
 */

#include "MyInvitationHandler.h"

MyInvitationHandler::MyInvitationHandler() : InvitationHandler(){
	this->game= NULL;

}

MyInvitationHandler::~MyInvitationHandler() {
	// TODO Auto-generated destructor stub
}

//uses the game class to handle invitation
bool MyInvitationHandler::handleInvitation(std::string username,TCPSocket* userSock) {
	if(this->game == NULL) return false;

	if(this->game->handleInvitation(username) ){
		this->acceptInvitation(userSock);
		return true;
	} else {
		this->declineInvitation(userSock);
		return false;
	}
}

//interrupt waiting for auto game
void MyInvitationHandler::acceptInvitation(TCPSocket* userSock) {
	userSock->write(GAME_ACCEPTED);
	game->interrupt();
}

void MyInvitationHandler::declineInvitation(TCPSocket* userSock) {
	userSock->write(GAME_REFUSED);
}

void MyInvitationHandler::activateAfterGameAccepted() {
	game->handleGame();
}
