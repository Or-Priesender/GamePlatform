/*
 * MyInvitationHandler.h
 *
 *  Created on: Sep 1, 2017
 *      Author: user
 */

#ifndef MYINVITATIONHANDLER_H_
#define MYINVITATIONHANDLER_H_

#include "InvitationHandler.h"
#include "TCPSocket.h"
#include "MyGame.h"
#include "MThread.h"

using namespace std;
using namespace Lab;

//implementation of the invitation handler interface
class MyInvitationHandler : public InvitationHandler{
	MyGame* game;
public:
	MyInvitationHandler();
	virtual ~MyInvitationHandler();
	bool handleInvitation(std::string username,TCPSocket* userSock);
	void acceptInvitation(TCPSocket* userSock);
	void declineInvitation(TCPSocket* userSock);
	void activateAfterGameAccepted();
	void setGame(MyGame* game){this->game = game;}
};

#endif /* MYINVITATIONHANDLER_H_ */
