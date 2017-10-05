//============================================================================
// Name        : ServerSDK.cpp
// Author      : Or Priesender
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "CommandListener.h"
#include "Authenticator.h"
#include "TCPSocket.h"
#include "SDKProtocol.h"
#include "UserHandler.h"
#include "Server.h"
#include "MultipleSocketListener.h"
#include "ScoreBoardManager.h"
#include "Client.h"

using namespace std;
using namespace Lab;

int main() {


	Server server;
	server.start();
	server.waitForThread();

	return 0;
}
