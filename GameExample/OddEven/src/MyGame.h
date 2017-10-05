/*
 * MyGame.h
 *
 *  Created on: Aug 31, 2017
 *      Author: user
 */

#ifndef MYGAME_H_
#define MYGAME_H_
#include "Client.h"
#include <vector>
#include <sys/select.h>

using namespace std;
using namespace Lab;

//this class uses the client class to communicate with the server
class MyGame {
	Client* client;
	bool busy;
	bool waiting;
	UDPSocket* dummy;
	int dummyPort;


	void playAsMaster();
	void playAsSlave();
public:
	MyGame(Client* client);
	virtual ~MyGame();

	bool handleLogin(int command);
	void handleInterface(int command);
	bool handleInvitation(std::string username);
	void handleGame();
	void printVector(vector<std::string> v);
	bool startGameWithUser(std::string username);
	bool startGameAuto();
	bool isBusy(){return this->busy;}
	void setBusy(bool b){this->busy = b;}
	char getCharOrBlockUntilInterupted();
	void interrupt();
	void disconnectFromServer(){client->disconnect();}
	int getIntFromUser();
};

#endif /* MYGAME_H_ */
