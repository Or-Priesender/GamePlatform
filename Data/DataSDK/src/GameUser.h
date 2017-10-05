/*
 * GameUser.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef SRC_GAMEUSER_H_
#define SRC_GAMEUSER_H_

#include "TCPSocket.h"
#include "SDKProtocol.h"
using namespace Lab;

//store a user and his connection socket with the server
class GameUser {
	std::string username;
	TCPSocket* socket;
	bool inSession;
public:
	GameUser(std::string username,TCPSocket* socket);
	virtual ~GameUser();
	TCPSocket* getSocket();
	void setSocket(TCPSocket* socket);
	std::string getUsername();
	void setInSession(bool b);
	bool isInSession();
};

#endif /* SRC_GAMEUSER_H_ */
