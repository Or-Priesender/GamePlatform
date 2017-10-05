/*
 * InvitationHandler.h
 *
 *  Created on: Aug 24, 2017
 *      Author: user
 */

#ifndef SRC_INVITATIONHANDLER_H_
#define SRC_INVITATIONHANDLER_H_

#include "TCPSocket.h"
#include "SDKProtocol.h"

using namespace Lab;

//Interface to handle invitations - should be overriden by the game
class InvitationHandler {
public:
	InvitationHandler();
	virtual ~InvitationHandler();
	virtual bool handleInvitation(std::string username,TCPSocket* userSock)=0;
	virtual void acceptInvitation(TCPSocket* userSock)=0;
	virtual void declineInvitation(TCPSocket* userSock)=0;
	virtual void activateAfterGameAccepted()=0;

};

#endif /* SRC_INVITATIONHANDLER_H_ */
