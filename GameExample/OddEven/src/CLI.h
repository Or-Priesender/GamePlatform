/*
 * CLI.h
 *
 *  Created on: Aug 31, 2017
 *      Author: user
 */

#ifndef CLI_H_
#define CLI_H_
#include "MThread.h"
#include "MyGame.h"
#include <sys/select.h>
#include <sys/time.h>
#include "IntParser.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include "UDPSocket.h"


using namespace Lab;
using namespace std;

//Command line interface for the game
class CLI : public MThread{

MyGame* game;

public:
	CLI();
	virtual ~CLI();
	void run();
	void showUIMenu();
	void showLoginMenu();
	int pollCommand();
	void setGame(MyGame* game){this->game = game;}
};

#endif /* CLI_H_ */
