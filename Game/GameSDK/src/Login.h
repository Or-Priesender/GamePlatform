/*
 * Login.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef SRC_LOGIN_H_
#define SRC_LOGIN_H_
#include <stdlib.h>
#include <iostream>
#include "TCPSocket.h"
#include "SDKProtocol.h"
#include "GameUser.h"

using namespace std;
using namespace Lab;

//This class handles the login procedure
class Login{
public:

	GameUser* registerOrLoginUser(std::string username, std::string password,std::string action);
};


#endif /* SRC_LOGIN_H_ */
