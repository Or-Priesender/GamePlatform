/*
 * CommandListener.h
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#ifndef COMMANDLISTENER_H_
#define COMMANDLISTENER_H_
#include "MThread.h"
#include "TCPSocket.h"
#include "SDKProtocol.h"
#include "Authenticator.h"

using namespace std;
using namespace Lab;

//login users listener
class CommandListener : public MThread{
	TCPSocket* socket;
	Authenticator* auth;
	bool stop;
public:
	CommandListener(int port,Authenticator* auth);
	virtual ~CommandListener();
	void run();
	void stopListening();
	void shutdown();

};

#endif /* COMMANDLISTENER_H_ */
