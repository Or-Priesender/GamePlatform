//============================================================================
// Name        : OddEven.cpp
// Author      : Or Priesender
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MyInvitationHandler.h"
using namespace std;

#include "CLI.h"
#include "MyGame.h"

int main() {
	MyInvitationHandler* handler = new MyInvitationHandler();
	CLI* cli = new CLI();
	MyGame* game = new MyGame(new Client(handler));
	cli->setGame(game);
	handler->setGame(game);
	cli->start();
	cli->waitForThread();
	return 0;
}
