/*
 * Server.h
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "SDKProtocol.h"
#include "MultipleSocketListener.h"
#include "GameUser.h"
#include "CommandListener.h"
#include "UserHandler.h"
#include <vector>
#include "Authenticator.h"
#include "MThread.h"

using namespace std;
using namespace Lab;

//The main server class
class Server : public MThread{
	Authenticator* auth;
	CommandListener *listener;
	UserHandler *users;

public:
	Server();
	virtual ~Server();
	void run();
	void waitForThread();
	void printUsers();
};

#endif /* SERVER_H_ */
