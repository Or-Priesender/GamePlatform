/*
 * Authenticator.h
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#ifndef AUTHENTICATOR_H_
#define AUTHENTICATOR_H_
#include "File.h"
#include "GameUser.h"
#include "TCPSocket.h"
#include "UserHandler.h"
#include "IntParser.h"
#include <fstream>
#include <string>
using namespace Lab;

//Handles user authentication
class Authenticator {
	UserHandler* users;
	int userCounter;
	vector<int> regular;
	vector<int> cypher;
	int encrypt(std::string pass);
public:
	Authenticator(UserHandler* users);
	virtual ~Authenticator();
	bool registerUser(std::string username,std::string password,TCPSocket* clientSock);
	bool loginUser(std::string username,std::string password,TCPSocket* clientSock);
	bool performHandshake(std::string username,TCPSocket* client);
	bool userExists(std::string username);
	void initLists();
};

#endif /* AUTHENTICATOR_H_ */
